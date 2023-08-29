# Raspberry Pico Waveshare LCD Demo
#### Waveshare 240 x 240 LCD with Joypad + Buttons demo for Raspberry Pico

<img src="Graphics/Pico-LCD-RP2040.png" alt="Waveshare LCD" title="Waveshare LCD" width="25%" height="25%">

Modified version of [Raspberry Pico SDK Example](https://github.com/raspberrypi/pico-examples/tree/master/pio/st7789_lcd) for
Waveshare 1.3 Inch LCD panel [^1] with Joypad and 4 buttons.

The original demo works well with the Waveshare LCD simply by changing the pin definitions as shown.

This demo extends the original by adding functionality for the 5-way Joypad and 4 buttons on the Waveshare panel.

Panel initialisation has been simplified and modified for 90 degree LCD screen orientation.

Pico built-in LED toggles on/off every (theta_max / 12) degrees rotation.

Click on Releases > Assets for pre-built .uf2 image.

Start animation by clicking the Joypad when centred.

<details><summary>LCD Display Pin Numbers</summary>
<p>
  
| Function   | Original Demo Pin | Waveshare Panel Pin | 
|------------|:-----------------:| :------------------:|
| `DC`       |        3          |         8           |
| `CS`       |        2          |         9           |
| `CLK`      |        1          |         10          |
| `DIN`      |        0          |         11          |
| `RESET`    |        4          |         12          |
| `BL`       |        5          |         13          |

</p>
</details>

<details><summary>Joypad Pins and Functions</summary>
<p>

| Pin | Joypad     | Function              |
|:---:|------------|-----------------------|
|  2  | `UP`       |  Static Image - Up    |
| 18  | `DOWN`     |  Static Image - Down  |
| 16  | `LEFT`     |  Static Image - Left  |
| 20  | `RIGHT`    |  Static Image - Right |
|  3  | `CENTRE`   |  (Re)Start Animation  | 

</p>
</details>

<details><summary>Keypad Pins and Functions</summary>
<p>

| Pin | Keypad     | Function              |
|:---:|:----------:|-----------------------|
| 15  | `A`        | Speed up / slow down  |
| 17  | `B`        | Slow down / speed up  |
| 19  | `X`        | Stop Animation        |
| 21  | `Y`        | Animation Direction   |

</p>
</details>

[^1]:https://www.waveshare.com/wiki/Pico-LCD-1.3



