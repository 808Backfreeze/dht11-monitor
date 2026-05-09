import serial
import time
import threading
from datetime import datetime
import os

PORT = "COM3"
BAUD = 9600
LOG_FILE = "dht11_log.txt"
LOG_INTERVAL = 60

ser = serial.Serial(PORT, BAUD, timeout=2)
time.sleep(2)

current_mode = "b"
running = True
latest_line = None

print("Connected to Arduino.")
print("Logging file:", os.path.abspath(LOG_FILE))
print("Commands: t=temp, h=humidity, b=both, q=quit")

def parse_and_log(line):
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M")

    if line == "ERROR":
        log_line = f"{timestamp} | ERROR\n"

    elif line.startswith("TEMP_F,"):
        value = float(line.split(",")[1])
        log_line = f"{timestamp} | Temp_F: {value:.1f}\n"

    elif line.startswith("HUMIDITY,"):
        value = float(line.split(",")[1])
        log_line = f"{timestamp} | Humidity: {value:.1f}%\n"

    elif line.startswith("BOTH,"):
        parts = line.split(",")
        temp = float(parts[1])
        hum = float(parts[2])
        log_line = f"{timestamp} | Temp_F: {temp:.1f} | Humidity: {hum:.1f}%\n"

    else:
        log_line = f"{timestamp} | RAW: {line}\n"

    with open(LOG_FILE, "a", encoding="utf-8") as f:
        f.write(log_line)

    print("Logged:", log_line.strip())

def request_reading():
    global latest_line
    try:
        ser.reset_input_buffer()
        ser.write((current_mode + "\n").encode())

        line = ser.readline().decode(errors="ignore").strip()
        if line:
            latest_line = line
            print("Arduino:", line)
            return line
    except Exception as e:
        print("Serial error:", e)
        return None

def logger_thread():
    global running
    while running:
        line = request_reading()
        if line:
            parse_and_log(line)

        for _ in range(LOG_INTERVAL):
            if not running:
                break
            time.sleep(1)

thread = threading.Thread(target=logger_thread, daemon=True)
thread.start()

try:
    while True:
        cmd = input("Enter command: ").strip().lower()

        if cmd == "q":
            running = False
            break

        elif cmd in ["t", "h", "b"]:
            current_mode = cmd
            print(f"Mode changed to: {current_mode}")
            line = request_reading()
            if line:
                print("Updated Mode")
        else:
            print("Invalid command")

except KeyboardInterrupt:
    running = False

ser.close()
print("Disconnected.")
