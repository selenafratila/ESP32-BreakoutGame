
# ESP32-BreakoutGame
A hardware-based implementation of the classic Breakout game using an ESP32 microcontroller. This project features a hybrid control system, allowing the player to use either physical buttons or a smartphone via Bluetooth Classic.

## Key Features
- **Dual Control System:** Input handling via GPIO pushbuttons and Bluetooth Serial communication.
- **Real-time Graphics:** Rendered on an OLED 1306 display using SPI/I2C.
- **Multisensory Feedback:** - **Visual:** Red/Green LEDs for Game Over/Win states.
                             - **Acoustic:** PWM-generated melodies via a piezo buzzer.
- **Dynamic Physics:** Collision detection between the ball, paddle, and an 8x4 block array.

## Hardware Requirements
-**Microcontroller:** ESP-32 DevKit.
-**Display:** OLED SSD1306.
-**Peripherals:** 2x Tactile Buttons, 2x LEDs (Red & Green), Piezo Buzzer, Resistors.
<img width="280" height="373" alt="Picture1" src="https://github.com/user-attachments/assets/8c76e551-06c8-4beb-8a28-3a506cca4199" />

## How it Works
1. **Initialization:** The system sets up the Bluetooth serial ("ESP32test"), initializes the display, and configures the GPIO pins.
2. **Input Handling:** The `loop()` function constantly polls for data from the Bluetooth module or state changes on the physical buttons.
3. **Game Engine:**
   - The ball's trajectory is updated every 20ms.
   - If a collision with a block is detected, the block is deactivated (`blocks[i][j] = 0`) and the ball bounces back.
   - The game checks if all blocks are destroyed to trigger the "WIN" state.
4. **End Game:** Depending on the outcome, the system activates the corresponding LED and plays a dedicated melody before restarting.
<img width="280" height="373" alt="Picture2" src="https://github.com/user-attachments/assets/469f3aac-77b3-4818-8f5a-c3abc8c1c609" />

<img width="280" height="373" alt="Picture3" src="https://github.com/user-attachments/assets/19e94ed0-e148-4dbf-b220-5e0b94011312" />

<img width="280" height="373" alt="Picture4" src="https://github.com/user-attachments/assets/e69c671b-f7c7-4bda-b350-8640b64ba8ba" />


##  Installation
1. Clone the repo.
2. Install the `TFT_eSPI` and `BluetoothSerial` libraries in your Arduino IDE.
3. Wire the components according to the pin definitions in the code.
4. Upload the sketch to your ESP32.
