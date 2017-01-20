#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

from matplotlib import rc
rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
## for Palatino and other serif fonts use:
#rc('font',**{'family':'serif','serif':['Palatino']})
rc('text', usetex=True)

#data0 = pd.read_csv("results/dipole_shielding_sheath_1layer.csv")
data1 = pd.read_csv("results/helmholtz_1layer_rc2015.csv")
data2 = pd.read_csv("results/helmholtz_2layer_rc2015.csv")
data3 = pd.read_csv("results/helmholtz_3layer_rc2015.csv")
data4 = pd.read_csv("results/helmholtz_4layer_rc2015.csv")

# sort values
#data0 = data0.sort_values('Bout',ascending=True)
data1 = data1.sort_values('Bout',ascending=True)
data2 = data2.sort_values('Bout',ascending=True)
data3 = data3.sort_values('Bout',ascending=True)
data4 = data4.sort_values('Bout',ascending=True)

fig, axs = plt.subplots(1,1,figsize=(6,5))

mark='.'
c1='r'
c2='g'
c3='orange'
c4='b'

#axs.plot(data0['Bout'],data0['Bins'])

axs.errorbar( data1['Bout'], data1['Bins'], yerr=data1['Bins_sdev'].values, color=c1, marker=mark, label='1 layer')
axs.errorbar( data1.loc[data1['tdep'],'Bout'], data1.loc[data1['tdep'],'Bins'], yerr=data1.loc[data1['tdep'],'Bins_sdev'].values, color=c1, marker=mark, mfc='white', label=None)

axs.errorbar( data2['Bout'], data2['Bins'], yerr=data2['Bins_sdev'].values, color=c2, marker=mark, label='2 layer')
axs.errorbar( data2.loc[data2['tdep'],'Bout'], data2.loc[data2['tdep'],'Bins'], yerr=data2.loc[data2['tdep'],'Bins_sdev'].values, color=c2, marker=mark, mfc='white', label=None)

axs.errorbar( data3['Bout'], data3['Bins'], yerr=data3['Bins_sdev'].values, color=c3, marker=mark, label='3 layer')
axs.errorbar( data3.loc[data3['tdep'],'Bout'], data3.loc[data3['tdep'],'Bins'], yerr=data3.loc[data3['tdep'],'Bins_sdev'].values, color=c3, marker=mark, mfc='white', label=None)

axs.errorbar( data4['Bout'], data4['Bins'], yerr=data4['Bins_sdev'].values, color=c4, marker=mark, label='4 layer')
axs.errorbar( data4.loc[data4['tdep'],'Bout'], data4.loc[data4['tdep'],'Bins'], yerr=data4.loc[data4['tdep'],'Bins_sdev'].values, color=c4, marker=mark, mfc='white', label=None)


axs.legend(loc='upper left')

axs.set_xlabel("$B_{mri} (mT)$")
axs.set_ylabel("$B_{in} (mT)$")

axs.set_title("1-4 layer Helmholtz shielding")

#plt.xlabel(r'\textbf{time} (s)')
#plt.ylabel(r'\textit{voltage} (mV)',fontsize=16)
#plt.title(r"\TeX\ is Number "
#          r"$\displaystyle\sum_{n=1}^\infty\frac{-e^{i\pi}}{2^n}$!",
#          fontsize=16, color='gray')

plt.savefig("plots/shielding_helmholtz_rc2015.png")
plt.show()

