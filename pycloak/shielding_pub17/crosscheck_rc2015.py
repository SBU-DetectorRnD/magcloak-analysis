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

data_rc2015 = pd.read_csv("results/copy_rc2015_wide_tape_1layer_sheath_fit_results.csv")
data = pd.read_csv("results/dipole_shielding_sheath_1layer.csv")

# sort values
data_rc2015 = data_rc2015.sort_values('Bo',ascending=True)
data = data.sort_values('Bout',ascending=True)


print( data_rc2015.head(10))

fig, axs = plt.subplots(1,1,figsize=(6,5))
#, linestyle='-'

axs.errorbar( data_rc2015['Bo'], data_rc2015['Bi'], yerr=data_rc2015['sig_bi'].values, color='r', marker='.', linestyle='')
axs.errorbar( data['Bout'], data['Bins'], yerr=data['Bins_sdev'].values, color='b', marker='.', linestyle='')

axs.set_xlabel("$B_{mri} (mT)$")
axs.set_ylabel("$B_{in} (mT)$")

axs.set_title("\TeX\ 45-layer MRI shielding")

#plt.xlabel(r'\textbf{time} (s)')
#plt.ylabel(r'\textit{voltage} (mV)',fontsize=16)
#plt.title(r"\TeX\ is Number "
#          r"$\displaystyle\sum_{n=1}^\infty\frac{-e^{i\pi}}{2^n}$!",
#          fontsize=16, color='gray')

plt.show()

