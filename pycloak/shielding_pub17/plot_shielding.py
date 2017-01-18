#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

#import load as ld
#import applied_field as af


from matplotlib import rc
rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
## for Palatino and other serif fonts use:
#rc('font',**{'family':'serif','serif':['Palatino']})
rc('text', usetex=True)

data = pd.read_csv("foo.txt")

print(data)

fig, axs = plt.subplots(1,1,figsize=(6,5))
#, linestyle='-'
axs.plot( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], color='b', marker='o')
axs.errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, color='b', marker='o', mfc='white')
axs.set_xlabel("$B_{mri} (mT)$")
axs.set_ylabel("$B_{in} (mT)$")

axs.set_title("\TeX\ 45-layer MRI shielding")

#plt.xlabel(r'\textbf{time} (s)')
#plt.ylabel(r'\textit{voltage} (mV)',fontsize=16)
#plt.title(r"\TeX\ is Number "
#          r"$\displaystyle\sum_{n=1}^\infty\frac{-e^{i\pi}}{2^n}$!",
#          fontsize=16, color='gray')

plt.show()

