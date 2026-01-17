# Arduino Bomb Timer Firmware

Sketch and configuration for the Arduino microcontroller that displays the bomb countdown and manages the LCD/LED displays.

## Overview

This folder contains the Arduino code that:

- Receives bomb events via serial communication from the Node.js server
- Displays countdown timers on a 16x2 LCD display
- Controls an LED indicator for visual feedback
- Handles bomb states: planted, defusing, defused, exploded

## Hardware Requirements

### Components

- Arduino Uno, Mega, or compatible microcontroller
- 16x2 LCD display (HD44780 compatible)
- 1 LED (any color)
- 220-330Ω resistor (for LED)
- USB cable for programming
- Jumper wires

### Pin Configuration

The default pin mapping in `config.h` is:

| Component  | Pin |
| ---------- | --- |
| LCD RS     | 12  |
| LCD Enable | 11  |
| LCD D4     | 5   |
| LCD D5     | 4   |
| LCD D6     | 3   |
| LCD D7     | 2   |
| LED        | 8   |

Adjust these values in `config.h` to match your actual wiring.

## Wiring Diagram

```
Arduino                LCD (16x2)
────────────────────────────────────────────────
Pin 12  ─────────────────────────── RS (4)
Pin 11  ─────────────────────────── E  (6)
Pin 5   ─────────────────────────── D4 (11)
Pin 4   ─────────────────────────── D5 (12)
Pin 3   ─────────────────────────── D6 (13)
Pin 2   ─────────────────────────── D7 (14)

Pin 8  ─┬─────────────┬─────────── LED (+)
        │           │
        │         220Ω (resistor)
        │           │
       GND ─────────┘

5V  ──────────────────────────────── VCC (2)
GND ──────────────────────────────── GND (1, 5, 16)
```

## Installation

1. **Wire your components** according to the pin configuration

2. **Open Arduino IDE**
   - File → Open → `lcd_timer.ino`

3. **Select your board**
   - Tools → Board → Select your Arduino model
   - Tools → Port → Select the COM port your Arduino is connected to

4. **Upload the sketch**
   - Click the Upload button (arrow icon) or press Ctrl+U
   - Wait for "Upload complete" message

5. **Verify**
   - The LCD should display "CS2 Bomb Timer" briefly
   - Then show a blank screen ready for events

## Files

### lcd_timer.ino

Main firmware. Handles:

- Serial communication from server
- LCD display updates
- Timer countdown logic
- LED blinking patterns

### config.h

Configuration file containing:

- Pin definitions
- Serial baud rate
- Default bomb timer duration

## Serial Commands

The Arduino listens for these commands from the server:

```
BOMB_PLANTED:40    → Start 40-second countdown, light up LED
DEFUSING:5         → Show 5-second defuse timer
BOMB_DEFUSED       → Display "Bomb Defused", turn off LED
BOMB_EXPLODED      → Display "Bomb Exploded", turn off LED
```

## LCD Display Output

```
Normal (waiting):
┌────────────────┐
│                │
│ (blank screen) │
│                │
└────────────────┘

Bomb Planted:
┌────────────────┐
│ Bomb: 40 sec   │
│ Time: 00:40    │
└────────────────┘

Defusing:
┌────────────────┐
│ Defusing...    │
│ Time: 00:05    │
└────────────────┘

End State:
┌────────────────┐
│ Bomb Defused   │
└────────────────┘
```

## Troubleshooting

### LCD shows nothing

- Check power connections (5V and GND)
- Verify pin numbers in config.h
- Adjust LCD contrast with potentiometer (if equipped)

### LCD shows garbled text

- Double-check D4-D7 pin connections
- Verify LCD_RS and LCD_E pins
- Try adjusting the contrast

### No serial communication

- Check USB cable and Arduino port in IDE
- Verify baud rate is 9600 in config.h
- Test with Serial Monitor in Arduino IDE

### LED doesn't light up

- Check LED polarity (longer leg = +, shorter = -)
- Verify resistor connection
- Test LED separately

## Customization

### Change Baud Rate

Edit in `config.h` and `index.js` (server):

```cpp
#define SERIAL_BAUD 115200  // Change from 9600
```

### Change Pins

Edit `config.h`:

```cpp
#define LCD_RS 12  // Use different pin
#define LCD_E  11
#define LED_PIN 8
```

### Modify Display Messages

Edit in `lcd_timer.ino`:

```cpp
lcd.print("Custom Message");
```

## Related

- See [server/](../server/) for Node.js backend
- See [cs2-gsi/](../cs2-gsi/) for game state integration setup
- Main [README](../README.md) for complete project documentation

---

**Happy coding! 🔧**
