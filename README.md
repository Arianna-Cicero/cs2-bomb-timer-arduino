# CS2 Arduino Bomb Timer

Real-time Counter-Strike 2 bomb countdown display using Arduino and LCD hardware.

## Overview

This project connects Counter-Strike 2 to a physical Arduino device with an LCD screen and LED, displaying the in-game bomb timer in real-time. When a bomb is planted in CS2, your Arduino hardware shows the countdown, defuse progress, and explosion/defusal status.

### How It Works

1. **CS2 Game State Integration (GSI)** - CS2 sends game events to a local server via HTTP
2. **Node.js Server** - Receives GSI data and forwards bomb events to Arduino via serial
3. **Arduino + LCD** - Displays countdown timer and controls LED indicator

## Hardware Requirements

- Arduino Uno, Mega, or compatible board
- 16x2 LCD display (HD44780 compatible)
- 1 LED (any color)
- 220-330Ω resistor (for LED)
- USB cable for programming
- Jumper wires

## Quick Start

### 1. Setup Hardware

Wire your Arduino according to the [arduino/README.md](arduino/README.md):

- LCD RS → Pin 12
- LCD Enable → Pin 11
- LCD D4-D7 → Pins 5, 4, 3, 2
- LED → Pin 8 (with resistor)

### 2. Upload Arduino Firmware

1. Open `arduino/lcd_timer/lcd_timer.ino` in Arduino IDE
2. Select your board and COM port
3. Upload the sketch

### 3. Install Node.js Server

```bash
cd server
npm install
```

Edit `server/index.js` to set your Arduino's COM port:

```javascript
const SERIAL_PORT = "COM3"; // Windows
// const SERIAL_PORT = "/dev/ttyUSB0"; // Linux
// const SERIAL_PORT = "/dev/cu.usbserial-*"; // macOS
```

### 4. Configure CS2 GSI

Copy `cs2-gsi/gamestate_integration_arduino.cfg` to your CS2 config folder:

**Windows:**

```
C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive\game\csgo\cfg
```

See [cs2-gsi/README.md](cs2-gsi/README.md) for other platforms.

### 5. Run Everything

1. **Start the server:**

   ```bash
   cd server
   node index.js
   ```

2. **Launch Counter-Strike 2**

3. **Play a match** - Plant the bomb and watch your Arduino display the countdown!

## Project Structure

```
csarduino/
├── arduino/              # Arduino firmware
│   └── lcd_timer/        # Main sketch and config
│       ├── lcd_timer.ino # Arduino code
│       └── config.h      # Pin mappings and settings
├── cs2-gsi/              # CS2 integration
│   └── gamestate_integration_arduino.cfg
├── server/               # Node.js bridge server
│   ├── index.js          # Main server code
│   └── package.json      # Dependencies
└── README.md             # This file
```

## Features

- ✅ Real-time bomb countdown display
- ✅ Defuse progress tracking
- ✅ LED indicator (blinks during countdown)
- ✅ Visual feedback for bomb exploded/defused
- ✅ Automatic state management
- ✅ No performance impact on CS2

## Serial Protocol

The server sends simple text commands to Arduino:

- `BOMB_PLANTED:<seconds>` - Start countdown timer
- `BOMB_DEFUSING:<seconds>` - Show defuse progress
- `BOMB_DEFUSED` - Bomb successfully defused
- `BOMB_EXPLODED` - Bomb detonated

## Configuration

### Arduino (`arduino/lcd_timer/config.h`)

```cpp
#define LCD_RS 12              // LCD pins
#define LCD_E  11
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2
#define LED_PIN 8              // LED pin

#define SERIAL_BAUD 9600       // Serial speed
#define DEFAULT_BOMB_TIME 40   // Fallback timer
```

### Server (`server/index.js`)

```javascript
const HTTP_PORT = 3000; // GSI listening port
const SERIAL_PORT = "COM3"; // Arduino port
const BAUD_RATE = 9600; // Match Arduino
```

## Troubleshooting

### Arduino not responding

- Check COM port in Device Manager (Windows) or `ls /dev/tty*` (Linux/macOS)
- Verify baud rate matches in both Arduino and server (9600)
- Ensure USB cable supports data transfer

### CS2 not sending events

- Verify GSI config file is in correct CS2 cfg folder
- Check server is running on port 3000
- Test server with: `curl -X POST http://localhost:3000 -d "{}"`

### LCD not displaying correctly

- Check wiring matches pin configuration in `config.h`
- Adjust LCD contrast potentiometer
- Verify LCD is 16x2 HD44780 compatible

### Server can't find Arduino

- Close Arduino IDE Serial Monitor (conflicts with server)
- Update SERIAL_PORT in `server/index.js`
- On Linux, add user to dialout group: `sudo usermod -a -G dialout $USER`

## License

See [LICENSE](LICENSE) file for details.

## Contributing

Feel free to submit issues and pull requests!

## Links

- [Arduino Documentation](https://www.arduino.cc/reference/en/)
- [CS2 Game State Integration](https://developer.valvesoftware.com/wiki/Counter-Strike_2/Game_State_Integration)
- [LiquidCrystal Library](https://www.arduino.cc/en/Reference/LiquidCrystal)

---

**Enjoy your physical CS2 bomb timer! 💣⏱️**
