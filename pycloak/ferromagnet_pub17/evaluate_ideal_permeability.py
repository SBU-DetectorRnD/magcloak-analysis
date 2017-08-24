#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
from uncertainties import unumpy
from uncertainties import ufloat
import pandas as pd
import sys
import os

def mean_from_file(radfile):
    # turn files of tube dimensions into array
    r = np.genfromtxt(radfile)
    a = ufloat(np.mean(r), np.std(r)) #radius value,err
    return a


def calc_mu_cloak( radius_fm_inner, radius_fm_outer):

    # ratio of inner and outer radius
    c = radius_fm_inner / radius_fm_outer

    #calculate the perfect cloak permeability based on the dimensions of your
    #ferromagnet
    #u_cloak = (1/c.n**2+1)/(1/c.n**2-1)
    u_cloak = (1/c**2+1)/(1/c**2-1)

    return u_cloak


if __name__ == '__main__':

    setlist="filelist_ferromagnet_mri.txt"
    results_file="results/cloak_fm_permeability_ideal.csv"
    

    # delete results file if exists
    if os.path.exists(results_file):
        os.remove(results_file)

    # create results dataframe
    dresults = pd.DataFrame( columns = ["ID","diam_in","diam_out","mu_ideal"] )    

    # open input lines
    f = open(setlist, "r")
    parlines = f.read().splitlines()

    for parline in parlines:

        if not ( parline.startswith("#") ):
            pars = parline.split()

            if len(pars) == 7:

                ID = pars[0]
                frac = pars[1]
                fname_do=pars[5]
                fname_th=pars[6]

                if (dresults['ID']==ID).any():
                    continue

                # get inner and outer diameter
                diam_out = mean_from_file('diameter_files/'+fname_do)
                thickness = mean_from_file('diameter_files/'+fname_th)
                diam_in = diam_out - 2*thickness

                # get ideal permeability
                mu_ideal = calc_mu_cloak( diam_in , diam_out )

                print(ID,diam_in,diam_out,mu_ideal)

                # append to result
                d = {'ID': ID, 'diam_in': diam_in, 'diam_out': diam_out, 'mu_ideal': mu_ideal}
                dresults = dresults.append(d, ignore_index=True)

    print(dresults)
    dresults.to_csv(results_file, index=False, na_rep='NaN')
