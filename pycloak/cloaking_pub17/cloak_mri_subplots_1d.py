import os
import sys

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from matplotlib import cm




# make plot 1
fig, axs = plt.subplots(2,1,figsize=(6,4))

# Compare SC+FM and SC only at 450 mT 'in front of cloak', i.e. 1 cm away
df_bvx_cloak_1 = pd.read_csv('data-calib/DATA_MegaVIEW/DataFile_2016-12-09_12-12-19.csv')
df_bvx_sc_1 = pd.read_csv('data-calib/DATA_MegaVIEW/DataFile_2016-12-09_15-06-03.csv')

# Compare SC+FM and SC only at 450 mT 'across cloak'
df_bvx_cloak_2 = pd.read_csv('data-calib/DATA_MegaVIEW/DataFile_2016-12-09_12-42-47.csv')
df_bvx_sc_2 = pd.read_csv('data-calib/DATA_MegaVIEW/DataFile_2016-12-09_15-09-34.csv')


axs[0].set_title("B vs X scans, 45 layer SC, fM = 0.618")
axs[0].plot( df_bvx_cloak_1['x'], abs(df_bvx_cloak_1['B3']), marker='o', label='1 cm away') 
axs[0].plot( df_bvx_sc_1['x'], abs(df_bvx_sc_1['B3']), marker='o', label='1 cm away', alpha=0.4) 
axs[0].set_ylabel("$B$ (mT)")
axs[0].set_ylim((375,455))
axs[0].text(-120,420,'1 cm away',fontsize=12)

axs[1].plot( df_bvx_cloak_2['x'], abs(df_bvx_cloak_2['B3']), marker='o', label='across') 
axs[1].plot( df_bvx_sc_2['x'], abs(df_bvx_sc_2['B3']), marker='o', label='across', alpha=0.4) 
axs[1].set_ylabel("$B$ (mT)")
axs[1].set_xlabel("position (mm)")
axs[1].set_ylim((440,485))
axs[1].text(-120,470,'across',fontsize=12)

plt.savefig("plots/cloak_mri_450mT_1d_combined.png")
plt.show()
