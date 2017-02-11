#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

# set figure parameters
fig, axs = plt.subplots(1,1,figsize=(6,5))
axs.tick_params(labelsize=12)
axs.set_title("45-layer MRI shielding")
plt.xlabel("time (s)",fontsize=12)
plt.ylabel("$B_{in}$ (mT)",fontsize=12)

# read data
df3 = pd.read_csv("data-calib/DATA_MegaVIEW/DataFile_2016-12-08_02-12-19.csv") #440 mT
#df2 = pd.read_csv("data-calib/DATA_MegaVIEW/DataFile_2016-12-08_03-10-16.csv") #500 mT
df2 = pd.read_csv("data-calib/DATA_MegaVIEW/DataFile_2016-12-08_03-29-53.csv") #520 mT
df1 = pd.read_csv("data-calib/DATA_MegaVIEW/DataFile_2016-12-08_04-09-26.csv") #560 mT

# sort values
df1 = df1.sort_values('time',ascending=True)
df2 = df2.sort_values('time',ascending=True)
df3 = df3.sort_values('time',ascending=True)

# mean reference field
Bnom1 = abs(df1['B2']).mean()
Bnom2 = abs(df2['B2']).mean()
Bnom3 = abs(df3['B2']).mean()

# convert mean reference field to label
lab1 = str("B = ") + str(int(round(Bnom1))) + str(" mT")
lab2 = str("B = ") + str(int(round(Bnom2))) + str(" mT")
lab3 = str("B = ") + str(int(round(Bnom3))) + str(" mT")

# plot data
axs.errorbar( df1['time'], abs(df1['B3']), yerr=0, marker='.', label=lab1 )
axs.errorbar( df2['time'], abs(df2['B3']), yerr=0, marker='.', label=lab2 )
axs.errorbar( df3['time'], abs(df3['B3']), yerr=0, marker='.', label=lab3 )

# add legend
axs.legend(loc = 'center left', prop={'size':12})

# save & show plot
plt.savefig("plots/shielding_mri_45layer_timedep.png")
plt.show()
