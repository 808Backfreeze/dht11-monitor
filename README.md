# DHT11 Monitor

This project uses a DHT11 sensor to check humidity and temperature. It uses an Arduino to read the sensor, and a Python script checks every minute and records the data.

## Features

- Logs temperature (°F) and humidity readings every 60 seconds
- Records mode changes to the log file
- Three display modes:
  - **Default (`b`)** — shows both humidity and temperature
  - **`T`** — shows temperature only on the Arduino LCD
  - **`H`** — shows humidity only
  - **`B`** — resets to both
- Data is stored in `dht11_log.txt` in the same folder as the Python script
- Works as long as the computer is connected to the Arduino

## Requirements

### Hardware
- Arduino (Uno or compatible)
- DHT11 sensor (connected to pin 2)
- 16x2 LCD (connected to pins 7, 8, 9, 10, 11, 12)

### Software
- Arduino IDE with the following libraries:
  - `DHT sensor library` (Adafruit)
  - `LiquidCrystal`
- Python 3.x with `pyserial` installed:
  ```
  pip install pyserial
  ```

## Usage

1. Upload `DHT11-3.ino` to your Arduino
2. Run `DHT11Logger.py` on your computer (ensure the correct COM port is set)
3. Use keyboard commands in the Python terminal:
   - `t` — switch to temperature mode
   - `h` — switch to humidity mode
   - `b` — switch to both
   - `q` — quit and disconnect

## Log File

Data is saved to `dht11_log.txt` in the script's directory. Example entries:
```
2026-05-08 21:30 | Temp_F: 72.5 | Humidity: 45.0%
2026-05-08 21:31 | Temp_F: 72.6 | Humidity: 44.8%
```
