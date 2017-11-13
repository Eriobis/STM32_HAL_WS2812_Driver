import serial
import time

s = serial.Serial(port = "COM3", baudrate = "115200")

updateCmd = (0x2B,0,0,0,0,0)
delay = 0.04

while 1:
    for power in range (1, 5):
    
        for i in range (0, 30):
            intensity = power*i
            color = 0x0,0x0,intensity
            payload = (0x3d,0x00,i)
            s.write(payload)
            s.write(color)
            payload = (0x3d,0x00,35-i)
            s.write(payload)
            s.write(color)
            s.write(updateCmd)
            time.sleep(delay)
        for i in range (0, 30):
            intensity = power*i
            color = 0x0,intensity,0x0
            payload = (0x3d,0x00,i)
            s.write(payload)
            s.write(color)
            payload = (0x3d,0x00,35-i)
            s.write(payload)
            s.write(color)
            s.write(updateCmd)
            time.sleep(delay)
        for i in range (0, 30):
            intensity = power*i
            color = intensity,0x0,0x0
            payload = (0x3d,0x00,i)
            s.write(payload)
            s.write(color)
            payload = (0x3d,0x00,35-i)
            s.write(payload)
            s.write(color)
            s.write(updateCmd)
            time.sleep(delay)
        for i in range (0, 30):
            intensity = power*i
            color = 0x0,intensity,intensity
            payload = (0x3d,0x00,i)
            s.write(payload)
            s.write(color)
            payload = (0x3d,0x00,35-i)
            s.write(payload)
            s.write(color)
            s.write(updateCmd)
            time.sleep(delay)
        
        