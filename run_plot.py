import random
from itertools import count
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

plt.style.use('fivethirtyeight')

x_vals = []
y_vals = []

index = count()


def animate(i):
    data = pd.read_csv('power.csv')
    x = data['time']
    y = data['power_root']

    plt.cla()

    plt.ylabel("power(mW)")
    plt.plot(x, y, label='power_root', linewidth=2)
    # plt.plot(x, y2, label='array_loop', linewidth=2)
    # plt.plot(x, y2, label='mqtt', linewidth=2)

    plt.legend(loc='upper left')
    plt.tight_layout()


ani = FuncAnimation(plt.gcf(), animate, interval=2000)

plt.tight_layout()
plt.show()
