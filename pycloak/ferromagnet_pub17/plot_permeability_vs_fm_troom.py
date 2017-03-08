#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

from scipy.interpolate import interp1d
from scipy.interpolate import PchipInterpolator

from scipy import optimize

# set plotting style
import mycolors
mcol = mycolors.pub17
plt.style.use("../style_pub17/cloak17_paper.mplstyle")

# read file with results from permeability calculation
data = pd.read_csv("results/ferromagnet_sbu_troom.csv")

# sort data by external field
data.sort_values( 'Bout', inplace=True )

# choose reference field
Bref = 40 # mT

# set figure parameters
fig, axs = plt.subplots(1,1,figsize=(6,5))

#axs.set_title("Ferromagnet Permeability (at 40 mT)")
plt.xlabel("$f_M$")
plt.ylabel("$\mu_{r}$")

a_mu = [1]
a_mu_sdev = [0]
a_fm = [0]


for id_i in ["fm104", "fm199", "fm503", "fm548", "fm554", "fm574", "fm602", "fm612", "fm625", "fm651", "fm673"]:

    # get data subset for this id_i
    data_sub = data[data['ID']==id_i].copy()

    # remove duplicate external field values- interpolation crashes otherwise
    data_sub.drop_duplicates('Bout', keep='last', inplace=True)
    data_sub.sort_values( 'Bout', inplace=True )

    #    f = interp1d(calibration[:,1], calibration[:,2])
    #interpol = PchipInterpolator(data.loc[data['ID']==id_i,'Bout'].values, data.loc[data['ID']==id_i,'mu'].values, extrapolate = False)
    interpol = PchipInterpolator(data_sub['Bout'].values, data_sub['mu'].values, extrapolate = False)

    a_mu_interpol = interpol(Bref)

    if not ( np.isnan(a_mu_interpol) ):
        a_mu.append( a_mu_interpol )
        a_mu_sdev.append( data.loc[data['ID']==id_i,'mu_err'].values[-1] )
        a_fm.append( data.loc[data['ID']==id_i,'frac'].values[0] )

# convert lists to numpy arrays
a_mu = np.array(a_mu)
a_mu_sdev = np.array(a_mu_sdev)
a_fm = np.array(a_fm)

# do plot
print(a_fm)
axs.errorbar( a_fm, a_mu, yerr=a_mu_sdev, linestyle='', marker='.', color=mcol[0])
axs.set_xlim((0,0.8))
axs.set_ylim((0.8,4))


# Do fitting
fitfunc = lambda p, x: p[0]/np.tan(p[1]*x+p[2]) + p[3] # Target function
errfunc = lambda p, x, y: fitfunc(p, x) - y # Distance to the target function

p0 = [1, 1, 1, 1] # Initial guess for the parameters
#p0 = [0.545, -1.78, 1.454, 0.93577] # Initial guess for the parameters / Fit results from Thomas
p1, success = optimize.leastsq(errfunc, p0[:], args=(a_fm, a_mu))
#p1=p0

time = np.linspace(0, 1, 100)
plt.plot(time, fitfunc(p1, time), "r-") # Plot of the data and the fit

# save & show plot
plt.savefig("plots/eps/permeability_vs_fm_sbu_troom.eps")
plt.savefig("plots/png/permeability_vs_fm_sbu_troom.png")
plt.show()
###
