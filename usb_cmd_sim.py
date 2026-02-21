import serial
import time

# CHANGE THIS to your ESP32 serial port
# Windows: "COM3", "COM4", ...
# Linux:   "/dev/ttyUSB0" or "/dev/ttyACM0"
# macOS:   "/dev/tty.usbserial-xxxx" or "/dev/tty.usbmodemxxxx"
PORT = "COM5"
BAUD = 9600

commands = ["BIN_1"]
#commands = ["BIN_1", "BIN_2", "BIN_3"]


ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)  # allow ESP32 to reset after opening serial

print("Sending commands...")

idx = 0
while True:
    cmd = commands[idx]
    ser.write((cmd + "\n").encode("utf-8"))
    print(f"Sent: {cmd}")

    idx = (idx + 1) % len(commands)
    time.sleep(6)
