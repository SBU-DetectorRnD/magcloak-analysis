import os
import sys

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from matplotlib import cm

from scipy.integrate import trapz, simps


# set figure parameters
fig, ax = plt.subplots( figsize=(6,5) )
ax.tick_params(labelsize=12)
ax.set_title("B vs Z Van de Graaff prototype")
plt.xlabel("position (mm)",fontsize=12)
plt.ylabel("$B_{T}$ (mT)",fontsize=12)

# mark cloak dimensions
plt.axvline(-500, color='grey', linestyle='--')
plt.axvline(500, color='grey', linestyle='--')

data_noshield = pd.read_csv("data-calib/DATA_Gaussmeter/DataFile_170208_203721.csv", comment='#')
data_shield = pd.read_csv("data-calib/DATA_Gaussmeter/DataFile_170208_213322.csv", comment='#')

# flip sign of B field measurements
data_noshield['B1'] = -1 * data_noshield['B1']
data_shield['B1'] = -1 * data_shield['B1']

# calibrate z position of scans, i.e. translate from 'base 250' scale to 'base 10' scale
for i in range(0,7):
    data_noshield.loc[ (data_noshield['pos'] > i*1000) & (data_noshield['pos'] < (i+1)*1000), 'pos'] = data_noshield.loc[ (data_noshield['pos'] > i*1000) & (data_noshield['pos'] < (i+1)*1000), 'pos'] - i*1000 + i*250

for i in range(0,7):
    data_shield.loc[ (data_shield['pos'] > i*1000) & (data_shield['pos'] < (i+1)*1000), 'pos'] = data_shield.loc[ (data_shield['pos'] > i*1000) & (data_shield['pos'] < (i+1)*1000), 'pos'] - i*1000 + i*250


# z center at '3005' mm in 'base 250'
zcenter = 3005 - 3000 + 3*250
data_noshield['pos'] = data_noshield['pos'] - zcenter
data_shield['pos'] = data_shield['pos'] - zcenter

# ...

# plot curves
ax.plot( data_noshield['pos'], data_noshield['B1'], marker='o', color='r', label="no shield")
ax.plot( data_shield['pos'], data_shield['B1'], marker='o', color='b', label="with shield")

# plot cosmetics: set axis parameters
# ax.set_xlim(-12000, 12000)
# ax.set_ylim(0,500)

# add legend
ax.legend(loc = 'upper right', prop={'size':12})

# save & display
plt.savefig("plots/shield_BvsZ_vdg2layer.png")
plt.show()
