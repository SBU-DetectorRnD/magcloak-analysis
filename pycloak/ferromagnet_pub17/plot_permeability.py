#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt


# read file with results from permeability calculation
data_sbu = pd.read_csv("results/ferromagnet_sbu.csv")
data_anl = pd.read_csv("results/ferromagnet_mri.csv")

# sort data by external field
data_sbu.sort_values( 'Bout', inplace=True )
data_anl.sort_values( 'Bout', inplace=True )

### plot data: plot 1

# set figure parameters
fig, axs = plt.subplots(1,1,figsize=(6,5))
axs.tick_params(labelsize=12)

axs.set_title("Ferromagnet Permeability")
plt.xlabel("$B_{out}$ (mT)",fontsize=12)
plt.ylabel("$\mu_{r}$",fontsize=12)

# do plot
#for id_i in ["fm590a", "fm590b", "fm554a", "fm554b", "fm574a", "fm574b", "fm574c", "fm574d", "fm581a", "fm581b", "fm618a", "fm618b", "fm673a", "fm673b"]:
#for id_i in ["fm574o", "fm574a", "fm574b", "fm574c", "fm574d", "fm574e"]:
for id_i in ["fm590a", "fm554a", "fm602"]:
    axs.errorbar( data_sbu.loc[data_sbu['ID']==id_i,'Bout'], data_sbu.loc[data_sbu['ID']==id_i,'mu'], yerr=data_sbu.loc[data_sbu['ID']==id_i,'mu_err_pp'], marker='o', label=id_i)

# add legend
axs.legend(loc = 'upper right')

# save & show plot
plt.savefig("plots/permeability_sbu.png")
plt.show()
###


### plot data: plot 2

# set figure parameters
fig, axs = plt.subplots(1,1,figsize=(6,5))
axs.tick_params(labelsize=12)

axs.set_title("Ferromagnet Permeability")
plt.xlabel("$B_{out}$ (mT)",fontsize=12)
plt.ylabel("$\mu_{r}$",fontsize=12)

# do plot
Bout_min = 1
for id_i in [ "fm618" , "fm554" , "fm699" , "fm673" ]:
    select_rows = (data_anl['ID']==id_i) & (data_anl['Bout'] > Bout_min)
    axs.errorbar( data_anl.loc[ select_rows,'Bout' ], data_anl.loc[ select_rows, 'mu' ], yerr=data_anl.loc[ select_rows, 'mu_err_pp' ], marker='o', label=id_i)

# add legend
axs.legend(loc = 'upper right')

# save & show plot
plt.savefig("plots/permeability_anl.png")
plt.show()
###



### plot data: plot 3

# set figure parameters
fig, axs = plt.subplots(1,1,figsize=(6,5))
axs.tick_params(labelsize=12)

axs.set_title("Ferromagnet Permeability")
plt.xlabel("$B_{out}$ (mT)",fontsize=12)
plt.ylabel("$\mu_{r}$",fontsize=12)

# do plot
Bout_min = 1
# do plot
for id_i in ["fm554a", "fm554b"]:
    axs.errorbar( data_sbu.loc[data_sbu['ID']==id_i,'Bout'], data_sbu.loc[data_sbu['ID']==id_i,'mu'], yerr=data_sbu.loc[data_sbu['ID']==id_i,'mu_err_pp'], marker='o', label=id_i)

for id_i in [ "fm554" ]:
    select_rows = (data_anl['ID']==id_i) & (data_anl['Bout'] > Bout_min)
    axs.errorbar( data_anl.loc[ select_rows,'Bout' ], data_anl.loc[ select_rows, 'mu' ], yerr=data_anl.loc[ select_rows, 'mu_err_pp' ], marker='o', label=id_i, color='r')

# add legend
axs.legend(loc = 'upper right')

# save & show plot
plt.savefig("plots/permeability_anl_sbu.png")
plt.show()
###
