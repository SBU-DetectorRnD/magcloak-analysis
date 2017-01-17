import numpy as np
from uncertainties import unumpy
from uncertainties import ufloat
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from mpl_toolkits.axes_grid1 import make_axes_locatable

import quality_check

import sys

change_tolerance = 0.01

sigma_param = 15.07
n = 1/2

def func_log(x, a, b, c):
    penalization = 0
    return a*np.log((x/b+ 1 )) + c + penalization

def func_power(x,a,b,c):
    penalization = 0
    return (a*(x+c)**b + penalization)

def func_power2(x,a,b,c,d):
    penalization = 0
    return (a*(x+c)**b +d + penalization)


def time_dependent(x,y):
    cut = x.size/3
    mean_first = np.mean(y[0:cut])
    std_first = np.std(y[0:cut])
    mean_last = np.mean(y[-1-cut:-1])
    std_last = np.std(y[-1-cut:-1])
    return( abs(mean_first-mean_last) > (std_first + std_last))

def initial_guess(func,x,y, b0):
    if func == func_log:
        return (b0.n/((1-n)*sigma_param), 10, 2000)
    elif func == func_power:
        x = np.log(x)
        if np.all(y<0):
            y = np.log(-y)
            p = np.polyfit(x,y,1)
            b = p[0]
            a = -np.exp(p[1])
            
        elif np.all(y>0):
            y = np.log(y)
            p = np.polyfit(x,y,1)
            b = p[0]
            a = np.exp(p[1])
        else:
            print('******Over Here******')
            return(0.03, 0.23, 11.2)
        return (a,b,0)        
    elif func == func_power2:
        offset = 0
        if np.any(y<0):
            offset = -0.001 + np.min(y)
        x = np.log(x+0.001)
        y = np.log(y-offset)
        p = np.polyfit(x,y,1)
        b = p[0]
        a = np.exp(p[1])
        return (a,b,0,0)
    else:
        sys.exit("Error: No intial guess.")

def func_var(x, f, p, cov):
    if f == func_log:
        g = np.array([np.log(x/p[1] + 1), p[0]/(x+p[1]), 1])
    elif f == func_power:
        g = np.array([(x+p[2])**p[1], p[0]*(x+p[2])**p[1]*np.log(x + p[2]),
            p[0]*p[1]*(x+p[2])**(p[1]-1)])
    elif f == func_power2:
        g = np.array([(x+p[2])**p[1], p[0]*(x+p[2])**p[1]*np.log(x + p[2]),
            p[0]*p[1]*(x+p[2])**(p[1]-1), 1])
    else:
        sys.exit("Error: No proper variance formula")
    var = np.dot(np.dot(np.transpose(g),cov),g)
    return var


#get reduced chi square and p value
def fit_flat(x, y, yerr):
    p, cov = np.polyfit(x, y, 0, w=1/yerr, cov=True)
    print(np.std(y))
    perr = np.sqrt(np.diag(cov))
    extrapolate_val = ufloat(p[0], perr[0])
    yfit = np.polyval(p,x)
    resid = y-yfit
    chi2red = np.sum((resid/yerr)**2)/(y.size- len(p))
    return (extrapolate_val )

def fit_func(func, x, y, yerr, extrapolate_time, b0):
    p, cov = curve_fit(func, x, y, p0 = initial_guess(func,x,y,b0), sigma = yerr,
            maxfev = 1000000)
    if func == func_power:
        pi = p
#        print(pi)
        p,cov = curve_fit(func, x, y, p0 = initial_guess(func,x+pi[2], y,b0), sigma
                = yerr, maxfev = 100000)
        while (np.any((pi/p-1) > 0.01)):
            pi = p
            #print((pi/p-1)>0.01)
            p,cov = curve_fit(func, x, y, p0 = initial_guess(func,x+pi[2], y,b0), sigma
                    = yerr, maxfev = 100000)
    if func == func_power2:
        pi = p
        p, cov = curve_fit(func, x, y, p0 = initial_guess(func, x + p[2],
            y-p[3],b0),
                sigma = yerr, maxfev = 1000000)
        while (np.any((pi/p-1)>0.001)):
            pi=p
#            print(p)
            p, cov = curve_fit(func, x, y, p0 = initial_guess(func, x+p[2],
                y-p[3],b0),
                    sigma = yerr, maxfev = 1000000)            

    extrapolate_val_s = np.sqrt(func_var(extrapolate_time, func,p, cov))
    extrapolate_val_n = func(extrapolate_time, *p)
    extrapolate_val = ufloat(extrapolate_val_n, extrapolate_val_s)

    return (extrapolate_val) 

###### main part
def analyze(x, y, yerr, extrapolate_time, isGraph, b0, b_before_cool, title, description, 
         results_file, fit_graphs_folder,param_fit_file):

    tdependent = 0
    overshoot = 0
    #if time_dependent(y,yerr):
    if time_dependent(x,y):
        print('time dependent')

        tdependent = 1
        extrapolate_val_log = fit_func(func_log, x, y, yerr, extrapolate_time,
                b0)
        if not quality_check.extrapolation_quality(extrapolate_val_log, b0):
            return
        if np.all(y > 0):
            extrapolate_val_pow = fit_func(func_power2, x, y, yerr,
                    extrapolate_time,b0)

            if not quality_check.extrapolation_quality(extrapolate_val_pow, b0):
                return

#            sigma_extrapolate = np.abs(extrapolate_val_log.n-extrapolate_val_pow.n)/2 + \
#                    np.sqrt(extrapolate_val_log.s**2 + extrapolate_val_pow.s**2)/2
    
            mean_extrapolate = (extrapolate_val_log.n + extrapolate_val_pow.n)/2
        
            if (extrapolate_val_log.n > extrapolate_val_pow.n):
                overshoot = 1
                sigma_extrapolate = (extrapolate_val_log.n+ extrapolate_val_log.s)\
                        - (extrapolate_val_pow.n - extrapolate_val_pow.s)
            else:
                overshoot = 2
                sigma_extrapolate = (extrapolate_val_pow.n+extrapolate_val_pow.s)\
                        -(extrapolate_val_log.n - extrapolate_val_log.s)
            extrapolate_val = ufloat(mean_extrapolate, sigma_extrapolate)
            print("Bin:\t" + str(extrapolate_val))
        else:
            extrapolate_val = extrapolate_val_log
            overshoot = 3

            
        if not quality_check.extrapolation_quality(extrapolate_val, b0):
            return

    #elif not time_dependent(y, yerr):
    elif not time_dependent(x,y):
        tdependent = 0
        #extrapolate_val = fit_flat(x,y,yerr)
        extrapolate_val = ufloat(np.mean(y), np.std(y)) 

        print('Not time dependent')
        print("Bin:\t" + str(extrapolate_val))

    bshield = b0 - extrapolate_val
    shielding_factor = bshield/b0

    with open(results_file, "a") as myfile:
        myfile.write('%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' %(b0.n, b0.s, extrapolate_val.n, 
            extrapolate_val.s,
            bshield.n, bshield.s, shielding_factor.n, shielding_factor.s,
            tdependent, overshoot))




        

