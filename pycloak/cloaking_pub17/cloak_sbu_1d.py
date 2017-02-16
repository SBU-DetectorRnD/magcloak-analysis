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
    fig, ax = plt.subplots( figsize=(6,5) )

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
        ax.plot( data['pos'], abs(data['B1']), marker='.', label=dname) #color='b'

        # fill areas between curves and reference value
        #Bnominal = abs(data['B2']).mean()
        #ax.fill_between( data['x'], abs(data['B3']), Bnominal)


    # plot cosmetics: set axis parameters
    ax.set_title(figtitle)
    ax.set_xlabel('x-position (mm)')
    #ax.set_xlim(-12000, 12000)
    ax.set_ylabel('B (mT)')
    #ax.set_ylim(0,500)
    ax.set_yscale("log", nonposy='clip')

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


    # 45-layer SC and FM=0.618 ferromagnet, 40 mT
    # BvX, 1mm from edge of cloak (y=101mm): DataFile_170214_205838.txt
    # BvX, 1cm from edge of cloak (y=110mm): DataFile_170214_210939.txt

    # 45-layer SC and FM=0.554 ferromagnet, 40 mT
    #...

    # 4-layer SC and FM=0.618 ferromagnet, 40 mT
    # BvX, 1 mm away from edge @ z-center: DataFile_170213_194029.txt
    # BvX, 1 cm away from edge @ z-center: DataFile_170213_201438.txt

    # BvX reference 1mm away from edge @ z-center: DataFile_170213_204446.txt
    # BvX reference 1cm away from edge @ z-center: DataFile_170213_205815.txt

    # Compare SC4+FM618, SC45+FM618 at 40 mT
    comparescans( "plots/cloak_sbu_compare_sc_1.png" ,
                  "Cloaking at 40 mT (field measured 1 cm in front of prototype)",
                  ["data-calib/DATA_Gaussmeter/DataFile_170213_201438.csv",
                   "data-calib/DATA_Gaussmeter/DataFile_170214_210939.csv",
#                   "data-calib/DATA_Gaussmeter/DataFile_170203_204341.csv",
                   "data-calib/DATA_Gaussmeter/DataFile_170213_205815.csv"],
                  [" 4-layer SC + fm618",
                   "45-layer SC + fm618",
#                   "fm = SC45 + fm574",
                   "reference"])



    # Compare SC4 + SC45 BvZ
    comparescans( "plots/cloak_sbu_compare_sc_2.png" ,
                  "Shielding at 40 mT (field measured in center of SC)",
                  ["data-calib/DATA_Gaussmeter/DataFile_170215_181018.csv",
                   "data-calib/DATA_Gaussmeter/DataFile_170215_184713.csv",
                   "data-calib/DATA_Gaussmeter/DataFile_170214_201515.csv"],
                  [" 4-layer SC",
                   "45-layer SC",
                   "no SC shield"])

                  
