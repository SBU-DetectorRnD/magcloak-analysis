#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

# set plotting style
import mycolors
mcol = mycolors.pub17
plt.style.use("../style_pub17/cloak17_paper.mplstyle")

# set figure parameters
fig, axs = plt.subplots(1,1,figsize=(6,5))
#axs.set_title("2-layer Van de Graaff shielding")
plt.xlabel("t (minutes)")
plt.ylabel("$B_{in}$ (mT)")

# read data
#df1 = pd.read_csv("data-calib/DATA_Gaussmeter/DataFile_170131_135134.csv") #10 mT
#df1 = pd.read_csv("data-calib/DATA_Gaussmeter/DataFile_170221_170726.csv") #10 mT
df1 = pd.read_csv("data-calib/DATA_Gaussmeter/DataFile_170221_172156.csv") #15 mT
df2 = pd.read_csv("data-calib/DATA_Gaussmeter/DataFile_170208_215133.csv") #30 mT
df3 = pd.read_csv("data-calib/DATA_Gaussmeter/DataFile_170131_143117.csv") #45 mT

# sort values
df1 = df1.sort_values('time',ascending=True)
df2 = df2.sort_values('time',ascending=True)
df3 = df3.sort_values('time',ascending=True)

# mean reference field
Bnom1 = abs(df1['Bnom']).mean()
Bnom2 = abs(df2['Bnom']).mean()
Bnom3 = abs(df3['Bnom']).mean()

# convert mean reference field to label
lab1 = str("$B_{out}$ = ") + str(int(round(Bnom1))) + str(" mT")
lab2 = str("$B_{out}$ = ") + str(int(round(Bnom2))) + str(" mT")
lab3 = str("$B_{out}$ = ") + str(int(round(Bnom3))) + str(" mT")

#Bstart_1 = -1*df1.loc[ df1['time'] < 60]['B1'].mean()
#Bstart_3 = -1*df3.loc[ df3['time'] < 60]['B1'].mean()
#print(Bstart_1,Bstart_3)

# plot data

axs.errorbar( df3['time']/60, -1*df3['B1'], yerr=0, marker='.', label=lab3, color=mcol[1] )
#axs.errorbar( df2['time']/60, -1*df2['B1'], yerr=0, marker='.', label=lab2, color=mcol[2] )
axs.errorbar( df1['time']/60, -1*df1['B1'], yerr=0, marker='.', label=lab1, color=mcol[0] )


#axs.set_yscale("log", nonposy='clip')

# add legend
axs.legend(loc = 'center right')
#axs.set_xlim(0,600)

# save & show plot
plt.savefig("plots/eps/shielding_vdg2layer_timedep.eps")
plt.savefig("plots/png/shielding_vdg2layer_timedep.png")
plt.show()
