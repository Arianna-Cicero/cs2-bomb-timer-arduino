# Node.js GSI Server

HTTP server that receives Counter-Strike 2 Game State Integration (GSI) events and forwards them to an Arduino via serial connection.

## Overview

This Express.js server acts as a bridge between Counter-Strike 2 and your Arduino. It:

- Listens for HTTP POST requests from CS2's GSI integration
- Parses bomb-related game events
- Sends formatted commands to the Arduino via serial port
- Avoids spam by tracking previous bomb states

## Installation

```bash
npm install
```

This installs the required dependencies:

- **Express.js** - Web framework for HTTP server
- **body-parser** - JSON request parsing
- **SerialPort** - Arduino communication

## Configuration

Edit the top of `index.js` to match your setup:

```javascript
const HTTP_PORT = 3000; // Server listening port
const SERIAL_PORT = "COM3"; // Arduino serial port (Windows)
// const SERIAL_PORT = "/dev/ttyUSB0"; // Linux
const BAUD_RATE = 9600; // Match Arduino SERIAL_BAUD
```

### Finding Your Arduino's COM Port

**Windows**: Device Manager → Ports (COM & LPT) → Look for "USB Serial Device"

**Linux**: `ls /dev/tty*` or `dmesg | grep tty`

**macOS**: `ls /dev/tty.*` or `ls /dev/cu.*`

## Running

```bash
node index.js
```

Expected output:

```
Serial connected: COM3
GSI server listening on http://localhost:3000
```

## API

### POST /

Expects JSON body with bomb event data:

```json
{
  "bomb": {
    "state": "planted",
    "countdown": 40
  }
}
```

### Supported Bomb States

| State      | Description               | Arduino Command            |
| ---------- | ------------------------- | -------------------------- |
| `planted`  | Bomb just planted         | `BOMB_PLANTED:{countdown}` |
| `defusing` | Defusal in progress       | `DEFUSING:{defuse_time}`   |
| `defused`  | Bomb successfully defused | `BOMB_DEFUSED`             |
| `exploded` | Bomb detonated            | `BOMB_EXPLODED`            |

## Debugging

The server logs all communications:

```
→ Arduino: BOMB_PLANTED:40
→ Arduino: BOMB_EXPLODED
```

If you don't see these messages:

1. Verify the CS2 GSI config is installed
2. Check that your firewall allows connections to localhost:3000
3. Make sure Counter-Strike 2 is running a bomb mode match

## Testing with cURL

Test bomb event manually:

```bash
curl -X POST http://localhost:3000 \
  -H "Content-Type: application/json" \
  -d '{"bomb": {"state": "planted", "countdown": 40}}'
```

## File Structure

```
server/
├── index.js         # Main server code
├── package.json     # Dependencies
└── README.md        # This file
```

## Common Issues

**Port already in use**: Change `HTTP_PORT` to 3001 or higher

**Serial port errors**: Verify COM port, USB cable, and Arduino is powered on

**No events received**: Confirm GSI config is in the right CS2 folder

---

See the main [README](../README.md) for full project documentation.
