#!/usr/bin/env python3

import numpy as np
from scipy.interpolate import interp1d
from scipy.interpolate import PchipInterpolator
from uncertainties import ufloat
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

def calc_applied_field_lin(i, multimeter_file="", calibration_file=""):
    [i_mean, sig_i] = i_stats(i, multimeter_file)
    calibration = np.genfromtxt(calibration_file)
    p, cov = np.polyfit(calibration[:,1], calibration[:,2], 1,  cov=True)
    print(p)
    b0 = np.polyval(p,i_mean)
    sig_b0 = abs((np.polyval(p,i_mean+sig_i) - np.polyval(p,i_mean-sig_i))/2)
    return ufloat(b0, sig_b0)
