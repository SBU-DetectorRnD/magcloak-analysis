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
ax.set_yscale("log", nonposy='clip')
plt.xlabel("position (mm)",fontsize=12)
plt.ylabel("$B_{T}$ (mT) + 0.1 mT",fontsize=12)

# mark cloak dimensions
plt.axvline(-500, color='grey', linestyle='--')
plt.axvline(500, color='grey', linestyle='--')
plt.axhline(0.1, color='grey', linestyle='-',alpha=0.5)

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
plt1=ax.plot( data_shield['pos'], data_shield['B1']+0.1, marker='.', label='shield')
plt2=ax.plot( data_noshield['pos'], data_noshield['B1']+0.1, marker='.', label='no shield')

# plot cosmetics: set axis parameters
# ax.set_xlim(-12000, 12000)
# ax.set_ylim(0,500)

# add legend
#ax.legend(loc = 'upper right', prop={'size':12})
ax.text(-195, 2.3, 'superconductor',fontsize=12,color=plt1[0].get_color())
ax.text(30, 29, 'no superconductor',fontsize=12,color=plt2[0].get_color())

# save & display
plt.savefig("plots/shield_BvsZ_vdg2layer.png")
plt.show()
