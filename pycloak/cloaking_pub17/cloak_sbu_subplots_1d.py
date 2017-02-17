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

xcenter=141
ycenter=0
zcenter=71

# make plot 1
fig, axs = plt.subplots(2,1,figsize=(6,5),sharex=True)

# mark cloak dimensions
axs[0].axvline(-2.032, color=mcol[2], linestyle='-',alpha=0.5)
axs[0].axvline(2.032, color=mcol[2], linestyle='-',alpha=0.5)

axs[1].axvline(-2.032, color=mcol[2], linestyle='-',alpha=0.5)
axs[1].axvline(2.032, color=mcol[2], linestyle='-',alpha=0.5)

# 4 layer SC + fm618 BvX along axis of cloak @ z-center: DataFile_170213_202411.txt
# 4 layer SC + fm618 BvX, 1 mm away from edge @ z-center: DataFile_170213_194029.txt
# 4 layer SC + fm618 BvX, 1 cm away from edge @ z-center: DataFile_170213_201438.txt
# BvX reference along axis of cloak @ z-center: DataFile_170213_203450.txt
# BvX reference 1mm away from edge @ z-center: DataFile_170213_204446.txt
# BvX reference 1cm away from edge @ z-center: DataFile_170213_205815.txt
# 4 layer SC only: BvsX along cloak axis at z-center (,y = 78mm, z = 71mm): DataFile_170215_162938.txt
# 4 layer SC only: BvsX 1mm from surface at z-center (, y = 103mm, z = 71mm): DataFile_170215_163835.txt
# 4 layer SC only: BvsX 33.1 mm from cloak center in z-center (y = 111mm, z = 71mm): DataFile_170215_165018.txt


df_bvx_cloak_1 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170213_202411.csv')
df_bvx_cloak_2 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170213_194029.csv')
df_bvx_cloak_3 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170213_201438.csv')

df_bvx_sc4l_1 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170215_162938.csv')
df_bvx_sc4l_2 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170215_163835.csv')
df_bvx_sc4l_3 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170215_165018.csv')

df_bvx_ref_1 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170213_203450.csv')
df_bvx_ref_2 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170213_204446.csv')
df_bvx_ref_3 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170213_205815.csv')

#axs[1].set_title("B vs X scans, 4 layer SC, fM = 0.618")
axs[1].plot( (df_bvx_ref_1['pos']-xcenter)/10, -1*df_bvx_ref_1['B1'], marker='o', color=mcol[2], label='across center') 

select_bvx_left = (df_bvx_sc4l_1['pos']-xcenter) < -10
select_bvx_right = (df_bvx_sc4l_1['pos']-xcenter) > 10
axs[1].plot( (df_bvx_sc4l_1.loc[select_bvx_left,'pos']-xcenter)/10,
             -1*df_bvx_sc4l_1.loc[select_bvx_left,'B1'],
             marker='o', color=mcol[1], label='across center') 
axs[1].plot( (df_bvx_sc4l_1.loc[select_bvx_right,'pos']-xcenter)/10,
             -1*df_bvx_sc4l_1.loc[select_bvx_right,'B1'],
             marker='o', color=mcol[1], label='across center') 

select_bvx_left = (df_bvx_cloak_1['pos']-xcenter) < -10
select_bvx_right = (df_bvx_cloak_1['pos']-xcenter) > 10
axs[1].plot( (df_bvx_cloak_1.loc[select_bvx_left,'pos']-xcenter)/10,
             -1*df_bvx_cloak_1.loc[select_bvx_left,'B1'],
             marker='o', color=mcol[0], label='across center') 
axs[1].plot( (df_bvx_cloak_1.loc[select_bvx_right,'pos']-xcenter)/10,
             -1*df_bvx_cloak_1.loc[select_bvx_right,'B1'],
             marker='o', color=mcol[0], label='across center') 

axs[1].set_ylabel("$B_T$ (mT)")
axs[1].set_ylim((36,49))
#axs[1].text(175,38.2,'across',fontsize=12)
axs[1].set_xlabel("position (cm)")

axs[0].plot( (df_bvx_ref_2['pos']-xcenter)/10, -1*df_bvx_ref_2['B1'], marker='o', color=mcol[2], label='reference') 
axs[0].plot( (df_bvx_sc4l_2['pos']-xcenter)/10, -1*df_bvx_sc4l_2['B1'], marker='o', color=mcol[1], label='4-layer SC') 
axs[0].plot( (df_bvx_cloak_2['pos']-xcenter)/10, -1*df_bvx_cloak_2['B1'], marker='o', color=mcol[0], label='cloak') 
axs[0].set_ylabel("$B_T$ (mT)")
axs[0].set_ylim((29,41.5))
#axs[0].text(175,38.2,'1 mm away',fontsize=12)
axs[0].legend(loc = 'lower left')

