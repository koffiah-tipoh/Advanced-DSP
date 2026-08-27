import serial
import matplotlib
matplotlib.use('Qt5Agg')  # add this BEFORE importing pyplot
import matplotlib.pyplot as plt
from collections import deque

ser = serial.Serial('/dev/ttyACM0', 115200)
data = deque(maxlen=200)

plt.ion()
fig, ax = plt.subplots()
line, = ax.plot(data)

while True:
    raw = ser.readline().decode().strip()
    try:
        value = float(raw)
        data.append(value)
        line.set_ydata(data)
        line.set_xdata(range(len(data)))
        ax.relim()
        ax.autoscale_view()
        plt.pause(0.01)
    except ValueError:
        pass
