# Counter-Strike 2 Game State Integration Setup

This folder contains the GSI configuration file that enables Counter-Strike 2 to send game events to your Node.js server.

## What is GSI?

Game State Integration (GSI) is Valve's official API that allows CS2 to push real-time game state data to a local server via HTTP. This is how your Arduino knows when the bomb is planted!

## Installation Steps

### 1. Locate Your CS2 Config Folder

**Windows:**

```
C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive\game\csgo\cfg
```

Or try:

```
C:\Users\[YourUsername]\AppData\Local\Steam\steamapps\common\Counter-Strike Global Offensive\game\csgo\cfg
```

**Linux:**

```
~/.local/share/Steam/steamapps/common/Counter-Strike Global Offensive/game/csgo/cfg
```

**macOS:**

```
~/Library/Application Support/Steam/steamapps/common/Counter-Strike Global Offensive/game/csgo/cfg
```

### 2. Copy the Configuration File

1. Copy `gamestate_integration_arduino.cfg` from this folder
2. Paste it directly into your CS2 `cfg` folder from Step 1

**Verify:** The file should be at:

```
[CS2_PATH]/game/csgo/cfg/gamestate_integration_arduino.cfg
```

### 3. Verify Installation

- Launch Counter-Strike 2
- Start a practice match or bomb defusal game
- Your Node.js server should start receiving events

Check your server output for:

```
→ Arduino: BOMB_PLANTED:40
```

## Configuration Details

The `gamestate_integration_arduino.cfg` file configures CS2 to:

- Send HTTP requests to `http://localhost:3000`
- Update bomb state information in real-time
- Track round and map information
- Use a 5-second timeout for requests

### What Data is Sent?

```json
{
  "provider": {
    "name": "Counter-Strike: Global Offensive",
    "appid": 730
  },
  "map": {
    "name": "de_mirage",
    "phase": "live"
  },
  "round": {
    "phase": "plant"
  },
  "bomb": {
    "state": "planted",
    "countdown": 40
  }
}
```

## Troubleshooting

**GSI not working?**

- Verify file is in correct location with exact filename
- Restart Counter-Strike 2 after adding config file
- Check Windows Firewall allows localhost connections
- Ensure Node.js server is running on port 3000

**Server shows no activity?**

- Confirm GSI file exists in CS2 cfg folder
- Start a private/practice match to trigger bomb events
- Check browser: open `http://localhost:3000` (should show Express error)

**Getting POST errors in server?**

- This is normal for some CS2 heartbeat messages without bomb data
- Only messages with bomb events will trigger Arduino commands

---

See the main [README](../README.md) for full project documentation.
