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
#plt.style.use('ggplot')

# get data measured with Hall sensor
data = pd.read_csv("results/shielding_vdg2layer_2.csv")

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

#axs[0].set_title("2-layer Van de Graaff prototpye in beam steering dipole shielding")
axs[0].errorbar( data_beam_subset2['Bnom'], data_beam_subset2['x_mean'], yerr=data_beam_subset2['x_sdev'].values, marker='o', color=mcol[1], label="No SC shield")
axs[0].errorbar( data_beam_subset['Bnom'], data_beam_subset['x_mean'], yerr=data_beam_subset['x_sdev'].values, marker='o', color=mcol[0], label="With SC shield")
#axs[1].set_xlabel("$B_{out} (mT)$")
axs[0].set_ylabel("$\Delta x_{Li7}$ (mm)")
axs[0].set_xlim((0,50))
axs[0].legend(loc="best")

plt1=axs[1].errorbar( data.loc[data['tdep']==False,'Bout'], data.loc[data['tdep']==False,'Bins'], yerr=data.loc[data['tdep']==False,'Bins_sdev'].values, marker='o', color=mcol[0], label="Not time dependent")
axs[1].errorbar( data.loc[data['tdep'],'Bout'], data.loc[data['tdep'],'Bins'], yerr=data.loc[data['tdep'],'Bins_sdev'].values, marker='o', color=plt1[0].get_color(), mfc='white', label="Time dependent")
axs[1].set_xlabel("$B_{out}$ (mT)")
axs[1].set_ylabel("$B_{in}$ (mT)")
axs[1].set_xlim((0,50))
axs[1].legend(loc="best")

plt.savefig("plots/eps/shielding_vdg2layer.eps")
plt.savefig("plots/png/shielding_vdg2layer.png")
plt.show()

