#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt


# set figure parameters
figsize_x = 6
figsize_y = 5

# read data
data = pd.read_csv("results/mri_shielding_45layer.csv")

# sort values
data = data.sort_values('Bout',ascending=True)

# scale field readings from mT to T
data['Bins'] /= 1000
data['Bout'] /= 1000
data['Bins_sdev'] /= 1000
data['Bout_sdev'] /= 1000

# include 1% shielding referenchttps line
ref_x = np.arange(0.1,1.1,0.01)
ref_y = ref_x * 0.01

# plot data: Combined- Top
fig_a = plt.figure( figsize = (figsize_x, figsize_y) )

ax1 = plt.subplot(2, 1, 1)
ax1.tick_params(labelsize=12)
ax1.get_xaxis().set_visible(False)

# mark 0.5 and 1 T
ax1.axvline(0.5, color='grey', linestyle='--')
ax1.axvline(1.0, color='grey', linestyle='--')

# reference line
ax1.plot(ref_x,ref_y, color='g', linestyle='--')

# data
ax1.plot( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], color='b', marker='o')
ax1.errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, color='b', marker='o', mfc='white')

plt.ylabel("$B_{in}$ (T)",fontsize=12)


# plot data: Combined- Bottom
ax2 = plt.subplot(2, 1, 2)

# mark 0.5 and 1 T
ax2.axvline(0.5, color='grey', linestyle='--')
ax2.axvline(1.0, color='grey', linestyle='--')

# ref line
ax2.plot(ref_x,ref_y, color='g', linestyle='--')

# actual data
ax2.plot( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], color='b', marker='o')
ax2.errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, color='b', marker='o', mfc='white')

ax2.set_yscale("log", nonposy='clip')

plt.xlabel("$B_{out}$ (T)",fontsize=12)
plt.ylabel("$B_{in}$ (T)",fontsize=12)

# reduce whitespace
plt.tight_layout()
plt.show()
plt.savefig("plots/shielding_mri_45layer_linlog.png")


##############################################

# set figure parameters                                                                                                               
fig, axs = plt.subplots(1,1,figsize=(6,5))
axs.tick_params(labelsize=12)

axs.set_title("45-layer MRI shielding")
plt.xlabel("$B_{out}$ (T)",fontsize=12)
plt.ylabel("$B_{in}$ (T)",fontsize=12)

# mark 0.5 and 1 T
plt.axvline(0.5, color='grey', linestyle='--')
plt.axvline(1.0, color='grey', linestyle='--')

# plot data
axs.plot( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], color='b', marker='o')
axs.errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, color='b', marker='o', mfc='white')

# save & show plot
plt.savefig("plots/shielding_mri_45layer_lin.png")



#######
# log version of plot with 99% shielding line
#fig, axs = plt.subplots(1,1,figsize=(6,5))

axs.set_yscale("log", nonposy='clip')

# include 1% shielding reference line
ref_x = np.arange(0.1,1.1,0.01)
ref_y = ref_x * 0.01
axs.plot(ref_x,ref_y, color='g', linestyle='--')

# plot data
axs.plot( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], color='b', marker='o')
axs.errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, color='b', marker='o', mfc='white')

axs.set_yscale("log", nonposy='clip')

# save & show plot
plt.savefig("plots/shielding_mri_45layer_log.png")


