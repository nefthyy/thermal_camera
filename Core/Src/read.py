import serial
import numpy as np
import matplotlib.pyplot as plt

ser = serial.Serial("COM3", 1000000)   # your COM port and baud rate

img = plt.imshow(np.zeros((24, 32)), cmap="inferno")
plt.colorbar()

while True:
    junk = ser.read_until(b"FRAME")     # wait for the start of a frame
    if len(junk) > 5:                   # show any debug text sent before it
        print(junk[:-5].decode(errors="replace"))
    data = ser.read(3072)               # 768 floats x 4 bytes
    temps = np.frombuffer(data, dtype=np.float32).reshape(24, 32)

    img.set_data(temps)
    img.set_clim(135,160)
    plt.pause(0.01)
    print(temps[0:10])