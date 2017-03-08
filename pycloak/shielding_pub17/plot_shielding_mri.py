#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

import mycolors

# set plotting style
plt.style.use("../style_pub17/cloak17_paper.mplstyle")
#../style_pub17/cloak17_slides.mplstyle

# pick colors
mcol = mycolors.pub17

# set figure parameters
figsize_x = 6
figsize_y = 5

# read data
data = pd.read_csv("results/mri_shielding_45layer.csv")

# sort values
data = data.sort_values('Bout',ascending=True)

# once time dependence is established, set all time dependences at higher fields to TRUE
B_first_time_dependence = (data.loc[data['tdep'],'Bout'].values)[0]
print('First time dependence at B (mT) = ', B_first_time_dependence)
data.loc[ data['Bout'] >= B_first_time_dependence, 'tdep'] = True

# duplicate point at transition from no time dependence to time dependence for cleaner plotting
data_dupl = (data[data['Bout'] == B_first_time_dependence]).copy(deep=True)
data_dupl.loc[:,'tdep'] = False
data = data.append( data_dupl )

# sort values again
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
ax1.get_xaxis().set_visible(False)

# mark 0.5 and 1 T
ax1.axvline(0.5, color=mcol[2], linestyle='-', alpha=0.3)
ax1.axvline(1.0, color=mcol[2], linestyle='-', alpha=0.3)

# reference line
ax1.plot(ref_x,ref_y, color=mcol[1], linestyle='-')

# data
plt1 = ax1.errorbar( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], yerr=data.loc[data['tdep']==False,'Bins_sdev'].values, color=mcol[0], marker='o', label="Not time dependent")
ax1.errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, color=plt1[0].get_color(), marker='o', mfc='white', label="Time dependent")

#ax1.set_yticks(np.arange(0,1.2,0.2))
ax1.set_yticks([0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0])
ax1.set_yticklabels([0.0,'','','','',0.5,'','','','',1.0])
ax1.set_ylabel("$B_{in}$ (T)",labelpad=15)

plt.legend(loc="upper left")

# plot data: Combined- Bottom
ax2 = plt.subplot(2, 1, 2)

# mark 0.5 and 1 T
ax2.axvline(0.5, color=mcol[2], linestyle='-', alpha=0.3)
ax2.axvline(1.0, color=mcol[2], linestyle='-', alpha=0.3)

# ref line
ax2.plot(ref_x,ref_y, color=mcol[1], linestyle='-')
ax2.text(0.65,0.001,"$B_{in} = B_{out} \cdot 0.01$",color=mcol[1])

# actual data
plt2 = ax2.errorbar( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], data.loc[data['tdep']==False,'Bins'], color=mcol[0],  marker='o')
ax2.errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, color=plt2[0].get_color(), marker='o', mfc='white')

ax2.set_yscale("log", nonposy='clip')
ax2.set_yticks((1e-6,1e-3,1))
ax2.set_xticks([0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1])
ax2.set_xticklabels([0.0,'','','','',0.5,'','','','',1.0,''])
ax2.set_xlabel("$B_{out}$ (T)")
ax2.set_ylabel("$B_{in}$ (T)")

# reduce whitespace
plt.tight_layout()
plt.savefig("plots/png/shielding_mri_45layer_linlog.png")
plt.savefig("plots/eps/shielding_mri_45layer_linlog.eps")
plt.show()

##############################################

# set figure parameters                                                                                                               
fig, axs = plt.subplots(1,1,figsize=(6,5))

axs.set_title("45-layer MRI shielding")
plt.xlabel("$B_{out}$ (T)")
plt.ylabel("$B_{in}$ (T)")

# mark 0.5 and 1 T
plt.axvline(0.5, color=mcol[2], linestyle='-')
plt.axvline(1.0, color=mcol[2], linestyle='-')

# plot data
axs.plot( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], color='b', marker='o')
axs.errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, color='b', marker='o', mfc='white')

# save & show plot
plt.savefig("plots/png/shielding_mri_45layer_lin.png")
plt.savefig("plots/eps/shielding_mri_45layer_lin.eps")


#######
# log version of plot with 99% shielding line
#fig, axs = plt.subplots(1,1,figsize=(6,5))

axs.set_yscale("log", nonposy='clip')

# include 1% shielding reference line
ref_x = np.arange(0.1,1.1,0.01)
ref_y = ref_x * 0.01
axs.plot(ref_x,ref_y, color=mcol[1], linestyle='-')

# plot data
axs.plot( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], color='b', marker='o')
axs.errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, color='b', marker='o', mfc='white')

axs.set_yscale("log", nonposy='clip')

# save & show plot
plt.savefig("plots/png/shielding_mri_45layer_log.png")
plt.savefig("plots/eps/shielding_mri_45layer_log.eps")
