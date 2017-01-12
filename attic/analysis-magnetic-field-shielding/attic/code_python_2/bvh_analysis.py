#!/usr/bin/env python

import sys

sys.path.append('/home/ephenix/anaconda3/pkgs/')

import os
import argparse
import numpy as np
import matplotlib.pyplot as plt
import uncertainties
from uncertainties import ufloat
from uncertainties import unumpy
from scipy.optimize import curve_fit
from multiprocessing import Pool

import path
import applied_field
import adjust_b
import fitting
import summary
import quality_check
#changing set_list from 'sets_list_paper.txt' to 'sets_list_1layertest.txt'
set_list = 'sets_list_1layertest.txt'

#add flag if you want to plot each fit individually 
parser = argparse.ArgumentParser()
parser.add_argument("-g", "--graph", help="Graph the individual fits",
                    action="store_true")
args = parser.parse_args()

#define how long you want to extrapolate
extrapolate_time = 1.578e+7  #1/2 year
#define how many processes you want to run in parallel
cores = 1

#names of many files are defined here. See path.py for more detail.
[description, datafolder, datalist, pre_cool_offset, post_cool_offset, 
        calibration, sig_gaussmeter_file, sig_multimeter_file, 
        fit_graphs, fit_file, param_fit_file] = path.paths(set_list)

#define a function to be looped for each run. A run consists of multiple
#files created from ramping up the current.
def analyze_run(ind):

    print('************')
    #description of the run. E.g. run079_4layer_helix
    print(description[ind])

    #check if the initial magnetic field is too large or if the calibration is
    #the wrong sign. See quality_check.py for more details
    if not quality_check.initial_quality(pre_cool_offset[ind], calibration[ind]):
        print('%s does not pass quality test'%(description[ind]))
        return


    #create a summary file and give it the proper header
    with open(fit_file[ind], "a") as myfile:
        myfile.write('#%s\n' %(description[ind]))
        myfile.write('#b0\ts_b0\tbin(1/2year)\ts_bin(1/2year)\tshield\ts_shield\tsf\ts_sf\tt_dependent\tovershoot\n')

    #open the list of data files and turn it into a variable.
    with open(datalist[ind]) as f:
        Data = f.read().splitlines()

  
    #go through each data file
    for d in Data:
        #skip line if it is blank
        if not d:
            continue

        #print datafile name
        print(d)
        #parse datafile into columns
        my_data = np.genfromtxt(datafolder[ind] + d, delimiter='\t')

        #first column is time, second is current, 3rd is internal field
        t = my_data[:,0]
        i = abs(my_data[:,1])
        b = my_data[:,2]
#       
#        if not quality_check.calibration_range(calibration[ind], i):
#            continue

        #leakage field after taking into account calibration change from
        #cooling hall probe. Add undertainty. Returns unumpy array
        b_leak = adjust_b.calc_b_leak(b, sig_gaussmeter_file[ind], 
                pre_cool_offset[ind], post_cool_offset[ind])

        #calculate applied field from calibration. REturns ufloat
        b0 = applied_field.calc_applied_field(i, sig_multimeter_file[ind], 
                calibration[ind]) 


        #ignore, this is irrelevant
        [b_before_cool, b_after_cool] =adjust_b.calc_offsets(
                pre_cool_offset[ind], post_cool_offset[ind])

        x = t
        y = unumpy.nominal_values(b_leak)
        yerr = unumpy.std_devs(b_leak)
        
        #title of single plot. only relevant if -g flag is on
        title = 'Critical Field Measurement Test, $B_0 =  \
            %5.3f \pm % 5.3f mT$' % (b0.nominal_value, b0.std_dev)

        #fit functions to data
        fitting.analyze(x, y, yerr, extrapolate_time, args.graph, b0,
            b_before_cool, title, description[ind], fit_file[ind], fit_graphs[ind], param_fit_file[ind])
        print("\n")


    #print(param_fit_file[ind])
    summary.plot_summaries([fit_file[ind]], description[ind],
            param_fit_file[ind])

#loop through all different runs. the pool is to use a different process for
#each run.
if __name__ == '__main__':
    with Pool(processes= cores) as pool:
        pool.map(analyze_run, range(len(datalist)))
        #pool.map(analyze_run, range(1))

#plot summary files
summary.plot_summaries(list(set(fit_file)), 'total', list(set(param_fit_file)))

