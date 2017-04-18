#!/usr/bin/env python3

import numpy as np
import pandas as pd
import load as ld
from scipy.interpolate import interp1d
from scipy.interpolate import PchipInterpolator
from uncertainties import ufloat, unumpy
from add_uncertainties import quad_add
import sys

#get applied field from mean and standard deviation of current
 
def i_stats(i, multimeter_file):
#    sig_mult = np.genfromtxt(multimeter_file)
    sig_mult=[0, 0, 0]
    return (np.mean(i), quad_add([np.std(i), sig_mult[2]]))

def calc_applied_field(i, multimeter_file="", calibration_file=""):
    [i_mean, sig_i] = i_stats(i, multimeter_file)
    calibration = np.genfromtxt(calibration_file)
#    f = interp1d(calibration[:,1], calibration[:,2])
    calibration = calibration[calibration[:,1].argsort()]
    f = PchipInterpolator(calibration[:,1], calibration[:,2], 
            extrapolate = True)
    b0 = f(i_mean)
    sig_b0 = abs((f(i_mean+sig_i)-f(i_mean - sig_i))/2)
    print("Applied Field:\t" + str(b0) + "+/-" + str(sig_b0))
    return ufloat(b0, sig_b0)

def calc_applied_field_lin(i, calibration_file=""):
    data_calibration = ld.Gaussmeter(calibration_file, drop=False)
    df_calibration = pd.DataFrame(data_calibration)

    # linear fit to calibration measurement
    # polynomial p(x) = p[0] * x**deg + ... + p[deg] of degree deg
    p, cov = np.polyfit(df_calibration['multi'], df_calibration['B1'], 1,  cov=True)

    # get uncertainties for fit parameters p0 and p1
    sigma_p0 = np.sqrt( cov[0,0] );
    sigma_p1 = np.sqrt( cov[1,1] );

    # set uncertainty of current reading from multimeter
    sigma_current = 0.01

    b0 = np.polyval(p,i)

    #sig_b0 = abs((np.polyval(p,i+sigma_current) - np.polyval(p,i-sigma_current))/2)
    sigma_b0 = np.sqrt( ( p[0]**2 * sigma_current**2 ) + ( i**2 * sigma_p0**2 ) + ( 1 * sigma_p1**2 ) );

    return unumpy.uarray(b0, sigma_b0)
