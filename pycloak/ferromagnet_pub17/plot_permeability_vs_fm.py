#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

from scipy.interpolate import interp1d
from scipy.interpolate import PchipInterpolator

# read file with results from permeability calculation
data = pd.read_csv("results/ferromagnet_sbu.csv")

# sort data by external field
data.sort_values( 'Bout', inplace=True )

# choose reference field
Bref = 40 # mT

# set figure parameters
fig, axs = plt.subplots(1,1,figsize=(6,5))
axs.tick_params(labelsize=12)

axs.set_title("Ferromagnet Permeability (at 40 mT)")
plt.xlabel("fM",fontsize=12)
plt.ylabel("$\mu_{r}$",fontsize=12)

a_mu = []
a_mu_sdev = []
a_fm = []

#for id_i in ["fm000a", "fm000b", "fm554a", "fm554b", "fm581a", "fm581b", "fm590a", "fm590b", "fm618a", "fm618b", "fm673a", "fm673b", "fm699a", "fm699b", "fm745a", "fm745b"]:
for id_i in ["fm000a", "fm000b", "fm554a", "fm554b", "fm581a", "fm581b", "fm590a", "fm590b", "fm618a", "fm618b", "fm673a", "fm673b", "fm699b", "fm745a", "fm745b" ]:


    #    f = interp1d(calibration[:,1], calibration[:,2])
    interpol = PchipInterpolator(data.loc[data['ID']==id_i,'Bout'].values, data.loc[data['ID']==id_i,'mu'].values, extrapolate = False)

    a_mu.append( interpol(Bref) )
    a_mu_sdev.append( data.loc[data['ID']==id_i,'mu_err'].values[-1] )
    a_fm.append( data.loc[data['ID']==id_i,'frac'].values[0] )

# do plot
print(a_fm)
axs.errorbar( a_fm, a_mu, a_mu_sdev, marker='o')

# save & show plot
plt.savefig("plots/permeability_vs_fm_sbu.png")
plt.show()
###
