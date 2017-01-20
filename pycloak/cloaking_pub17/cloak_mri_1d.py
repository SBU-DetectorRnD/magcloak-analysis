import os
import sys

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from matplotlib import cm


def comparescans( figname, figtitle, datafiles, datanames, legend=True ):

    if ( len( datafiles ) != len( datanames ) ):
        print ("ERROR: Mismatch length datafiles and datanames. Exit.")
        sys.exit(1)


    # set figure parameters
    fig, ax = plt.subplots( figsize=(6,5) )
    ax.tick_params(labelsize=12)
    ax.set_title(figtitle)
    plt.xlabel("x-position (mm)",fontsize=12)
    plt.ylabel("$B_{T}$ (mT)",fontsize=12)

    # mark cloak dimensions
    plt.axvline(-20.32, color='grey', linestyle='--')
    plt.axvline(20.31, color='grey', linestyle='--')

    # first: plot lines for nominal fields
    for i, (dfile, dname) in enumerate( zip( datafiles, datanames ) ):
        print( dfile, dname )
        data = pd.read_csv(dfile, comment='#')
        
        ax.axhline( abs(data['B2']).mean() , color='grey' , linestyle='--')
        
    # second: plot field map data
    for i, (dfile, dname) in enumerate( zip( datafiles, datanames ) ):
        print( dfile, dname )
        data = pd.read_csv(dfile, comment='#')
        ax.plot( data['x'], abs(data['B3']), marker='o', label=dname) #color='b'

        # fill areas between curves and reference value
        #Bnominal = abs(data['B2']).mean()
        #ax.fill_between( data['x'], abs(data['B3']), Bnominal)


    # plot cosmetics: set axis parameters
    #ax.set_xlim(-12000, 12000)
    #ax.set_ylim(0,500)

    # add legend
    if ( legend ):
        ax.legend(loc = 'lower right', prop={'size':12})

    plt.savefig(figname)
    plt.show()

# /// END def comparescans ///


if __name__ == '__main__':

    # Compare SC+FM and SC only at 450 mT 'in front of cloak'
    comparescans( "plots/cloak_mri_450mT_1d.png" ,
                  "Cloaking at 0.45 T (field measured in front of prototype)",
                  ["data-calib/DATA_MegaVIEW/DataFile_2016-12-09_15-06-03.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_12-12-19.csv" ],
                  ["sc",
                   "sc + fm"])

    # Compare SC+FM and SC only at 500 mT 'in front of cloak'
    comparescans( "plots/cloak_mri_500mT_1d.png" ,
                  "Cloaking at 0.50 T (field measured in front of prototype)",
                  ["data-calib/DATA_MegaVIEW/DataFile_2016-12-09_15-42-29.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_13-32-08.csv" ],
                  ["sc",
                   "sc + fm"])

    # Compare SC+FM and SC only at 450 mT 'across cloak'
    comparescans( "plots/cloak_mri_450mT_1d_across.png" ,
                  "Cloaking at 0.45 T (field measured across prototype)",
                  ["data-calib/DATA_MegaVIEW/DataFile_2016-12-09_15-09-34.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_12-42-47.csv" ],
                  ["sc",
                   "sc + fm"],
                  legend = False)

    # Compare SC+FM and SC only at 500 mT 'across cloak'
    comparescans( "plots/cloak_mri_500mT_1d_across.png" ,
                  "Cloaking at 0.50 T (field measured across prototype)",
                  ["data-calib/DATA_MegaVIEW/DataFile_2016-12-09_15-45-26.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_13-43-18.csv" ],
                  ["sc",
                   "sc + fm"],
                  legend = False)

    # Compare SC+FM and SC at different fields, fM = 0.699
    comparescans( "plots/cloak_mri_varB_fm699_1d.png" ,
                  "Cloaking at different fields, fM = 0.699?",
                  ["data-calib/DATA_MegaVIEW/DataFile_2016-12-09_02-03-00.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_02-17-55.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_02-35-01.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_02-54-35.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_03-08-20.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_03-29-01.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_06-06-01.csv"],
                  ["B = 0.10 T",
                   "B = 0.15 T",
                   "B = 0.20 T",
                   "B = 0.25 T",
                   "B = 0.30 T",
                   "B = 0.40 T",
                   "B = 0.50 T"],
                  legend = False)

    # Compare SC+FM and SC at different fields, fM = 0.618
    comparescans( "plots/cloak_mri_varB_fm618_1d.png" ,
                  "Cloaking at different fields, fM = 0.618",
                  ["data-calib/DATA_MegaVIEW/DataFile_2016-12-09_08-15-24.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_08-30-00.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_08-48-06.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_09-05-54.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_09-23-06.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_09-52-35.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_11-47-17.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_12-01-02.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_12-12-19.csv",
                   "data-calib/DATA_MegaVIEW/DataFile_2016-12-09_13-32-08.csv"],
                  ["B = 0.10 T",
                   "B = 0.15 T",
                   "B = 0.20 T",
                   "B = 0.25 T",
                   "B = 0.30 T",
                   "B = 0.35 T",
                   "B = 0.38 T",
                   "B = 0.40 T",
                   "B = 0.45 T",
                   "B = 0.50 T"],
                  legend = False)

