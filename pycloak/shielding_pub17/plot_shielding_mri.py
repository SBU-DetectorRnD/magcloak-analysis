#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

#import load as ld
#import applied_field as af


#from matplotlib import rc
#rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
### for Palatino and other serif fonts use:
##rc('font',**{'family':'serif','serif':['Palatino']})
#rc('text', usetex=True)


# set figure parameters
fig, axs = plt.subplots(1,1,figsize=(6,5))
axs.tick_params(labelsize=12)

axs.set_title("45-layer MRI shielding")
plt.xlabel("$B_{out}$ (T)",fontsize=12)
plt.ylabel("$B_{in}$ (T)",fontsize=12)


#plt.xlabel(r'$B_{out}$ (T)',fontsize=16)
#plt.ylabel(r'$B_{in}$ (T)',fontsize=16)
#axs.set_xlabel("$B_{out} (T)$")
#axs.set_ylabel("$B_{in} (T)$")

#plt.xlabel(r'\textbf{time} (s)')
#plt.ylabel(r'\textit{voltage} (mV)',fontsize=16)
#plt.title(r"\TeX\ is Number "
#          r"$\displaystyle\sum_{n=1}^\infty\frac{-e^{i\pi}}{2^n}$!",
#          fontsize=16, color='gray')


# read data
data = pd.read_csv("results/mri_shielding_45layer.csv")

# sort values
data = data.sort_values('Bout',ascending=True)

# scale field readings from mT to T
data['Bins'] /= 1000
data['Bout'] /= 1000
data['Bins_sdev'] /= 1000
data['Bout_sdev'] /= 1000

# mark 0.5 and 1 T
plt.axvline(0.5, color='grey', linestyle='--')
plt.axvline(1.0, color='grey', linestyle='--')

# plot data
axs.plot( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], color='b', marker='o')
axs.errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, color='b', marker='o', mfc='white')

# save & show plot
plt.savefig("plots/shielding_mri_45layer.png")
#plt.show()



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
plt.show()
