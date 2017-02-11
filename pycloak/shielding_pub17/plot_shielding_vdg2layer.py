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
rc('text', usetex=True)

data = pd.read_csv("results/shielding_vdg2layer.csv")

# sort values
data = data.sort_values('Bout',ascending=True)

print(data)

fig, axs = plt.subplots(1,1,figsize=(6,5))

axs.plot( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], color='b', marker='o')
axs.errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, color='b', marker='o', mfc='white')
axs.set_xlabel("$B_{out} (mT)$")
axs.set_ylabel("$B_{in} (mT)$")

axs.set_title("2-layer Van de Graaff prototpye in beam steering dipole shielding")

plt.savefig("plots/shielding_vdg2layer.png")
plt.show()

