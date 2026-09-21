# Two-Player Reaction Game (Arduino)

A head-to-head reaction test built with an Arduino, a breadboard, and a 0.96" OLED. Press start, watch three lights count down, and when the buzzer sounds, the first player to hit their button wins. The winner's LED lights up and the OLED shows who won and their reaction time in milliseconds.

## How it works

1. The OLED shows **PRESS START**.
2. Press the start button. The screen shows **GET READY** while three LEDs light one after another.
3. When the last light goes off, the buzzer sounds and the screen shows **GO!**
4. The first player to press their button wins. The buzzer stops, the winner's LED turns on, and the OLED shows the winner and their time (for example `PLAYER 1 / WINS! / 312 ms`).
5. After 5 seconds the LED turns off and the game returns to **PRESS START**.

## Hardware

| Part | Quantity | Notes |
|---|---|---|
| Arduino Uno (or compatible) | 1 | Uses A4/A5 for I2C |
| Breadboard + jumper wires | 1 | |
| 0.96" OLED, 128x64, I2C (SSD1306) | 1 | 4 pins: GND, VCC, SCL, SDA |
| Active buzzer | 1 | |
| Push buttons | 3 | Start, Player 1, Player 2 |
| LEDs (countdown lights) | 3 | |
| LEDs (winner indicators) | 2 | One next to each player button |
| Resistors (about 220 Ω) | 5 | One per LED |

## Pin map

| Component | Arduino pin |
|---|---|
| Start button | 2 |
| Player 1 button | 3 |
| Player 2 button | 9 |
| Buzzer | 8 |
| Countdown LED 1 | 12 |
| Countdown LED 2 | 4 |
| Countdown LED 3 | 7 |
| Player 1 winner LED | 5 |
| Player 2 winner LED | 10 |
| OLED SDA | A4 |
| OLED SCL | A5 |
| OLED VCC | 5V |
| OLED GND | GND |

## Wiring notes

- **Buttons:** Wire one leg of each button to its Arduino pin and the diagonally opposite leg to GND. The sketch uses `INPUT_PULLUP`, so no external resistors are needed. A 4-pin push button has two pairs of legs that are always connected internally. If a button seems stuck or does nothing, rotate it 90°.
- **LEDs:** Long leg (anode) toward the Arduino pin through a resistor, short leg (cathode) to GND.
- **Buzzer:** An active buzzer, driven directly from pin 8. The positive leg goes to pin 8 and the other to GND.
- **Player 2 button:** It is on pin 9 rather than 13. Pin 13 has the Arduino's built-in LED attached, which can interfere with button reads.
- **Common ground:** All components must share GND with the Arduino.

## Software setup

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Open **Sketch > Include Library > Manage Libraries** and install:
   - **Adafruit SSD1306**
   - **Adafruit GFX Library**
   - **Adafruit BusIO** (installed automatically with the two above when you click "Install all")
3. Open `reaction_game.ino`, select your board and port under **Tools**, and upload.

`Wire.h` is built into the Arduino IDE and doesn't need to be installed.

## Troubleshooting

| Problem | Fix |
|---|---|
| Compile error: `Adafruit_SSD1306.h: No such file or directory` | The library isn't installed. Repeat the library setup above. |
| OLED stays blank but the game works | Check the four wires, especially that SDA and SCL aren't swapped. Try changing the I2C address in `display.begin(...)` from `0x3C` to `0x3D`. |
| OLED text is shifted or has noise at the edges | Your screen may use an SH1106 chip instead of SSD1306. Use the SH1106 library instead. |
| Lights run on their own at power-up | The start button is stuck closed. Rotate it 90° on the breadboard. |
| Nothing happens when a button is pressed | Test by touching a jumper from GND to the button's Arduino pin. If that works, the problem is the button wiring. |
| A player LED doesn't light | Check LED polarity and that the resistor and GND are connected. |

## Known behavior

- Player buttons only count once the buzzer starts. Pressing early does nothing, but holding a button down when the buzzer starts wins instantly.
- If both buttons are pressed in the same instant, Player 1 wins, since that button is checked first.
- The start button doesn't respond while the winner is displayed (5 seconds).
- Reaction time is measured from the moment the buzzer starts.

## Ideas for improvements

- False-start penalty (pressing early loses the round)
- Running score across rounds shown on the OLED
- Random delay before the buzzer so players can't predict the start
- Non-blocking timing with `millis()` so the start button works during the results screen
