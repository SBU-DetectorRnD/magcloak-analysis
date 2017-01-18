#!/usr/bin/env python3

import sys
import os
import argparse

import numpy as np
import pandas as pd

#import load as ld
#import applied_field as af

def x_dependent(x,y):
    cut = x.size/3
    mean_first = np.mean(y[0:cut])
    std_first = np.std(y[0:cut])
    mean_last = np.mean(y[-1-cut:-1])
    std_last = np.std(y[-1-cut:-1])
    return( abs(mean_first-mean_last) > (std_first + std_last))


def evaluate_shielding(pars):

    fname_data = pars[0] + "/" + pars[1]

    df = pd.read_csv(fname_data)

    #Time dependece?
    timedependence = ( len(df['time']) > 10 ) and x_dependent(df['time'].values, abs(df['B3']).values)

    Bout_mean = df['Bnom'].mean()
    Bout_sdev = df['Bnom_sdev'].mean()
    Bins_mean = abs(df['B3']).mean()
    Bins_sdev = abs(df['B3']).std()

    return (fname_data, Bout_mean, Bout_sdev, Bins_mean, Bins_sdev, timedependence)


# Run bottom part only if macro is being called directly (i.e. not imported)
if __name__ == '__main__':

    # Define input file list via command line
    parser = argparse.ArgumentParser()
    parser.add_argument("setlist") #, metavar = 'S', help="List of files to clibrate", action="store_const")

    args = parser.parse_args()

    f = open(args.setlist, "r")
    parlines = f.read().splitlines()

    results_file = "foo.txt"

    if os.path.exists(results_file):
        os.remove(results_file)

    with open(results_file, "a") as myfile:
        myfile.write('file,Bout,Bout_sdev,Bins,Bins_sdev,tdep\n')

    for parline in parlines:

        if not ( parline.startswith("#") ):
            pars = parline.split()

            if len(pars) == 3:
                sresult = evaluate_shielding( pars )
                print(sresult)
                
                with open(results_file, "a") as myfile:
                    myfile.write('%s,%s,%s,%s,%s,%s\n' %sresult)

            else:
                print ("Skipping line:")
                print (parline)
