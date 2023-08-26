/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <math.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/interp.h"

#include "st7789_lcd.pio.h"
#include "raspberry_256x256_rgb565.h"

// Tested with Waveshare 1.3 inch LCD Display Module only

#define IMAGE_SIZE 256
#define LOG_IMAGE_SIZE 8
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240

// LCD Module Pins (hardwired)

#define PIN_DC 8
#define PIN_CS 9
#define PIN_CLK 10
#define PIN_DIN 11
#define PIN_RESET 12
#define PIN_BL 13

#define KEY_A 15
#define KEY_B 17
#define KEY_X 19
#define KEY_Y 21

#define JOY_UP 2
#define JOY_DOWN 18
#define JOY_LEFT 16
#define JOY_RIGHT 20
#define JOY_CENTRE 3

// Sitronix ST7789VW Commands

#define NOP     0x00        // No Operation
#define SWRESET 0x01        // Software Reset
#define SLPOUT  0x11        // Sleep Out
#define NORON   0x13        // Normal Display Mode On
#define INVOFF  0x20        // Display Inversion Off
#define INVON   0x21        // Display Inversion On
#define DISPOFF 0x28        // Display Off
#define DISPON  0x29        // Display On
#define CASET   0x2A        // Column Address Set 
#define RASET   0x2B        // Row Address Set
#define RAMWR   0x2C        // RAM Write
#define RAMRD   0x2E        // RAM Read
#define TEOFF   0x34        // Line Tearing Effect Off
#define TEON    0x35        // Line Tearing Effect On
#define MADCTL  0x36        // Memory Data Access Control
#define IDMOFF  0x38        // Idle Mode Off
#define IDMON   0x39        // Idle Mode On
#define COLMOD  0x3A        // Interface Pixel Format
#define WRMEMC  0x3C        // Write Memory Continue
#define WRDISBV 0x51        // Write Display Brightness
#define PORCTRL 0xB2        // Porch Control

#define SERIAL_CLK_DIV 1.f

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Format: cmd length (including cmd byte), post delay in units of 5 ms, then command payload.
// Simplified initialisation using power-on / swreset default values, including CASET and RASET.

static const uint8_t st7789_init_seq[] = {
        1, 2, NOP,                                  // No Operation - 10mS power on delay
        1, 2, SWRESET,                              // Software reset, 10mS stability delay
        1, 2, SLPOUT,                               // Exit sleep mode, 10mS stability delay
        2, 0, COLMOD, 0x55,                         // Set colour mode to 16 bits/pixel
        2, 0, MADCTL, 0x70,                         // Set MADCTL for LCD 90 degree physical hardware orientation 
        1, 0, INVON,                                // Colour inversion on (positive image with supplied data)
        1, 0, DISPON,                               // Main screen display on
        1, 1, NOP,                                  // No Operation - 5mS guard time 
        0                                           // Terminate list
};

static inline void lcd_set_dc_cs(bool dc, bool cs) {
    sleep_us(1);
    gpio_put_masked((1u << PIN_DC) | (1u << PIN_CS), !!dc << PIN_DC | !!cs << PIN_CS);
    sleep_us(1);
}

static inline void lcd_write_cmd(PIO pio, uint sm, const uint8_t *cmd, size_t count) {
    st7789_lcd_wait_idle(pio, sm);
    lcd_set_dc_cs(0, 0);
    st7789_lcd_put(pio, sm, *cmd++);
    if (count >= 2) {
        st7789_lcd_wait_idle(pio, sm);
        lcd_set_dc_cs(1, 0);
        for (size_t i = 0; i < count - 1; ++i)
            st7789_lcd_put(pio, sm, *cmd++);
    }
    st7789_lcd_wait_idle(pio, sm);
    lcd_set_dc_cs(1, 1);
}

static inline void lcd_init(PIO pio, uint sm, const uint8_t *init_seq) {
    const uint8_t *cmd = init_seq;
    while (*cmd) {
        lcd_write_cmd(pio, sm, cmd + 2, *cmd);
        sleep_ms(*(cmd + 1) * 5);
        cmd += *cmd + 2;
    }
}

static inline void st7789_start_pixels(PIO pio, uint sm) {
    uint8_t cmd = RAMWR;
    lcd_write_cmd(pio, sm, &cmd, 1);
    lcd_set_dc_cs(1, 0);
}

static inline void set_panel_gpio(uint gpio_pin_number) {
    gpio_init(gpio_pin_number);
    gpio_pull_up(gpio_pin_number);
    gpio_set_dir(gpio_pin_number, GPIO_IN);
}

