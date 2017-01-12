import numpy as np
from uncertainties import unumpy
from uncertainties import ufloat
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import time
import os
from scipy import stats

ifile = 'DataFile_140708_092917.txt'
t_cuts = [90, 300, 600, 1800, 3600, 7200]
#t_cuts = [300, 600, 1800, 3600, 7200]
#t_cuts = [ 1800, 3600, 7200]

extrapolate_times = np.array([86400, 3.156e+7])
ofile_log='results_log.txt'
ofile_exp = 'results_exp.txt'
#ofile_log = ['results_log_90.txt','results_log_300.txt',\
#    'results_log_600.txt','results_log_1800.txt',\
#    'results_log_3600.txt', 'results_log_7200.txt', 'results_log_14400.txt']

if os.path.isfile(ofile_log):
    os.remove(ofile_log)
if os.path.isfile(ofile_exp):
    os.remove(ofile_exp)

if os.path.isfile('test.txt'):
    os.remove('test.txt')


legend_log = ['90 seconds', '5 minutes', '10 minutes', '30 minutes', '1 hour',\
                '2 hours']

#ofile_stretched_exp = ['results_stretched_exp_90.txt','results_stretched_exp_300.txt',\
#    'results_stretched_exp_3600.txt', 'results_stretched_exp_14400.txt']

def func_log(x, a, b, c):
    return a*np.log(x + 1 + b) +c

def func_2log(x, a, b, c, d, e):
    return a*np.log(x + 1 + b) +c*np.log(x + 1 + d) + e

def func_stretched_exp(x,a,b,c,d):
    return a*np.exp(-(x/b)**c)+d

def func_2stretched_exp(x,a,b,c,d,e,f,g):
    return a*np.exp(-(x/b)**c)+d*np.exp(-(x/e)**f) + g 

def fit_log(x, y, yerr, xlong):
    start = time.time()
    p, cov = curve_fit(func_log, x, y, sigma = yerr, maxfev = 100000)
    perr = np.sqrt(np.diag(cov))    

    yfit = func_log(x, p[0], p[1], p[2])
    resid = y-yfit

    chi2 = np.sum((resid/yerr)**2)
    dof = y.size-len(p)
    chi2red = chi2/dof
    p_val = 1 - stats.chi2.cdf(chi2,dof)
    yfit = func_log(xlong, p[0], p[1], p[2])
    p = unumpy.uarray(p,perr)
    extrapolate_val = p[0]*unumpy.log(extrapolate_times + 1 + p[1]) + p[2]
    end = time.time()
    elapsed = end-start
    return (chi2red, resid, extrapolate_val, 1, unumpy.nominal_values(p),
            unumpy.std_devs(p), yfit, elapsed, p_val)

#def fit_2log(x, y, yerr, xlong):
#    start = time.time()
#    p, cov = curve_fit(func_2log, x, y, sigma = yerr, maxfev = 100000)
#    print(cov)
#    perr = np.sqrt(np.diag(cov))    
#
#    yfit = func_2log(x, p[0], p[1], p[2], p[3], p[4])
#    resid = y-yfit
#
#    chi2 = np.sum((resid/yerr)**2)
#    dof = y.size-len(p)
#    chi2red = chi2/dof
#    p_val = 1 - stats.chi2.cdf(chi2,dof)
#    print(chi2,dof)
#    print(p_val)
#    yfit = func_2log(xlong, p[0], p[1], p[2], p[3], p[4])
#    p = unumpy.uarray(p,perr)
#    extrapolate_val = p[0]*unumpy.log(extrapolate_times + 1 + p[1]) + \
#        p[2]*unumpy.log(extrapolate_times +1 + p[3]) + p[4]
#    end = time.time()
#    elapsed = end-start
#    return (chi2red, resid, extrapolate_val, 1, unumpy.nominal_values(p),
#            unumpy.std_devs(p), yfit, elapsed, p_val)

def fit_stretched_exp(x, y, yerr, xlong):
    start = time.time()
    p0 = (-1.5, 100, 0.15, 2)
    p, cov = curve_fit(func_stretched_exp, x, y, p0=p0, sigma = yerr, maxfev = 100000)
    print(p)
    perr = np.sqrt(np.diag(cov))    

    yfit = func_stretched_exp(x, p[0], p[1], p[2], p[3])
    resid = y-yfit
    dof = y.size - len(p)
    chi2 = np.sum((resid/yerr)**2)
    chi2red = chi2/dof
    p_val = 1 - stats.chi2.cdf(chi2,dof)
    yfit = func_stretched_exp(xlong, p[0], p[1], p[2], p[3])
    p = unumpy.uarray(p,perr)
    extrapolate_val = p[0]*unumpy.exp(-(extrapolate_times/p[1])**p[2])+p[3] 

    if p[2]>0:
        extrap_inf = p[3]
    elif p[2]<0:
        extrap_inf = p[0]+p[3]

    end = time.time()
    elapsed = end-start
    return (chi2red, resid, extrapolate_val, extrap_inf, 1, unumpy.nominal_values(p),
            unumpy.std_devs(p), yfit, elapsed, p_val)
