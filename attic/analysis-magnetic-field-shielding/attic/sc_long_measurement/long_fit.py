import numpy as np
from uncertainties import unumpy
from uncertainties import ufloat
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import time
import os
from scipy import stats
import sys

extrapolate_times = np.array([86400, 3.156e+7])
def func_log(x, a, b, c):
    penalization = 0
    if (b<0) or (a<0) or c < 0:
        penalization = 10000000000
    return a*np.log((x + 1 + b)) +c + penalization


#def func_vortex(x, a, b, c, d, e):
#    penalization = 0
#    if b<0 or b>2 or d<0 or e<0:
#        penalization = 100000000000
#    return a/((1 + b*c*np.log(x/d+e+1))**(1/b)) + penalization
def func_vortex(x, a, b, c, d):
    penalization = 0
 #   if d < 0 or c <0:
 #       penalization = 100000000
    return (a + b*np.log(x+c+1))**(-1/d)


#have initial parameter fit guesses for each function
def initial_guess(func):
    if func == func_log:
        return (2,3,2)
    elif func == func_2log:
        return (2,3,100,5)
    elif func ==func_stretched_exp:
        return (-5, 10, u, 7)
    elif func ==func_stretched_exp_assume:
        return (-5, 10, 7)
    elif func ==func_double_log:
        return (-5, 10, 7 )
    elif func == func_2stretched_exp:
        return (-5,10,u,-5,10,u,7)
#    elif func == func_vortex:
#        return (5,u,5,10, 10)
    elif func == func_vortex:
        return (5, 0, 10, u)
    else:
        sys.exit("Error")

#evaluate extrapolation with associated errors
def func_err_prop(func, x,p):
    if func == func_log:
        return p[0]*unumpy.log((extrapolate_times + 1 + p[1])) + p[2]
    if func == func_2log:
        return p[0]*unumpy.log(extrapolate_times + 1 + p[1]) +\
            p[2]*unumpy.log(extrapolate_times + 1 +p[1]) + p[3]
    if func == func_stretched_exp:
        return p[0]*unumpy.exp(-(extrapolate_times/p[1])**p[2])+p[3] 
    if func == func_stretched_exp_assume:
        return p[0]*unumpy.exp(-(extrapolate_times/p[1])**u)+p[2] 
    if func == func_double_log:
        return p[0]*unumpy.log(1+unumpy.log(x+1+p[1]))+p[2] 
    if func == func_2stretched_exp:
        return p[0]*unumpy.exp(-(x/p[1])**p[2])+p[3]*unumpy.exp(-(x/p[4])**p[5]) + p[6] 
#    if func == func_vortex:
#        return p[0]/((1 + p[1]*p[2]*unumpy.log(x/p[3]+p[4]+1))**(1/p[2])) 
    if func == func_vortex:
        return (p[0] + p[1]*unumpy.log(x+p[2]+1))**(-1/p[3])
    else:
        sys.exit("Error")

#get reduced chi square and p value
def calculate_stats(x, y, yerr, p,perr, func):
    yfit = func(x, *p)
    resid = y-yfit
    dof = y.size-p.size
    chi2 = np.sum((resid/yerr)**2)
    chi2red = chi2/dof
    pval = 1 - stats.chi2.cdf(chi2,dof)
    return(chi2red, pval)

#fit data to function
def fit_func(func, x, y, yerr, xlong,ylong):
    start = time.time()
    p, cov = curve_fit(func, x, y, p0 = initial_guess(func), sigma = yerr,
            maxfev = 1000000)
    print(p)
    #print(p)
    #print(cov)
    perr = np.sqrt(np.diag(cov))

    chi2red, pval = calculate_stats(x, y, yerr, p, perr, func)

    yfit = func(xlong,*p)
    resid = ylong-yfit
    p = unumpy.uarray(p,perr)
    extrapolate_val = func_err_prop(func,extrapolate_times,p)

    end = time.time()
    elapsed = end-start

    return (p, yfit, resid, extrapolate_val, chi2red, pval, elapsed) 
