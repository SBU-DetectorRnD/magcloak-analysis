import sys
import argparse

import numpy as np
import pandas as pd

import load as ld


def calibrateMegaVIEW(pars, calibconsts="none"):

    print ( pars[0] )
    print ( pars[1] )

    # hard-coded input folder name
    fname_in = "data/DATA_MegaVIEW/"
    fname_in += pars[0]

    # hard-coded output folder name
    fname_out = "data-calib/DATA_MegaVIEW/"
    fname_out += pars[0]
    fname_out = fname_out.replace(".txt",".csv")

    # read raw data and convert to Pandas dataframe
    data = ld.MegaVIEW(fname_in, drop=False)
    df = pd.DataFrame(data)

    # if calibration constants specified: apply them
    if ( calibconsts != "none" ):
        calib = pd.read_csv(calibconsts,comment='#')
        print("Using calibration constants:")
        print(calibconsts)
        print(calib.head(2))
        dfcal = df.mul(calib.loc[0,:], axis='columns')


    # Drop (unused) temperature and multimeter reading columns
    dfcal = dfcal[['time','x','y','z','B1','B1range','B2','B2range','B3','B3range']]

    # Set nominal MRI field given in input filelist (based on set magnet current, see eLog)
    dfcal['Bnom'] = pars[1]
    dfcal['Bnom_sdev'] = 0

    # Print three lines as crosscheck
    print(dfcal.head(3))

    # Write csv output
    dfcal.to_csv(fname_out, index=False)


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

            if len(pars) == 2:
                calibrateMegaVIEW( pars, "calibrations/calibrations_MegaVIEW_ANL2016.csv" )
            else:
                print ("Skipping line:")
                print (parline)