#def fit_2stretched_exp(x, y, yerr, xlong):
#    start = time.time()
#    p, cov = curve_fit(func_2stretched_exp, x, y, sigma = yerr, maxfev = 100000)
#    perr = np.sqrt(np.diag(cov))    
#
#    yfit = func_2stretched_exp(x, p[0], p[1], p[2], p[3], p[4], p[5], p[6])
#    resid = y-yfit
#    dof = y.size - len(p)
#    chi2 = np.sum((resid/yerr)**2)
#    chi2red = chi2/dof
#    p_val = 1 - stats.chi2.cdf(chi2,dof)
#    print(chi2, dof)
#    print(p_val)
#    print(chi2red)
#    yfit = func_2stretched_exp(xlong, p[0], p[1], p[2], p[3], p[4], p[5], p[6])
#    p = unumpy.uarray(p,perr)
#    extrapolate_val = p[0]*unumpy.exp(-(extrapolate_times/p[1])**p[2]) + \
#    p[3]*unumpy.exp(-(extrapolate_times/p[4])**p[5]) + p[6] 
#
#    if p[2]>0:
#        extrap_inf = p[3]
#    elif p[2]<0:
#        extrap_inf = p[0]+p[3]
#
#    end = time.time()
#    elapsed = end-start
#    return (chi2red, resid, extrapolate_val, extrap_inf, 1, unumpy.nominal_values(p),
#            unumpy.std_devs(p), yfit, elapsed, p_val)
#
M = np.genfromtxt(ifile)
t = M[:t_cuts[-1],0]
b = M[:t_cuts[-1],2]
sig_b = 0.001*np.ones(np.size(b))

with open(ofile_log,"a") as myfile:
    myfile.write('#time cut\t1day_extrapolation\t uncertainty\t1year_extrapolation\
    \t uncertainty\tchi2red\tprocessing time\tp-val\n')
plt.plot(t,b,'.', label = 'data' )
plt.xlabel('time [s]')
plt.ylabel('$B_{leak}$ [mT]')
plt.title('1 Layer Sheath, $B_0 = 28.5 mT$')
plt.semilogx()
plt.savefig('data.png')
plt.close()

plt.plot(t,b,'.', label = 'data' )
for ind, cut in enumerate(t_cuts):
    x = t[0:cut]
    y = b[0:cut]
    sig_y = sig_b[0:cut]

    [chi2red, resid, extrapolated_vals, fit_type, p, perr, yfit, elapsed, p_val] = fit_log(x,
                y, sig_y, t)

    plt.plot(t,yfit, '--', label = legend_log[ind], linewidth = 2)
    with open(ofile_log,"a") as myfile:
        myfile.write('%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' %(cut,
                extrapolated_vals[0].nominal_value,
                extrapolated_vals[0].std_dev,
                extrapolated_vals[1].nominal_value,
                extrapolated_vals[1].std_dev, chi2red, elapsed, p_val))

plt.legend(loc = 'best')
plt.title('Log Fit for 2 Hour Run')
plt.xlabel('time [s]')
plt.ylabel('$B_{leak}$ [mT]')
plt.savefig('log_fit_2hrs_linx.png')
plt.semilogx()
plt.savefig('log_fit_2hrs_logx.png')
plt.close()

plt.plot(t,b,'.', label = 'data')
with open(ofile_exp,"a") as myfile:
    myfile.write('#time cut\t1day_extrapolation\t uncertainty\t1year_extrapolation\
    \t uncertainty\tinfinity extrapolation\
    \tuncertainty\tchi2red\tprocessing time\tp-val\n')
for ind, cut in enumerate(t_cuts):
    x = t[0:cut]
    y = b[0:cut]
    sig_y = sig_b[0:cut]

    [chi2red, resid, extrapolated_vals, extrap_inf, fit_type, p, perr, yfit,
            elapsed, p_val] = fit_stretched_exp(x, y, sig_y,t )

    plt.plot(t,yfit, '--', label = legend_log[ind], linewidth = 2)


    with open(ofile_exp,"a") as myfile:
        myfile.write('%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' %(cut,
                extrapolated_vals[0].nominal_value,
                extrapolated_vals[0].std_dev,
                extrapolated_vals[1].nominal_value,
                extrapolated_vals[1].std_dev, 
                extrap_inf.nominal_value,extrap_inf.std_dev, 
                chi2red, elapsed, p_val))

