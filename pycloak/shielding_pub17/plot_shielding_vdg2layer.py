#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

#plt.style.use('ggplot')

#from matplotlib import rc
#rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
#rc('text', usetex=True)

# get data measured with Hall sensor
data = pd.read_csv("results/shielding_vdg2layer.csv")

# sort values
data = data.sort_values('Bout',ascending=True)

# get beam data
data_beam = pd.read_csv("MagCloak-beam-spot-analysis-v12_c_results_short.csv")

print(data_beam.head(5))

data_beam_subset = data_beam.loc[ (data_beam['run'] == 1) &
                                  (data_beam['temp'] == 'cryo') &
                                  (data_beam['type'] == 'main') &
                                  (data_beam['type2'] == 'distance') ]

data_beam_subset2 = data_beam.loc[ (data_beam['run'] == 1) &
                                   (data_beam['temp'] == 'room') &
                                   (data_beam['type'] == 'main') &
                                   (data_beam['type2'] == 'distance') ]

## make test plot
#plt.errorbar( data_beam_subset['Bnom'], data_beam_subset['x_mean'], yerr=data_beam_subset['x_sdev'].values, marker='o')
#plt.errorbar( data_beam_subset2['Bnom'], data_beam_subset2['x_mean'], yerr=data_beam_subset2['x_sdev'].values, marker='o')
#plt.xlabel("B (mT)")
#plt.ylabel("displacement (mm)")
#plt.show()

# make plot
fig, axs = plt.subplots(2,1,figsize=(6,5))

axs[0].set_title("2-layer Van de Graaff prototpye in beam steering dipole shielding")
axs[0].errorbar( data_beam_subset['Bnom'], data_beam_subset['x_mean'], yerr=data_beam_subset['x_sdev'].values, marker='o')
axs[0].errorbar( data_beam_subset2['Bnom'], data_beam_subset2['x_mean'], yerr=data_beam_subset2['x_sdev'].values, marker='o')
#axs[1].set_xlabel("$B_{out} (mT)$")
axs[0].set_ylabel("beam displacement (mm)")
axs[0].set_xlim((0,50))

plt1=axs[1].plot( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], marker='o')
axs[1].errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, marker='o', color=plt1[0].get_color(), mfc='white')
axs[1].set_xlabel("$B_{out}$ (mT)")
axs[1].set_ylabel("$B_{in}$ (mT)")
axs[1].set_xlim((0,50))

plt.savefig("plots/shielding_vdg2layer.png")
plt.show()