#axs[2].plot( df_bvx_cloak_3['pos'], abs(df_bvx_cloak_3['B1']), marker='o', label='1 cm away') 
#axs[2].plot( df_bvx_sc4l_3['pos'], abs(df_bvx_sc4l_3['B1']), marker='o', label='across center') 
#axs[2].plot( df_bvx_ref_3['pos'], abs(df_bvx_ref_3['B1']), marker='o', label='1 cm away', alpha=0.4) 
#axs[2].set_ylabel("$B$ (mT)")
#axs[2].set_ylim((38,40.5))
#axs[2].text(175,38.2,'1 cm away',fontsize=12)

plt.savefig("plots/eps/cloak_sbu_bvx_1.eps")
plt.savefig("plots/png/cloak_sbu_bvx_1.png")
plt.show()
###############################################

# make plot 2
fig, axs = plt.subplots(2,1,figsize=(6,5),sharex=True)

# mark cloak dimensions
axs[0].axvline(-5.715, color=mcol[2], linestyle='-',alpha=0.5)
axs[0].axvline(5.715, color=mcol[2], linestyle='-',alpha=0.5)

axs[1].axvline(-5.715, color=mcol[2], linestyle='-',alpha=0.5)
axs[1].axvline(5.715, color=mcol[2], linestyle='-',alpha=0.5)

# BvZ, through the cloak axis @ x,y-center: DataFile_170214_185331.txt
# BvZ, 1mm away from edge @ y-center: DataFile_170214_190443.txt (x=118mm)
# BvZ, 1mm away from edge @ x-center: DataFile_170214_191525.txt (y=105mm)
# BvZ reference, through the cloak axis @ x,y-center: DataFile_170214_201515.txt
# BvZ reference, 1mm away from edge @ y-center: DataFile_170214_202512.txt (x=118mm)
# BvZ reference, 1mm away from edge @ x-center: DataFile_170214_203513.txt (y=105mm)
# BvsZ SC only through center of cloak (y = 75mm, x = 142mm): DataFile_170215_181018.txt
# BvsZ SC only 1 mm from cloak edge in z top (y = 103mm, x = 142mm): DataFile_170215_173242.txt
# BvsZ SC only 1mm from cloak edge in x (y=75m, x=113mm): DataFile_170215_182257.txt

df_bvz_cloak_1 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170214_185331.csv')
df_bvz_cloak_2 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170214_190443.csv')
df_bvz_cloak_3 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170214_191525.csv')

df_bvz_sc4l_1 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170215_181018.csv')
df_bvz_sc4l_2 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170215_173242.csv')
df_bvz_sc4l_3 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170215_182257.csv')

df_bvz_ref_1 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170214_201515.csv')
df_bvz_ref_2 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170214_202512.csv')
df_bvz_ref_3 = pd.read_csv('data-calib/DATA_Gaussmeter/DataFile_170214_203513.csv')

#axs[0].set_title("B vs Z scans, 4 layer SC, fM = 0.618")
#axs[0].plot( df_bvz_cloak_1['pos'], abs(df_bvz_cloak_1['B1']), marker='o', label='1 cm away') 
#axs[0].plot( df_bvz_sc4l_1['pos'], abs(df_bvz_sc4l_1['B1']), marker='o', label='1 cm away', alpha=0.4) 
#axs[0].plot( df_bvz_ref_1['pos'], abs(df_bvz_ref_1['B1']), marker='o', label='1 cm away', alpha=0.4) 
#axs[0].set_ylabel("$B$ (mT)")
##axs[0].set_yscale("log", nonposy='clip')
##axs[0].set_ylim((38,40.5))
#axs[0].text(-50,20,'x,y center')

axs[0].plot( (df_bvz_ref_2['pos']-zcenter)/10, -1*df_bvz_ref_2['B1'], marker='o', color=mcol[2], label='reference') 
axs[0].plot( (df_bvz_sc4l_2['pos']-zcenter)/10, -1*df_bvz_sc4l_2['B1'], marker='o', color=mcol[1], label='4-layer SC') 
axs[0].plot( (df_bvz_cloak_2['pos']-zcenter)/10, -1*df_bvz_cloak_2['B1'], marker='o', color=mcol[0], label='cloak') 
axs[0].set_ylabel("$B_T$ (mT)")
axs[0].set_ylim((29,41.5))
#axs[0].text(-50,42,'y center',fontsize=12)
axs[0].legend(loc = 'lower left')

axs[1].plot( (df_bvz_ref_3['pos']-zcenter)/10, -1*df_bvz_ref_3['B1'], marker='o', color=mcol[2], label='across') 
axs[1].plot( (df_bvz_sc4l_3['pos']-zcenter)/10, -1*df_bvz_sc4l_3['B1'], marker='o', color=mcol[1], label='1 cm away') 
axs[1].plot( (df_bvz_cloak_3['pos']-zcenter)/10, -1*df_bvz_cloak_3['B1'], marker='o', color=mcol[0], label='across') 
axs[1].set_ylabel("$B_T$ (mT)")
axs[1].set_xlabel("position (cm)")
axs[1].set_ylim((36,49))
#axs[1].text(-50,42,'x center',fontsize=12)

plt.savefig("plots/eps/cloak_sbu_bvz_1.eps")
plt.savefig("plots/png/cloak_sbu_bvz_1.png")
plt.show()