plt.legend(loc = 'best')
plt.title('Stretched Exponential Fit for 2 Hour Run')
plt.xlabel('time [s]')
plt.ylabel('$B_{leak}$ [mT]')
plt.savefig('exp_fit_2hrs_linx.png')
plt.semilogx()
plt.savefig('exp_fit_2hrs_logx.png')
plt.close()

#with open('test.txt',"a") as myfile:
#    myfile.write('#time cut\t1day_extrapolation\t uncertainty\t1year_extrapolation\
#    \t uncertainty\tchi2red\tprocessing time\tp-val\n')
#plt.plot(t,b,'.', label = 'data')
#with open(ofile_exp,"a") as myfile:
#    myfile.write('#time cut\t1day_extrapolation\t uncertainty\t1year_extrapolation\
#    \t uncertainty\tinfinity extrapolation\
#    \tuncertainty\tchi2red\tprocessing time\tp-val\n')
#for ind, cut in enumerate(t_cuts):
#    print(cut)
#    x = t[0:cut]
#    y = b[0:cut]
#    sig_y = sig_b[0:cut]
#
#    [chi2red, resid, extrapolated_vals, fit_type, p, perr, yfit,
#            elapsed, p_val] = fit_2log(x, y, sig_y,t )
#
#    plt.plot(t,yfit, '--', label = legend_log[ind], linewidth = 1)
##    with open(ofile_log[ind],"a") as myfile:
##        myfile.write('%s\t%s\n%s\t%s\n%s\t%s\n%s\t%s\n%s\t%s\n%s\n%s\n' %(p[0], perr[0],
##                p[1], perr[1], p[2], perr[2], 
##                extrapolated_vals[0].nominal_value,
##                extrapolated_vals[0].std_dev,
##                extrapolated_vals[1].nominal_value,
##                extrapolated_vals[1].std_dev, chi2red, elapsed))
#
#    with open('test.txt',"a") as myfile:
#        myfile.write('%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' %(cut,
#                extrapolated_vals[0].nominal_value,
#                extrapolated_vals[0].std_dev,
#                extrapolated_vals[1].nominal_value,
#                extrapolated_vals[1].std_dev, 
#                chi2red, elapsed, p_val))
#
#plt.semilogx()
#plt.legend(loc = 'best')
#plt.title('2 logs Fit for 2 Hour Run')
#plt.xlabel('time [s]')
#plt.ylabel('$B_{leak}$ [mT]')
#plt.savefig('logs2_fit_2hrs.png')
#plt.close()

#plt.plot(t,b,'.', label = 'data')
#with open('test.txt',"a") as myfile:
#    myfile.write('#time cut\t1day_extrapolation\t uncertainty\t1year_extrapolation\
#    \t uncertainty\tinfinity extrapolation\
#    \tuncertainty\tchi2red\tprocessing time\tp-val\n')
#for ind, cut in enumerate(t_cuts):
#    x = t[0:cut]
#    y = b[0:cut]
#    sig_y = sig_b[0:cut]
#
#    [chi2red, resid, extrapolated_vals, extrap_inf, fit_type, p, perr, yfit,
#            elapsed, p_val] = fit_2stretched_exp(x, y, sig_y,t )
#
#    plt.plot(t,yfit, '--', label = legend_log[ind], linewidth = 1)
##    with open(ofile_log[ind],"a") as myfile:
##        myfile.write('%s\t%s\n%s\t%s\n%s\t%s\n%s\t%s\n%s\t%s\n%s\n%s\n' %(p[0], perr[0],
##                p[1], perr[1], p[2], perr[2], 
##                extrapolated_vals[0].nominal_value,
##                extrapolated_vals[0].std_dev,
##                extrapolated_vals[1].nominal_value,
##                extrapolated_vals[1].std_dev, chi2red, elapsed))
#
#    with open('test.txt',"a") as myfile:
#        myfile.write('%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' %(cut,
#                extrapolated_vals[0].nominal_value,
#                extrapolated_vals[0].std_dev,
#                extrapolated_vals[1].nominal_value,
#                extrapolated_vals[1].std_dev, 
#                extrap_inf.nominal_value,extrap_inf.std_dev, 
#                chi2red, elapsed, p_val))
#
#plt.semilogx()
#plt.legend(loc = 'best')
#plt.title('2 Stretched Exponential Fit for 2 Hour Run')
#plt.xlabel('time [s]')
#plt.ylabel('$B_{leak}$ [mT]')
#plt.savefig('exp_2fit_2hrs.png')
#plt.close()
#

