#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
from uncertainties import unumpy
from uncertainties import ufloat
import pandas as pd
import sys
import os

def mean_radius(radfile):
    # turn files of tube dimensions into array
    r = np.genfromtxt(radfile)
    a = ufloat(np.mean(r), np.std(r)) #radius value,err
    return a


def calc_mu_cloak( radius_fm_inner, radius_fm_outer):

    # ratio of inner and outer radius
    c = radius_fm_inner / radius_fm_outer

    #calculate the perfect cloak permeability based on the dimensions of your ferromagnet
    u_cloak = (1/c.n**2+1)/(1/c.n**2-1)
    print("Ratio inner/outer radius: ", c, " --> Perfect cloak permeability (theory): ", u_cloak)


#define a function that will take the ferromagnet scan file and give you the
#permeability results.
def calc_mu(Bin, Bout, radius_inner, radius_outer, offsets=0):

    # apply offsets if needed
    Bin += offsets

    #ratio of inner and outer radius
    radius_ratio = radius_inner / radius_outer
    c = radius_ratio

    B = Bin/Bout #ratio of internal to external field
    B = B.values

    # If value under square root becomes neagtive, force it to be 1
    #B[B**2*c**2 - B*c**2 - B + 1 < 0] = 1
    #print(B**2*c**2 - B*c**2 - B + 1)

    #calculate permeability
    mu = ( B*c**2
           + B
           - 2
           - 2*unumpy.sqrt( B**2*c**2 - B*c**2 - B + 1 )
           ) / ( B*c**2 - B ) 

    #calculate uerr with just point to point uncertainties. I define this as just
    #uncertainty from the field measurements
    mu_pp = (B*c.n**2 + B -2 -2*unumpy.sqrt(B**2*c.n**2 - B*c.n**2 - B +
                                           1))/(B*c.n**2-B)

    mu_err_pp = unumpy.std_devs(mu_pp)

    #calculate uerr from just geometry uncertainties
    u_geom = ( unumpy.nominal_values(B)*c**2
               + unumpy.nominal_values(B)
               - 2
               - 2*unumpy.sqrt(unumpy.nominal_values(B)**2*c**2
                               - unumpy.nominal_values(B)*c**2
                               - unumpy.nominal_values(B)
                               + 1) ) / ( unumpy.nominal_values( B )*c**2 - unumpy.nominal_values( B ) )

    mu_err_geom = unumpy.std_devs(u_geom)

    return( unumpy.nominal_values(mu), mu_err_pp, mu_err_geom )


# process a single measurement file
def evaluate_permeability( fname_data, Bin, Bout, fname_di, fname_do ):

    print ("Evaluating permeability for: ", fname_data, Bin, Bout, fname_di, fname_do )

    # prepare result dataframe
    result = pd.DataFrame(columns = ["Bout","Bout_sdev","mu","mu_err_pp","mu_err_geom"])

    # get data
    data = pd.read_csv(fname_data)

    # set uncertainty for fiel reading manually to 0 if it does not exist
    if Bin + '_sdev' not in data:
        data[Bin + '_sdev'] = 0.0005

    if Bout + '_sdev' not in data:
        data[Bout + '_sdev'] = 0.0005

    # copy Bin and Bout to results
    result['Bout_c'] = unumpy.uarray( abs( data[Bout].values ), data[Bout + '_sdev'].values)
    result['Bin_c'] = unumpy.uarray( abs( data[Bin].values ), data[Bin + '_sdev'].values)

    # get inner and outer diameter
    diam_in = mean_radius('diameter_files/'+fname_di)
    diam_out = mean_radius('diameter_files/'+fname_do)

    # check ideal permeability
    calc_mu_cloak(diam_in, diam_out)

    # calculate permeability and store values and uncertainties in arrays
    (mu_c, mu_err_pp, mu_err_geom) = calc_mu(Bin=result['Bin_c'], Bout=result['Bout_c'], radius_inner=diam_in, radius_outer=diam_out)

    result["mu"] = mu_c
    result["mu_err_pp"] = mu_err_pp
    result["mu_err_geom"] = mu_err_geom

    # same for Bout
    result["Bout"] = unumpy.nominal_values(result["Bout_c"])
    result["Bout_sdev"] = unumpy.std_devs(result["Bout_c"])

    # drop 'mu_c' and 'Bout_c'
    result.drop('Bin_c', axis=1, inplace=True)
    result.drop('Bout_c', axis=1, inplace=True)

    return (result)


# loop over input file with list of measurement files to process
def evaluate_permeability_for_set(setlist="foo.txt", results_file="foo2.txt"):

    # delete results file if exists
    if os.path.exists(results_file):
        os.remove(results_file)

    # create results dataframe
    dresults = pd.DataFrame(columns = ["File","ID","frac","Bout","Bout_sdev","mu","mu_err_pp","mu_err_geom"])

    # open input file and get lines
    f = open(setlist, "r")
    parlines = f.read().splitlines()

    # loop over lines from input file (list of measuremets)
    for parline in parlines:

        # only process lines that don't start with #
        if not ( parline.startswith("#") ):
            pars = parline.split()

            # only process lines with correct number of parameters
            if len(pars) == 7:

                # calculate permeability for entries in single measurement file
                sresult = evaluate_permeability( fname_data=pars[2], Bin=pars[3], Bout=pars[4], fname_di=pars[5], fname_do=pars[6] )

                sresult["ID"] = pars[0]
                sresult["frac"] = pars[1]
                sresult["File"] = pars[2].split('/')[-1]

                # append results for this measurement file to the output dataframe
                dresults = dresults.append( sresult )

            else:
                print ("Skipping line: ", parline)

        else:
            print ("Skipping line: ", parline)

    # write results to CSV file
    dresults.to_csv(results_file, index=False, na_rep='NaN')

    return


if __name__ == '__main__':

    evaluate_permeability_for_set( setlist = "filelist_ferromagnet_sbu.txt", results_file = "results/ferromagnet_sbu.csv")
#    evaluate_permeability_for_set( setlist = "filelist_ferromagnet_mri.txt", results_file = "results/ferromagnet_mri.csv")
