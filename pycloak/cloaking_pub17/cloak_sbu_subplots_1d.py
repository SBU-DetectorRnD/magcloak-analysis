import os
import sys

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from matplotlib import cm




# make plot 1
fig, axs = plt.subplots(3,1,figsize=(6,6))

# BvX, 1 mm away from edge @ z-center: DataFile_170213_194029.txt
df_bvx_cloak_1 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170213_194029.csv')

# BvX, 1 cm away from edge @ z-center: DataFile_170213_201438.txt
df_bvx_cloak_2 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170213_201438.csv')

# BvX along axis of cloak @ z-center: DataFile_170213_202411.txt
df_bvx_cloak_3 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170213_202411.csv')

# BvX reference along axis of cloak @ z-center: DataFile_170213_203450.txt
df_bvx_ref_1 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170213_203450.csv')

# BvX reference 1mm away from edge @ z-center: DataFile_170213_204446.txt
df_bvx_ref_2 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170213_204446.csv')

# BvX reference 1cm away from edge @ z-center: DataFile_170213_205815.txt
df_bvx_ref_3 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170213_205815.csv')

axs[0].set_title("B vs X scans, 4 layer SC, fM = 0.618")
axs[0].plot( df_bvx_cloak_2['pos'], abs(df_bvx_cloak_2['B1']), marker='o', label='1 cm away') 
axs[0].plot( df_bvx_ref_2['pos'], abs(df_bvx_ref_2['B1']), marker='o', label='1 cm away', alpha=0.4) 
axs[0].set_ylabel("$B$ (mT)")
axs[0].set_ylim((38,40.5))
axs[0].text(175,38.2,'1 cm away',fontsize=12)

axs[1].plot( df_bvx_cloak_1['pos'], abs(df_bvx_cloak_1['B1']), marker='o', label='1 mm away') 
axs[1].plot( df_bvx_ref_1['pos'], abs(df_bvx_ref_1['B1']), marker='o', label='1 mm away', alpha=0.4) 
axs[1].set_ylabel("$B$ (mT)")
axs[1].set_ylim((38,40.5))
axs[1].text(175,38.2,'1 mm away',fontsize=12)

axs[2].plot( df_bvx_cloak_3['pos'], abs(df_bvx_cloak_3['B1']), marker='o', label='across') 
axs[2].plot( df_bvx_ref_3['pos'], abs(df_bvx_ref_3['B1']), marker='o', label='across', alpha=0.4) 
axs[2].set_ylabel("$B$ (mT)")
axs[2].set_xlabel("position (mm)")
axs[2].set_ylim((38,40.5))
axs[2].text(175,38.2,'across',fontsize=12)

#plt1=axs[1].plot( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], marker='o')
#axs[1].errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, marker='o', color=plt1[0].get_color(), mfc='white')

plt.savefig("plots/cloak_sbu_bvx_1.png")
plt.show()
