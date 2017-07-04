import os
import sys

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from matplotlib import cm

from scipy.integrate import trapz, simps

# set plotting style
import mycolors
mcol = mycolors.pub17
plt.style.use("../style_pub17/cloak17_paper.mplstyle")

# set figure parameters
fig, ax = plt.subplots( figsize=(6,5) )
#ax.set_title("B vs Z Van de Graaff prototype")
ax.set_yscale("log", nonposy='clip')
plt.xlabel("position (cm)")
plt.ylabel("$B_{T}$ (mT) + 0.2 mT")

# mark cloak dimensions
plt.axvline(-50.0, color=mcol[2], linestyle='-',alpha=0.5)
plt.axvline(50.0, color=mcol[2], linestyle='-',alpha=0.5)
plt.axhline(0.2, color=mcol[2], linestyle='-',alpha=0.5)

# list of measurements
# B vs Z scans- room - magnet on:
# DataFile_170416_212259.csv

# B vs Z scans- room - magnet off:
# DataFile_170416_214358.csv

# B vs Z scans- cryo - magnet off:
# DataFile_170416_231507.csv
# DataFile_170416_235331.csv

# B vs Z scans- cryo - magnet on:
# DataFile_170417_001337.csv
# DataFile_170417_004340.csv

# Select data to plot
data_noshield = pd.read_csv("data-calib/DATA_Gaussmeter/DataFile_170416_212259.csv", comment='#')
data_noshield_nofield = pd.read_csv("data-calib/DATA_Gaussmeter/DataFile_170416_214358.csv", comment='#')
data_shield = pd.read_csv("data-calib/DATA_Gaussmeter/DataFile_170417_001337.csv", comment='#')
data_shield_nofield = pd.read_csv("data-calib/DATA_Gaussmeter/DataFile_170416_231507.csv", comment='#')

# flip sign of B field measurements
data_noshield['B1'] = -1 * data_noshield['B1']
data_noshield_nofield['B1'] = -1 * data_noshield_nofield['B1']
data_shield['B1'] = -1 * data_shield['B1']
data_shield_nofield['B1'] = -1 * data_shield_nofield['B1']

# correct for z center
zcenter = 1221
data_noshield['pos'] = data_noshield['pos'] - zcenter
data_noshield_nofield['pos'] = data_noshield_nofield['pos'] - zcenter
data_shield['pos'] = data_shield['pos'] - zcenter
data_shield_nofield['pos'] = data_shield_nofield['pos'] - zcenter

# plot curves
plt3=ax.plot( data_noshield['pos']/10, data_noshield['B1']+0.2, marker='o', color=mcol[1], label='no shield, field')
plt4=ax.plot( data_shield['pos']/10, data_shield['B1']+0.2, marker='o', color=mcol[0], label='shield, field')
plt2=ax.plot( data_noshield_nofield['pos']/10, data_noshield_nofield['B1']+0.2, marker='o', color=mcol[3], label='no shield, no-field')
plt1=ax.plot( data_shield_nofield['pos']/10, data_shield_nofield['B1']+0.2, marker='o', color=mcol[2], label='shield, no-field')

# plot cosmetics: set axis parameters
# ax.set_xlim(-12000, 12000)
ax.set_ylim(0.008,30)

ax.legend(loc='lower right')

# save & display
plt.savefig("plots/eps/shield_BvsZ_vdg2layer_2.eps")
plt.savefig("plots/png/shield_BvsZ_vdg2layer_2.png")
plt.show()
