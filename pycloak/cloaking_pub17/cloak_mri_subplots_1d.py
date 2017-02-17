import os
import sys

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from matplotlib import cm

# set plotting style
import mycolors
mcol = mycolors.pub17
plt.style.use("../style_pub17/cloak17_paper.mplstyle")

# make plot 1
fig, axs = plt.subplots(2,1,figsize=(6,5),sharex=True)

# mark cloak dimensions
axs[0].axvline(-2.032, color=mcol[2], linestyle='-',alpha=0.5)
axs[0].axvline(2.032, color=mcol[2], linestyle='-',alpha=0.5)

axs[1].axvline(-2.032, color=mcol[2], linestyle='-',alpha=0.5)
axs[1].axvline(2.032, color=mcol[2], linestyle='-',alpha=0.5)

# Compare SC+FM and SC only at 450 mT 'in front of cloak', i.e. 1 cm away
df_bvx_cloak_1 = pd.read_csv('data-calib/DATA_MegaVIEW/DataFile_2016-12-09_12-12-19.csv')
df_bvx_sc_1 = pd.read_csv('data-calib/DATA_MegaVIEW/DataFile_2016-12-09_15-06-03.csv')

# Compare SC+FM and SC only at 450 mT 'across cloak'
df_bvx_cloak_2 = pd.read_csv('data-calib/DATA_MegaVIEW/DataFile_2016-12-09_12-42-47.csv')
df_bvx_sc_2 = pd.read_csv('data-calib/DATA_MegaVIEW/DataFile_2016-12-09_15-09-34.csv')


#axs[0].set_title("B vs X scans, 45 layer SC, fM = 0.618")
axs[0].plot( df_bvx_sc_1['x']/10, -1*df_bvx_sc_1['B3']/1000, marker='o', color=mcol[1], label='45-layer SC') 
axs[0].plot( df_bvx_cloak_1['x']/10, -1*df_bvx_cloak_1['B3']/1000, marker='o', color=mcol[0], label='cloak') 
axs[0].set_ylabel("$B_T$ (T)")
#axs[0].set_ylim((0.375,0.455))
#axs[0].text(-120,420,'1 cm away')
axs[0].legend(loc = 'lower left')

select_bvx_left = df_bvx_sc_2['x'] < -10
select_bvx_right = df_bvx_sc_2['x'] > 10

axs[1].plot( df_bvx_sc_2.loc[select_bvx_left,'x']/10,
             -1*df_bvx_sc_2.loc[select_bvx_left,'B3']/1000,
             marker='o', color=mcol[1], label='45-layer SC') 
axs[1].plot( df_bvx_sc_2.loc[select_bvx_right,'x']/10,
             -1*df_bvx_sc_2.loc[select_bvx_right,'B3']/1000,
             marker='o', color=mcol[1], label='45-layer SC') 

axs[1].plot( df_bvx_cloak_2['x']/10, -1*df_bvx_cloak_2['B3']/1000, marker='o', color=mcol[0], label='cloak') 
axs[1].set_ylabel("$B_T$ (T)")
axs[1].set_xlabel("position (cm)")
#axs[1].set_ylim((0.440,0.485))
#axs[1].text(-120,470,'across')

plt.savefig("plots/eps/cloak_mri_450mT_1d_combined.eps")
plt.savefig("plots/png/cloak_mri_450mT_1d_combined.png")
plt.show()
