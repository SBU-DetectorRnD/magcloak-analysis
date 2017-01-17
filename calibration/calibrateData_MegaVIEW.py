import sys
import argparse

import numpy as np
import pandas as pd

import load as ld


def calibrateMegaVIEW(pars):

    print ( pars[0] )
    print ( pars[1] )

    # hard-coded input folder name
    fname_in = "data/DATA_MegaVIEW/"
    fname_in += pars[0]

    # hard-coded output folder name
    fname_out = "data-calib/DATA_MegaVIEW/"
    fname_out += pars[0]
    fname_out.replace(".txt",".csv")

    data = ld.MegaVIEW(fname_in, drop=False)

    df = pd.DataFrame(data)

    # Drop (unused) temperature and multimeter reading columns
    dfcal = df[['time','x','y','z','B1','B1range','B2','B2range','B3','B3range']]

    # Set nominal MRI field given in input filelist (based on set magnet current, see eLog)
    dfcal['Bmri'] = pars[1]

    # Print one line as crosscheck
    print(dfcal.head(1))

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
                calibrateMegaVIEW( pars )
            else:
                print ("Skipping line:")
                print (parline)