static inline float get_theta(float theta, float theta_max, float theta_inc) {

    if(theta > theta_max)
    return theta - theta_max;

    if(!gpio_get(JOY_UP))
    return theta_max * 0.00f;
 
    if(!gpio_get(JOY_DOWN))
    return theta_max * 0.50f; 

    if(!gpio_get(JOY_LEFT))
    return theta_max * 0.25f;  

    if(!gpio_get(JOY_RIGHT))
    return theta_max * 0.75f; 

    if(!gpio_get(JOY_CENTRE))
    return theta_max * 1.00f;
    
    return theta + theta_inc;
}

static inline float get_theta_inc(float theta_inc) {

    if (!gpio_get(KEY_A) && (theta_inc <= +1.0f))
    return theta_inc - 0.0002f;

    if (!gpio_get(KEY_B) && (theta_inc >= -1.0f))
    return theta_inc + 0.0002f;

    if (!gpio_get(KEY_X))
    return 0.0f;

    if (!gpio_get(KEY_Y)) {
        while (!gpio_get(KEY_Y));
        return -theta_inc;
    }

    if(!gpio_get(JOY_CENTRE))
    return -0.02f;

    return theta_inc;
}

static inline void panel_init()
{

    // Waveshare 1.3 inch LCD Display Module, Joypad and 4 buttons

    set_panel_gpio(KEY_A);
    set_panel_gpio(KEY_B);
    set_panel_gpio(KEY_X);
    set_panel_gpio(KEY_Y);

    set_panel_gpio(JOY_UP);
    set_panel_gpio(JOY_DOWN);
    set_panel_gpio(JOY_LEFT);
    set_panel_gpio(JOY_RIGHT);
    set_panel_gpio(JOY_CENTRE);
}

int main() {

    stdio_init_all();

    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &st7789_lcd_program);

    st7789_lcd_program_init(pio, sm, offset, PIN_DIN, PIN_CLK, SERIAL_CLK_DIV);
    
    panel_init();

    gpio_init(PIN_BL);
    gpio_init(PIN_CS);
    gpio_init(PIN_DC);
    gpio_init(PIN_RESET);
   
    gpio_set_dir(PIN_BL, GPIO_OUT);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_set_dir(PIN_RESET, GPIO_OUT);
    
    gpio_put(PIN_BL, 1);
    gpio_put(PIN_CS, 1);
    gpio_put(PIN_RESET, 1);
    lcd_init(pio, sm, st7789_init_seq);
 
    // Lane 0 will be u coords (bits 8:1 of addr offset), lane 1 will be v
    // coords (bits 16:9 of addr offset), and we'll represent coords with
    // 16.16 fixed point. ACCUM0,1 will contain current coord, BASE0/1 will
    // contain increment vector, and BASE2 will contain image base pointer

    #define UNIT_LSB 16
    
    interp_config lane0_cfg = interp_default_config();
    interp_config_set_shift(&lane0_cfg, UNIT_LSB - 1); // -1 because 2 bytes per pixel
    interp_config_set_mask(&lane0_cfg, 1, 1 + (LOG_IMAGE_SIZE - 1));
    interp_config_set_add_raw(&lane0_cfg, true); // Add full accumulator to base with each POP
    
    interp_config lane1_cfg = interp_default_config();
    interp_config_set_shift(&lane1_cfg, UNIT_LSB - (1 + LOG_IMAGE_SIZE));
    interp_config_set_mask(&lane1_cfg, 1 + LOG_IMAGE_SIZE, 1 + (2 * LOG_IMAGE_SIZE - 1));
    interp_config_set_add_raw(&lane1_cfg, true);

    interp_set_config(interp0, 0, &lane0_cfg);
    interp_set_config(interp0, 1, &lane1_cfg);
    
    interp0->base[2] = (uint32_t) raspberry_256x256;

    float theta = 0.0f;
    float theta_inc = 0.0f;
    float theta_max = 2.0f * (float) M_PI;
  
    while (1) {

        theta_inc = get_theta_inc(theta_inc);

        theta = get_theta(theta, theta_max, theta_inc);

        int32_t rotate[4] = {
                (int32_t) (cosf(theta) * (1 << UNIT_LSB)), (int32_t) (-sinf(theta) * (1 << UNIT_LSB)),
                (int32_t) (sinf(theta) * (1 << UNIT_LSB)), (int32_t) (+cosf(theta) * (1 << UNIT_LSB))
        };

        interp0->base[0] = rotate[0] ;
        interp0->base[1] = rotate[2] ;

        st7789_start_pixels(pio, sm);
        
        for (int y = 0; y < SCREEN_HEIGHT; ++y) {
            interp0->accum[0] = rotate[1] * y;
            interp0->accum[1] = rotate[3] * y;
            for (int x = 0; x < SCREEN_WIDTH; ++x) {
                uint16_t colour = *(uint16_t *) (interp0->pop[2]);
                st7789_lcd_put(pio, sm, colour >> 8);
                st7789_lcd_put(pio, sm, colour & 0xff);
            }
        }
    }
}
