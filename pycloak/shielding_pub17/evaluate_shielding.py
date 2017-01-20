#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

#import fitting.py

def x_dependent(x,y):
    cut = x.size/3
    mean_first = np.mean(y[0:cut])
    std_first = np.std(y[0:cut])
    mean_last = np.mean(y[-1-cut:-1])
    std_last = np.std(y[-1-cut:-1])
    return( abs(mean_first-mean_last) > (std_first + std_last))




def evaluate_shielding(fname_data="foolu.txt", B="B1"):

    # read data
    df = pd.read_csv(fname_data)

    #Time dependece?
    timedependence = ( len(df['time']) > 10 ) and x_dependent(df['time'].values, abs(df[B]).values)

    Bout_mean = df['Bnom'].mean()
    Bout_sdev = df['Bnom_sdev'].mean()
    Bins_mean = abs(df[B]).mean()
    Bins_sdev = abs(df[B]).std()

    return (fname_data, Bout_mean, Bout_sdev, Bins_mean, Bins_sdev, timedependence)



def evaluate_shielding_for_set(setlist="foo.txt", results_file="foo2.txt"):

    # delete results file if exists
    if os.path.exists(results_file):
        os.remove(results_file)

    # create results file and add header
    with open(results_file, "a") as myfile:
        myfile.write('file,Bout,Bout_sdev,Bins,Bins_sdev,tdep\n')

    # open input lines
    f = open(setlist, "r")
    parlines = f.read().splitlines()

    for parline in parlines:

        if not ( parline.startswith("#") ):
            pars = parline.split()

            if len(pars) == 3:
                sresult = evaluate_shielding( fname_data=pars[0], B=pars[2] )
                print(sresult)
                
                with open(results_file, "a") as myfile:
                    myfile.write('%s,%s,%s,%s,%s,%s\n' %sresult)

            else:
                print ("Skipping line:")
                print (parline)

        else:
            print ("Skipping line:")
            print (parline)

    return
