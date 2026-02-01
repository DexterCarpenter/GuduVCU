# Dev Container

### USB passthrough in WSL2

If not installed already, run in powershell as administrator
```powershell
# Install usbipd-win
winget install --interactive --exact dorssel.usbipd-win
```

Reopen powershell
```powershell
# List USB devices to find your ESP32
usbipd list

# Attach the device (replace BUSID with the ID from the list)
usbipd bind --busid <BUSID>
usbipd attach --wsl --busid <BUSID>
```

After attaching, the device should show up in WSL2 as `/dev/ttyUSB0` or `/dev/ttyACM0`. Check with:

```bash
ls -la /dev/ttyUSB* /dev/ttyACM*
```

Update permissions, if not done already
```bash
sudo chmod 666 /dev/ttyUSB0
```

---
This in devcontainer.json is required:
```json
"runArgs": [
    "--privileged",
    "--device=/dev/ttyUSB0:/dev/ttyUSB0"
]
```
