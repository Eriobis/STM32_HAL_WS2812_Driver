import serial
import time

s = serial.Serial(port = "COM3", baudrate = "115200")

while 1:
    for i in range (0, 30):
        payload = (0x3d,0x00,i,0x0,0x0,8*i)
        s.write(payload)
        payload = (0x3d,0x00,35-i,0x0,0x0,8*i)
        s.write(payload)
        payload = (0x2B,0,0,0,0,0)
        s.write(payload)
        time.sleep(0.02)
    for i in range (0, 30):
        payload = (0x3d,0x00,i,0x0,8*i,0)
        s.write(payload)
        payload = (0x3d,0x00,35-i,0x0,8*i,0x0)
        s.write(payload)
        payload = (0x2B,0,0,0,0,0)
        s.write(payload)
        time.sleep(0.02)
    for i in range (0, 30):
        payload = (0x3d,0x00,i,8*i,0,0)
        s.write(payload)
        payload = (0x3d,0x00,35-i,8*i,0x0,0)
        s.write(payload)
        payload = (0x2B,0,0,0,0,0)
        s.write(payload)
        time.sleep(0.02)
    for i in range (0, 30):
        payload = (0x3d,0x00,i,0x0,8*i,8*i)
        s.write(payload)
        payload = (0x2B,0,0,0,0,0)
        s.write(payload)
        time.sleep(0.02)
    for i in range (0, 30):
        payload = (0x3d,0x00,i,8*i,8*i,0)
        s.write(payload)
        payload = (0x2B,0,0,0,0,0)
        s.write(payload)
        time.sleep(0.02)
        