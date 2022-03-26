import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.ticker as tck
matplotlib.use('Agg')
matplotlib.rcParams['text.usetex'] = True
matplotlib.rcParams['font.family'] = 'serif'
matplotlib.rcParams['ytick.direction'] = 'in'
matplotlib.rcParams['xtick.direction'] = 'in'
plt.rcParams["figure.facecolor"] = "#0c1117"
matplotlib.rcParams.update({'text.color': '#eff6fc'})


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
ax.xaxis.label.set_color("#eff6fc")
ax.yaxis.label.set_color("#eff6fc")

ax.spines['bottom'].set_color('#eff6fc')
ax.spines['top'].set_color('#eff6fc') 
ax.spines['right'].set_color('#eff6fc')
ax.spines['left'].set_color('#eff6fc')
ax.tick_params(colors='#eff6fc', which='both')

ax.set_facecolor("#0c1117")

cbar = plt.colorbar()
cbar.set_label('Z Values', rotation=270,color = "#eff6fc")
cbar.ax.yaxis.set_tick_params(color="#eff6fc")
cbar.outline.set_edgecolor("#eff6fc")
plt.setp(plt.getp(cbar.ax.axes, 'yticklabels'), color="#eff6fc")

plt.savefig("2D_Map.png", dpi = 300)
