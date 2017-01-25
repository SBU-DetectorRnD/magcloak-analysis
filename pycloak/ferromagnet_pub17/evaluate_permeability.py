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


def calc_u_cloak( radius_fm_inner, radius_fm_outer):

    # ratio of inner and outer radius
    c = radius_fm_inner / radius_fm_outer

    print("thickness ratio: ", c)

    #calculate the perfect cloak permeability based on the dimensions of your
    #ferromagnet
    u_cloak = (1/c.n**2+1)/(1/c.n**2-1)
    print("Perfect cloak permeability: ", u_cloak)


#define a function that will take the ferromagnet scan file and give you the
#permeability results.
def calc_u(Bin, Bout, radius_inner, radius_outer, offsets=0):

    # apply offsets if needed
    Bin += offsets

    #ratio of inner and outer radius
    radius_ratio = radius_inner / radius_outer
    c = radius_ratio

    B = Bin/Bout #ratio of internal to external field

    #calculate permeability
    print ("B ratio:",B)
    print ("radius ratio:",c)

    mu = ( B*c**2
           + B
           - 2
           - 2*unumpy.sqrt( B**2*c**2 - B*c**2 - B + 1 )
           ) / ( B*c**2 - B ) 

    #print(mu)

    #cakculate uerr with just point to point uncertainties. I define this as just
    #uncertainty from the field measurements
    u_pp = (B*c.n**2 + B -2 -2*unumpy.sqrt(B**2*c.n**2 - B*c.n**2 - B +
                                           1))/(B*c.n**2-B)

    u_err_pp = unumpy.std_devs(u_pp)

    #calculate uerr from just geometry uncertainties
    u_geom = ( unumpy.nominal_values(B)*c**2
               + unumpy.nominal_values(B)
               - 2
               - 2*unumpy.sqrt(unumpy.nominal_values(B)**2*c**2
                               - unumpy.nominal_values(B)*c**2
                               - unumpy.nominal_values(B)
                               + 1) ) / ( unumpy.nominal_values( B )*c**2 - unumpy.nominal_values( B ) )

    u_err_geom = unumpy.std_devs(u_geom)

    # return value+error
    return unumpy.uarray( mu, u_err_geom )


def evaluate_permeability( fname_data, Bin, Bout, fname_di, fname_do ):

    print ("Runnign for :", fname_data, Bin, Bout, fname_di, fname_do )

    # prepare result dataframe
    result = pd.DataFrame(columns = ["Bout","Bout_sdev","mu","mu_sdev"])

    # get data
    data = pd.read_csv(fname_data)

    # set uncertainty for fiel reading manually to 0 if it does not exist
    if Bin + '_sdev' not in data:
        data[Bin + '_sdev'] = 0

    if Bout + '_sdev' not in data:
        data[Bout + '_sdev'] = 0

    # copy Bin and Bout to results
    result['Bout_c'] = unumpy.uarray( abs( data[Bout].values ), data[Bout + '_sdev'].values)
    result['Bin_c'] = unumpy.uarray( abs( data[Bin].values ), data[Bin + '_sdev'].values)

    # get inner and outer diameter
    diam_in = mean_radius('diameter_files/'+fname_di)
    diam_out = mean_radius('diameter_files/'+fname_do)

    # check ideal permeability
    calc_u_cloak(diam_in, diam_out)

    # calculate permeability and fill in dataframe column 'mu_c'
    result ['mu_c'] = calc_u(Bin=result['Bin_c'], Bout=result['Bout_c'], radius_inner=diam_in, radius_outer=diam_out)

    # extract nominal value and standard deviation of 'mu_c' into 'mu' and 'mu_sdev'
    result["mu"] = unumpy.nominal_values(result["mu_c"])
    result["mu_sdev"] = unumpy.std_devs(result["mu_c"])

    # same for Bout
    result["Bout"] = unumpy.nominal_values(result["Bout_c"])
    result["Bout_sdev"] = unumpy.std_devs(result["Bout_c"])

    # drop 'mu_c' and 'Bout_c'
    result.drop('mu_c', axis=1, inplace=True)
    result.drop('Bin_c', axis=1, inplace=True)
    result.drop('Bout_c', axis=1, inplace=True)

    return (result)


def evaluate_permeability_for_set(setlist="foo.txt", results_file="foo2.txt"):

    # delete results file if exists
    if os.path.exists(results_file):
        os.remove(results_file)

    # create results dataframe
    dresults = pd.DataFrame(columns = ["File","ID","frac","Bout","Bout_sdev","mu","mu_sdev"])

    # open input lines
    f = open(setlist, "r")
    parlines = f.read().splitlines()

    for parline in parlines:

        if not ( parline.startswith("#") ):
            pars = parline.split()

            if len(pars) == 7:
                sresult = evaluate_permeability( fname_data=pars[2], Bin=pars[3], Bout=pars[4], fname_di=pars[5], fname_do=pars[6] )

                sresult["ID"] = pars[0]
                sresult["frac"] = pars[1]
                sresult["File"] = pars[2].split('/')[-1]

                dresults = dresults.append( sresult )

            else:
                print ("Skipping line:")
                print (parline)

        else:
            print ("Skipping line:")
            print (parline)

            dresults.to_csv(results_file, index=False, na_rep='NaN')

    return


if __name__ == '__main__':

    evaluate_permeability_for_set( setlist = "filelist_ferromagnet_mri.txt", results_file = "results/ferromagnet_mri.csv")
