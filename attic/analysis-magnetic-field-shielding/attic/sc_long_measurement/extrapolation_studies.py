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

ifile = 'DataFile_150222_041457.txt'
#ifile = 'DataFile_140708_092917.txt'
t_cuts = [90, 300, 600, 1800, 3600, 14400, 28800, 86400]

o_data_graph_linx = ifile[:-4]+'_linx.png'
o_data_graph_logx = ifile[:-4]+'_logx.png'
o_current_graph = ifile[:-4]+'_current_.png'

extrapolate_times = np.array([86400, 3.156e+7])
#ofile = ['results_log_24h.txt', 'results_2log_24h.txt','results_exp_24h.txt','results_exp_assume_24h.txt']
#ofile = ['results_log_24h.txt', 'results_exp_24h.txt','results_exp_assume_24h.txt']
ofile=['results_dlog_24h.txt']

ograph_linx = [s[:-4]+'_linx.png' for s in ofile]
ograph_logx = [s[:-4]+'_logx.png' for s in ofile]

bo = 34.3
field_str = 'Bo = 34.3 mT' 
run_time_str = '24 hr'
#fits_str = ['Log Fit', 'Stretched Exponential Fit', 'Constrained Stretched Exponential Fit']
fits_str = ['Double Log']
title = [fit + ', ' + run_time_str + ', ' + field_str for fit in fits_str]  

for file in ofile:
    if os.path.isfile(file):
        os.remove(file)
    with open(file,"a") as myfile:
        myfile.write('#time_cut\t1day_extrapolation\tuncertainty\
                \t1year_extrapolation\tuncertainty\tchi2red\
                \tprocessing_time\tpval\n')

legend = ['90 seconds', '5 minutes', '10 minutes', '30 minutes', '1 hour',\
                '4 hours', '8 hours',  '24 hours']

marker_color =[ 'g', 'r', 'c', 'm', 'y', 'k', 'g', 'r' ]
marker_style = ['None', 'None', 'None', 'None','None', 'None', '*', '*']
lw = 2

u = .18 
#list of posible functions

def func_log(x, a, b, c,d):
    penalization = 0
    if (b<0) or (a<0):
        penalization = 10000000000
    return a*np.log((x +b)/d+ 1) +c + penalization

def func_2log(x, a, b, c, d):
    penalization = 0
    if (b<0) or (a<0) or (c<0):
        penalization = 10000000000
    return a*np.log(x + 1 + b) +c*np.log(x + 1 + b) + d + penalization

def func_stretched_exp(x,a,b,c,d):
    penalization = 0
    if (b < 0) or (c < 0) or (a>0) or (c>2):
        penalization = 10000000000000
    return a*np.exp(-(x/b)**c)+d + penalization

def func_stretched_exp_assume(x,a,b,c):
    penalization = 0
    if (b<0) or (a>0):
        penalization = 1000000000000
    return a*np.exp(-(x/b)**u)+c + penalization

def func_2stretched_exp(x,a,b,c,d,e,f,g):
    penalization = 0
    if (b<0) or (e<0) or (c<0) or (f<0) or a>0 or d>0 or c>2 or f>2:
        penalization = 1000000000
    return a*np.exp(-(x/b)**c)+d*np.exp(-(x/e)**f) + g + penalization

def func_double_log(x, a, b, c ):
    penalization = 0
    if  b<0:
        penalization = 1000000000000000
    return a*np.log(1+np.log(x+1+b))+c + penalization

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
        return p[0]*unumpy.log(extrapolate_times + 1 + p[1]) + p[2]
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

#functions = [func_log, func_2log, func_stretched_exp_assume]
#functions = [func_log, func_2log, func_stretched_exp, func_stretched_exp_assume]
#functions = [func_log, func_stretched_exp, func_stretched_exp_assume]
#functions = [func_log]
functions = [func_double_log]

M = np.genfromtxt(ifile)
t = M[:,0]
current = M[:,1]

b = M[:,2]
sig_b = 0.001*np.ones(np.size(b))


plt.plot(t,current,marker='.')
plt.xlabel('time [s]')
plt.ylabel('Current [A] [mT]')
plt.semilogx()
plt.savefig(o_current_graph)
plt.close()

plt.plot(t,b,'.', label = 'data')
plt.xlabel('time [s]')
plt.ylabel('$B_{leak}$ [mT]')
plt.legend(loc = 'best')
plt.savefig(o_data_graph_linx)
plt.semilogx()
plt.savefig(o_data_graph_logx)
plt.close()

#just save plotted data
for ind_func,func in enumerate(functions):

    for j, cut in enumerate(t_cuts):
        x = t[0:cut]
        y = b[0:cut]
        sig_y = sig_b[0:cut]

        print(cut)
        [p, yfit, resid, extrapolate_val, chi2red, pval, elapsed] = fit_func(func, x, y,
                sig_y, t,b)
        
        plt.plot(t, yfit, marker = marker_style[j], color =
                marker_color[j], label = legend[j], linewidth = lw)

        with open(ofile[ind_func],"a") as myfile:
            myfile.write('%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s' %(cut,
                extrapolate_val[0].nominal_value,
                extrapolate_val[0].std_dev,
                extrapolate_val[1].nominal_value,
                extrapolate_val[1].std_dev, 
                chi2red, elapsed, pval))
            for k in p:
                myfile.write('\t%s\t%s' %(k.nominal_value, k.std_dev))
            myfile.write('\n')

    plt.title(title[ind_func])
    plt.plot(t, b, '--', label = 'data', linewidth = lw)
    plt.xlabel('time [s]')
    plt.ylabel('$B_{leak}$ [mT]')
    plt.legend(loc = 'best')
    plt.savefig(ograph_linx[ind_func])
    plt.semilogx()
    plt.savefig(ograph_logx[ind_func])
    plt.close()

    

        





