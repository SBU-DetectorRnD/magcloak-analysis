#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

import matplotlib.pyplot as plt

from scipy.interpolate import interp1d
from scipy.interpolate import PchipInterpolator

#from scipy import optimize
import scipy.optimize as optimization

# set plotting style
import mycolors
mcol = mycolors.pub17
plt.style.use("../style_pub17/cloak17_paper.mplstyle")


# define fitting function that provides error erstimates and Chi2/NDF
#def fit_leastsq(p0, datax, datay, function):
#
#    errfunc = lambda p, x, y: function(p,x) - y
#
#    pfit, pcov, infodict, errmsg, success = optimize.leastsq(errfunc, p0, args=(datax, datay), full_output=1, epsfcn=0.0001)
#
#    if (len(datay) > len(p0)) and pcov is not None:
#        s_sq = (errfunc(pfit, datax, datay)**2).sum()/(len(datay)-len(p0))
#        pcov = pcov * s_sq
#        print ("Fit Chi2/DOF: ", np.sqrt(s_sq))
#    else:
#        pcov = np.inf
#
#    error = [] 
#    for i in range(len(pfit)):
#        try:
#          error.append(np.absolute(pcov[i][i])**0.5)
#        except:
#          error.append( 0.00 )
#    pfit_leastsq = pfit
#    perr_leastsq = np.array(error) 
#    return pfit_leastsq, perr_leastsq 


# read file with results from permeability calculation
data = pd.read_csv("results/ferromagnet_sbu.csv")
print(data)

# sort data by external field
data.sort_values( 'Bout', inplace=True )

# choose reference field
Bref = 40 # mT

# set figure parameters
fig, axs = plt.subplots(1,1,figsize=(6,5))

#axs.set_title("Ferromagnet Permeability (at 40 mT)")
plt.xlabel("$f_M$")
plt.ylabel("$\mu_{r}$")

a_mu = []
a_mu_sdev = []
a_fm = []


for id_i in ["fm104_cryo",
             "fm199_cryo",
             "fm303_cryo",
             "fm409_cryo",
             "fm554_cryo",
             "fm590_cryo",
             "fm618_cryo",
             "fm651_cryo",
             "fm699_cryo",
             "fm745_cryo3"]:

    # get data subset for this id_i
    data_sub = data[data['ID']==id_i].copy()

    print(data_sub)

    # remove duplicate external field values- interpolation crashes otherwise
    data_sub.drop_duplicates('Bout', keep='last', inplace=True)
    data_sub.sort_values( 'Bout', inplace=True )
    
    interpol = PchipInterpolator(data_sub['Bout'].values, data_sub['mu'].values, extrapolate = False)

    a_mu_interpol = interpol(Bref)

    if not ( np.isnan(a_mu_interpol) ):
        print( "Add point at fm = " ,data.loc[data['ID']==id_i,'frac'].values[0] , " and mu = ", a_mu_interpol) 
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
axs.set_ylim((0.8,5))


# Do fitting
fitfunc = lambda p, x: p[0]/np.tan(p[1]*x+p[2]) + p[3] # Target function

def fitfunc (x, a, b, c, d): 
    return a/np.tan(b*x+c) + d # Target function
#errfunc = lambda p, x, y: fitfunc(p, x) - y # Distance to the target function

p0 = [0.5, -2, 2, 1] # Initial guess for the parameters
#p0 = [0.545, -1.78, 1.454, 0.93577] # Initial guess for the parameters / Fit results from Thomas
#p1, success = optimize.leastsq(errfunc, p0[:], args=(a_fm, a_mu))

#p1, perr = fit_leastsq(p0, a_fm, a_mu, fitfunc)

p1, pcov = optimization.curve_fit( fitfunc, a_fm, a_mu, p0, a_mu_sdev, absolute_sigma=True )

# calculate chisquare
chisquare = ( a_mu - fitfunc( a_fm, *p1 ) ) / a_mu_sdev
chisquare = chisquare**2
chisquare = chisquare.sum()
chisquare /= ( len(a_fm) - len(p1) )
print ("Chisq:", chisquare)

# Print fit results
print ( "*** Fit results: ***" )
print("\nFit paramters and parameter errors from lestsq method :")
print("pfit = ", p1)
print("Covariance matrix:")
print( pcov )

perrors = [] 
for i in range(len(p1)):
    try:
        perrors.append(np.absolute(pcov[i][i])**0.5)
    except:
          perrors.append( 0.00 )
    pfit_curvefit = p1
    perr_curvefit = np.array(perrors)

print("pfit = ", pfit_curvefit)
print("perr = ", perr_curvefit)


# plot data and curve

time = np.linspace(0, 1, 100)
plt.plot(time, fitfunc(time, *p1), "r-") # Plot of the data and the fit

# save & show plot
plt.savefig("plots/eps/permeability_vs_fm_sbu.eps")
plt.savefig("plots/png/permeability_vs_fm_sbu.png")
plt.show()
###
