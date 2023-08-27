# Raspberry Pico Waveshare LCD Demo
[Waveshare 240 x 240 LCD with Joypad + Buttons](Pico-LCD-1.3-1.jpg) demo for Raspberry Pico

Modified version of [Raspberry Pico Example](https://github.com/raspberrypi/pico-examples/tree/master/pio/st7789_lcd) for
[Waveshare 1.3 Inch LCD panel](https://www.waveshare.com/wiki/Pico-LCD-1.3) with Joypad and 4 buttons.

The original demo works well with the Waveshare LCD by simply changing the pin definitions as shown

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

This demo extends the original by adding functionality for the Joypad and Buttons on the Waveshare panel.

Panel initialisation has been simplified and modified for 90 Degree LCD orientation.


<details><summary>Joypad Functions</summary>
<p>

| Joypad     | Function              |
|------------|-----------------------|
| `UP`       |  Static Image - Up    |
| `DOWN`     |  Static Image - Down  |
| `LEFT`     |  Static Image - Left  |
| `RIGHT`    |  Static Image - Right |
| `CENTRE`   |  (Re)Start Animation  | 

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





