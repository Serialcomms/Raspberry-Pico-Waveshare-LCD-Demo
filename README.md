# Raspberry Pico Waveshare LCD Demo
[Waveshare 240 x 240 LCD and Joypad](Pico-LCD-1.3-1.jpg) demo for Raspberry Pico

Modified version of [Raspberry Pico Example](https://github.com/raspberrypi/pico-examples/tree/master/pio/st7789_lcd) for
[Waveshare 1.3 Inch LCD panel](https://www.waveshare.com/wiki/Pico-LCD-1.3) with Joypad and 4 buttons.

The original demo works well with the Waveshare panel by simply changing the pin definitions as shown


| Function   | Original Demo Pin | Waveshare Panel Pin | 
| -----------|------------------ | --------------------|
| `DC`       |        3          |         8           |
| `CS`       |        2          |         9           |
| `CLK`      |        1          |         10          |
| `DIN`      |        0          |         11          |
| `RESET`    |        4          |         12          |
| `BL`       |        5          |         13          |

This demo extends the origial by adding functionality for the Joypad and buttons on the Waveshare panel.

<details><summary>Joypad</summary>
<p>

| Joypad     | Function              |
| -----------|---------------------- |
| `UP`       |  Static Image - Up    |
| `DOWN`     |  Static Image - Down  |
| `LEFT`     |  Static Image - Left  |
| `RIGHT`    |  Static Image - Right |
| `CENTRE`   |  (Re)Start Animation  | 

</p>
</details>

| Keypad     | Function              |
| -----------|---------------------- |
| `A`        | Speed up / slow down  |
| `B`        | Slow down / speed up  |
| `X`        | Stop Animation        |
| `Y`        | Animation Direction   |




