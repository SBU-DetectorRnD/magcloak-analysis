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

# read file with results from permeability calculation
data_sbu = pd.read_csv("results/ferromagnet_sbu.csv")
data_anl = pd.read_csv("results/ferromagnet_mri.csv")

# sort data by external field
data_sbu.sort_values( 'Bout', inplace=True )
data_anl.sort_values( 'Bout', inplace=True )

### plot data: plot 1

# set figure parameters
fig, axs = plt.subplots(1,1,figsize=(6,5))

axs.set_title("Ferromagnet Permeability")
plt.xlabel("$B_{out}$ (mT)")
plt.ylabel("$\mu_{r}$")

# do plot
#for id_i in ["fm590a", "fm590b", "fm554a", "fm554b", "fm574a", "fm574b", "fm574c", "fm574d", "fm581a", "fm581b", "fm618a", "fm618b", "fm673a", "fm673b"]:
#for id_i in ["fm574o", "fm574a", "fm574b", "fm574c", "fm574d", "fm574e"]:
#for id_i in ["fm590a", "fm554a", "fm602"]:
for id_i in ["fm554a", "fm554b"]:
    axs.errorbar( data_sbu.loc[data_sbu['ID']==id_i,'Bout'], data_sbu.loc[data_sbu['ID']==id_i,'mu'], yerr=data_sbu.loc[data_sbu['ID']==id_i,'mu_err_pp'], marker='.', label=id_i)

# add legend
axs.legend(loc = 'upper right')

# save & show plot
plt.savefig("plots/eps/permeability_sbu.eps")
plt.savefig("plots/png/permeability_sbu.png")
#plt.show()
###


### plot data: plot 2

# set figure parameters
fig, axs = plt.subplots(1,1,figsize=(6,5))

plt.xlabel("$B_{out}$ (mT)")
plt.ylabel("$\mu_{r}$")

# do plot
Bout_min = 5
#for id_i in [ "fm618" , "fm554" , "fm699" , "fm673" ]:
for i,id_i in enumerate([ "fm699" , "fm618" ]):
    label_i = "$f_M$ = 0."
    label_i += id_i.replace('fm','')
    select_rows = (data_anl['ID']==id_i) & (data_anl['Bout'] > Bout_min)
    axs.errorbar( data_anl.loc[ select_rows,'Bout' ], data_anl.loc[ select_rows, 'mu' ], yerr=data_anl.loc[ select_rows, 'mu_err_pp' ], marker='.', color=mcol[i], label=label_i)

# add legend
axs.legend(loc = 'upper right')
#axs.set_xlim((5,500))

# save & show plot
plt.savefig("plots/eps/permeability_anl.eps")
plt.savefig("plots/png/permeability_anl.png")
plt.show()
###



### plot data: plot 3

# set figure parameters
fig, axs = plt.subplots(1,1,figsize=(6,5))

axs.set_title("Ferromagnet Permeability")
plt.xlabel("$B_{out}$ (mT)")
plt.ylabel("$\mu_{r}$")

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
plt.savefig("plots/eps/permeability_anl_sbu.eps")
plt.savefig("plots/png/permeability_anl_sbu.png")
#plt.show()
###
