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

    #calculate the perfect cloak permeability based on the dimensions of your ferromagnet
    u_cloak = (1/c.n**2+1)/(1/c.n**2-1)

    print("Ratio inner/outer radius: ", c, " --> Perfect cloak permeability (theory): ", u_cloak)


# take field measurements with ferromagnet and ferromagnet radii and return the permeability results, point-to-point
# uncertainties, and systematic (here: geometric) uncertainties.
def calc_mu(Bin, Bout, radius_inner, radius_outer):

    # ratio of inner and outer radius
    radius_ratio = radius_inner / radius_outer
    print radius_ratio
    # ratio of internal to external field
    B_ratio = Bin / Bout
    # convert from Series object to Numpy Array object
    B_ratio = B_ratio.values

    # If value under square root becomes neagtive, set B_ratio to NaN
    # (this seems to happen with Argonne MRI measurements at low fields)
    B_ratio[ (B_ratio**2) * (radius_ratio**2) - B_ratio * (radius_ratio**2) - B_ratio + 1 < 0 ] = np.nan

    # Calculate permeability. Here, use both uncertainties from field measurements and uncertainties from geometry, i.e. radius measurements.
    mu = ( B_ratio * (radius_ratio**2)
           + B_ratio
           - 2
           -2 * unumpy.sqrt( (B_ratio**2) * (radius_ratio**2) - B_ratio * (radius_ratio**2) - B_ratio + 1 )
           ) / ( B_ratio * (radius_ratio**2) - B_ratio )

    # store nominal values of mu in separate array
    mu_val = unumpy.nominal_values(mu)

    # store combined uncertainties of mu in separate array
    mu_err = unumpy.std_devs(mu)

    # Calculate uncertainties of mu values from just field measurement uncertainties (= point-to-point fluctuations). Ignore geometry (=radius) uncertainties.
    mu_pp = ( B_ratio * (radius_ratio.n**2)
              + B_ratio
              - 2
              -2 * unumpy.sqrt( (B_ratio**2) * (radius_ratio.n**2) - B_ratio * (radius_ratio.n**2) - B_ratio + 1 )
              ) / ( B_ratio * (radius_ratio.n**2) - B_ratio )

    # store point-to-point uncertainties of mu in separate array
    mu_err_pp = unumpy.std_devs(mu_pp)

    # Calculate uncertainties of mu values from just geometry uncertainties (= systematic uncertainty, i.e. all points move together). Ignore field uncertainties.
    B_ratio_n = unumpy.nominal_values( B_ratio )
    mu_geom = ( B_ratio_n * (radius_ratio**2)
                + B_ratio_n
                - 2
                -2 * unumpy.sqrt( (B_ratio_n**2) * (radius_ratio**2) - B_ratio_n * (radius_ratio**2) - B_ratio_n + 1 )
                ) / ( B_ratio_n * (radius_ratio**2) - B_ratio_n )

    # store geometric uncertainties of mu in separate array
    mu_err_geom = unumpy.std_devs(mu_geom)

    return( mu_val, mu_err, mu_err_pp, mu_err_geom )


# process a single measurement file to calculate magnetic permeability
# * fname_data = name of measurement file
# * Bin = name of column in fname_data file storing magnetic field inside ferromagnet cylinder
# * Bout = name of column in fname_data file storing magnetic field without ferromagnet cylinder
# * fname_di = name of file listing inner diameter measurements for this ferromagnet
# * fname_do = name of file listing outer diameter measurements for this ferromagnet
def evaluate_permeability( fname_data="samplemeasurement.csv", Bin="B1", Bout="B2", fname_do="fm618_do_cryo.txt", fname_th="fm618_th_cryo.txt" ):

    print ("Evaluating permeability for: ", fname_data, Bin, Bout, fname_do, fname_th )

    # prepare result dataframe
    result = pd.DataFrame(columns = ["Bout","Bout_sdev","mu","mu_err_pp","mu_err_geom"])
    
    # get data
    data = pd.read_csv(fname_data)

    print(data.head(10))

    # set uncertainty for field reading manually to 0 if it does not exist
    if Bin + '_sdev' not in data:
        print ("Use uncertainty of 0.005 for " , Bin+'_sdev')
        data[Bin + '_sdev'] = 0.005 #Gaussmeter precision 0.01 mT above 30 mT)

    # set uncertainty for field reading manually to 0 if it does not exist
    if Bout + '_sdev' not in data:
        print ("Use uncertainty of 0.005 for " , Bout+'_sdev')
        data[Bout + '_sdev'] = 0.005

    # copy Bin and Bout to results
    result['Bout_c'] = unumpy.uarray( abs( data[Bout].values ), data[Bout + '_sdev'].values)
    result['Bin_c'] = unumpy.uarray( abs( data[Bin].values ), data[Bin + '_sdev'].values)
    
    # get inner and outer diameter
    diam_out = mean_from_file('diameter_files/'+fname_do)
    thickness = mean_from_file('diameter_files/'+fname_th)
    diam_in = diam_out - 2*thickness

    # check ideal permeability
    calc_mu_cloak(diam_in, diam_out)

    # calculate permeability and store values and uncertainties in arrays
    (mu_c, mu_err, mu_err_pp, mu_err_geom) = calc_mu(Bin=result['Bin_c'], Bout=result['Bout_c'], radius_inner=diam_in, radius_outer=diam_out)

    result["mu"] = mu_c
    result["mu_err"] = mu_err
    result["mu_err_pp"] = mu_err_pp
    result["mu_err_geom"] = mu_err_geom

    # same for Bout
    result["Bout"] = unumpy.nominal_values(result["Bout_c"])
    result["Bout_sdev"] = unumpy.std_devs(result["Bout_c"])

    #print(result.head(10))

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
    dresults = pd.DataFrame(columns = ["File","ID","frac","Bout","Bout_sdev","mu","mu_err","mu_err_pp","mu_err_geom"])

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
                
                # calculate permeability for entries in single measurement file, return dataframe with results
                sresult = evaluate_permeability( fname_data=pars[2], Bin=pars[3], Bout=pars[4], fname_do=pars[5], fname_th=pars[6] )

                # set additional columns in results dataframe based on input parmeters
                sresult["ID"] = pars[0]
                sresult["frac"] = pars[1]
                sresult["File"] = pars[2].split('/')[-1]

                # append results dataframe for this measurement file to the results summary dataframe
                dresults = dresults.append( sresult )

            else:
                print ("Skipping line: ", parline)

        else:
            print ("Skipping line: ", parline)

    # write resuts summary dataframe to CSV file
    dresults.to_csv(results_file, index=False, na_rep='NaN')

    return


# if you call this macro form the command line, this __main__ function will be executed:
if __name__ == '__main__':

    evaluate_permeability_for_set( setlist = "filelist_ferromagnet_sbu_troom.txt", results_file = "results/ferromagnet_sbu_troom.csv")
    evaluate_permeability_for_set( setlist = "filelist_ferromagnet_sbu.txt", results_file = "results/ferromagnet_sbu.csv")
    evaluate_permeability_for_set( setlist = "filelist_ferromagnet_mri.txt", results_file = "results/ferromagnet_mri.csv")
