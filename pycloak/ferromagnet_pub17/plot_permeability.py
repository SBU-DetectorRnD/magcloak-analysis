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

axs.set_title("Ferromagnet Permeability")
plt.xlabel("$B_{out}$ (mT)",fontsize=12)
plt.ylabel("$\mu_{r}$",fontsize=12)


# read file with results from permeability calculation
data = pd.read_csv("results/ferromagnet_sbu.csv")


### plot data: plot 1
id_i = "fm548"
axs.errorbar( data.loc[data['ID']==id_i,'Bout'], data.loc[data['ID']==id_i,'mu'], yerr=data.loc[data['ID']==id_i,'mu_err_pp'], color='b', marker='o', label=id_i)

# add legend
axs.legend(loc = 'upper right')

# save & show plot
plt.savefig("plots/permeability_sbu.png")
plt.show()
###


### plot data: plot 2
#...
###
