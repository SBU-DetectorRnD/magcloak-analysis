import sys
import argparse

import numpy as np
import pandas as pd

from uncertainties import ufloat, unumpy

import load as ld
import applied_field as af

def calibrateGaussmeter(pars):

    print ( pars[0] )
    print ( pars[1] )
    print ( pars[2] )

    # hard-coded input folder name
    fname_in = "data/DATA_Gaussmeter/"
    fname_in += pars[0]

    # hard-coded output folder name
    fname_out = "data-calib/DATA_Gaussmeter/"
    fname_out += pars[0]
    fname_out = fname_out.replace(".txt",".csv")

    # file with calibration measurement
    fname_cal = "data/DATA_Gaussmeter/"
    fname_cal += pars[2]

    data = ld.Gaussmeter(fname_in, drop=False)

    df = pd.DataFrame(data)

    # Calculate nominal magnet field Bnom based on current and calibration measurement
    Bnom = af.calc_applied_field_lin( df['multi'].values, fname_cal)
    df['Bnom'] = unumpy.nominal_values(Bnom)
    df['Bnom_sdev'] = unumpy.std_devs(Bnom)

    # Print three line as crosscheck
    print(df.head(3))

    # Write csv output
    df.to_csv(fname_out, index=False)


# Run bottom part only if macro is being called directly (i.e. not imported)
if __name__ == '__main__':

    # Define input file list via command line
    parser = argparse.ArgumentParser()
    parser.add_argument("setlist") #, metavar = 'S', help="List of files to clibrate", action="store_const")

    args = parser.parse_args()

    f = open(args.setlist, "r")
    parlines = f.read().splitlines()

    for parline in parlines:

        if not ( parline.startswith("#") ):
            pars = parline.split()

            if len(pars) == 3:
                calibrateGaussmeter( pars )
            else:
                print ("Skipping line:")
                print (parline)
