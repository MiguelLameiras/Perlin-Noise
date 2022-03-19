import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.ticker as tck
matplotlib.use('Agg')
matplotlib.rcParams['text.usetex'] = True
matplotlib.rcParams['font.family'] = 'serif'
matplotlib.rcParams['ytick.direction'] = 'in'
matplotlib.rcParams['xtick.direction'] = 'in'

x,y,z = [],[],[]

file = open("bin/map.txt")

for line in file:
    row = line.split(" ")
    x.append(int(row[0]))
    y.append(int(row[1]))
    z.append(float(row[2]))

plt.scatter(x, y, c=z, cmap='viridis')

plt.title("Perlin Noise", fontsize=18)
plt.xlabel("x", fontsize=18)
plt.ylabel("y", fontsize=18)

ax = plt.gca()
ax.xaxis.set_minor_locator(tck.AutoMinorLocator())
ax.yaxis.set_minor_locator(tck.AutoMinorLocator())

cbar = plt.colorbar()
cbar.set_label('Z Values', rotation=270)

plt.savefig("2D_Map.png", dpi = 300)
