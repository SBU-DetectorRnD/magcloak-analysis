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

    # create figure
    fig, ax = plt.subplots()

#    # first: plot lines for nominal fields
#    for i, (dfile, dname) in enumerate( zip( datafiles, datanames ) ):
#        print( dfile, dname )
#        data = pd.read_csv(dfile, comment='#')
#        
#        ax.axhline( abs(data['B2']).mean() , color='grey' , linestyle='--')
        
    # second: plot field map data
    for i, (dfile, dname) in enumerate( zip( datafiles, datanames ) ):
        print( dfile, dname )
        data = pd.read_csv(dfile, comment='#')
        ax.plot( data['pos'], abs(data['B1']), marker='o', label=dname) #color='b'

        # fill areas between curves and reference value
        #Bnominal = abs(data['B2']).mean()
        #ax.fill_between( data['x'], abs(data['B3']), Bnominal)


    # plot cosmetics: set axis parameters
    ax.set_title(figtitle)
    ax.set_xlabel('x-position (mm)')
    #ax.set_xlim(-12000, 12000)
    ax.set_ylabel('B (mT)')
    #ax.set_ylim(0,500)

    # add legend
    if ( legend ):
        ax.legend(loc = 'lower right')

    plt.savefig(figname)
    plt.show()

# /// END def comparescans ///


if __name__ == '__main__':

    # Compare SC+FM and SC only at 450 mT
    comparescans( "plots/cloak_sbu_test.png" ,
                  "Cloaking at 0.45 T (field measured in front of prototype)",
                  ["data-calib/DATA_Gaussmeter/DataFile_161009_174739.csv",
                   "data-calib/DATA_Gaussmeter/DataFile_161009_192152.csv",
                   "data-calib/DATA_Gaussmeter/DataFile_161009_155547.csv"],
                  ["fm = 0.548",
                   "fm = 0.574",
                   "reference"])


    # Compare SC+FM and SC only at 450 mT
    comparescans( "plots/cloak_sbu_test_across.png" ,
                  "Cloaking at 0.45 T (field measured in front of prototype)",
                  ["data-calib/DATA_Gaussmeter/DataFile_161009_180155.csv",
                   "data-calib/DATA_Gaussmeter/DataFile_161009_193724.csv",
                   "data-calib/DATA_Gaussmeter/DataFile_161020_204436.csv"],
                  ["fm = 0.548",
                   "fm = 0.574",
                   "reference"])

                  
