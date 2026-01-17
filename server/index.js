const express = require("express");
const bodyParser = require("body-parser");
const { SerialPort } = require("serialport");

const HTTP_PORT = 3000;

// CHANGE THIS FOR YOUR SETUP
const SERIAL_PORT = "COM3"; // Windows example
// const SERIAL_PORT = "/dev/ttyUSB0"; // Linux
// const SERIAL_PORT = "/dev/tty.usbmodem14101"; // macOS

const BAUD_RATE = 9600;

const app = express();
app.use(bodyParser.json());

// Serial connection
const port = new SerialPort({
  path: SERIAL_PORT,
  baudRate: BAUD_RATE,
});

port.on("open", () => {
  console.log("Serial connected:", SERIAL_PORT);
});

port.on("error", (err) => {
  console.error("Serial error:", err.message);
});

// Keep track of last bomb state to avoid spam
let lastBombState = null;

app.post("/", (req, res) => {
  const bomb = req.body.bomb;
  if (!bomb) {
    return res.sendStatus(200);
  }

  // Bomb planted
  if (bomb.state === "planted" && lastBombState !== "planted") {
    const time = Math.ceil(bomb.countdown);
    sendToArduino(`BOMB_PLANTED:${time}`);
  }

  // Bomb defusing
  if (bomb.state === "defusing") {
    const defuseTime = Math.ceil(bomb.defuse_countdown);
    sendToArduino(`DEFUSING:${defuseTime}`);
  }

  // Bomb defused
  if (bomb.state === "defused" && lastBombState !== "defused") {
    sendToArduino("BOMB_DEFUSED");
  }

  // Bomb exploded
  if (bomb.state === "exploded" && lastBombState !== "exploded") {
    sendToArduino("BOMB_EXPLODED");
  }

  lastBombState = bomb.state;
  res.sendStatus(200);
});

function sendToArduino(message) {
  console.log("→ Arduino:", message);
  port.write(message + "\n");
}

app.listen(HTTP_PORT, () => {
  console.log(`GSI server listening on http://localhost:${HTTP_PORT}`);
});
