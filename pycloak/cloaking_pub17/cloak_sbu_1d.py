import os
import sys

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from matplotlib import cm

# set plotting style
import mycolors
mcol = mycolors.pub17
mmark = mycolors.m_styles
fcol = mycolors.f_colors
plt.style.use("../style_pub17/cloak17_paper.mplstyle")

def comparescans( figname, figtitle, datafiles, datanames, legend=True ):

    xcenter=141

    if ( len( datafiles ) != len( datanames ) ):
        print ("ERROR: Mismatch length datafiles and datanames. Exit.")
        sys.exit(1)

    # create figure
    fig, ax = plt.subplots( figsize=(6,5) )

    ax.set_yticks([38.5,39.0,39.5,40.0,40.5,41,41.5,42.0])
    ax.set_yticklabels(['',39.0,'',40.0,'',41,'',42.0])
    ax.set_ylim((38.5,42.0))

    #ax.set_xticks([-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10])
    #ax.set_xticklabels([-10,'','','','',-5,'','','','',0,'','','','',5,'','','','',10])
    ax.set_xticks([-10,-7.5,-5,-2.5,0,2.5,5,7.5,10])
    ax.set_xticklabels([-10,'',-5,'',0,'',5,'',10])
    ax.set_xlim((-10.0,10.0))

    # mark cloak dimensions                                                                                                             
    plt.axvline(-2.032, color=mcol[2], linestyle='-',alpha=0.5)
    plt.axvline(2.032, color=mcol[2], linestyle='-',alpha=0.5)

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

        ax.plot( (data['pos']-xcenter)/10, abs(data['B1']), marker=mmark[2-i], markerfacecolor=fcol[2-i], color=mcol[2-i], label=dname) #color='b'

        # fill areas between curves and reference value
        #Bnominal = abs(data['B2']).mean()
        #ax.fill_between( data['x'], abs(data['B3']), Bnominal)


    # plot cosmetics: set axis parameters
    #    ax.set_title(figtitle)
    ax.set_xlabel('position (cm)')
    #ax.set_xlim(-12000, 12000)
    ax.set_ylabel('$B_T$ (mT)')
    #ax.set_ylim(0,500)
    #ax.set_yscale("log", nonposy='clip')

    # add legend
    if ( legend ):
        ax.legend(loc = 'lower center')

    plt.savefig("plots/eps/"+figname+".eps")
    plt.savefig("plots/png/"+figname+".png")
    plt.show()

# /// END def comparescans ///


if __name__ == '__main__':

#    # Compare SC+FM and SC only at 450 mT
#    comparescans( "cloak_sbu_test" ,
#                  "Cloaking at 0.45 T (field measured in front of prototype)",
#                  ["data-calib/DATA_Gaussmeter/DataFile_161009_174739.csv",
#                   "data-calib/DATA_Gaussmeter/DataFile_161009_192152.csv",
#                   "data-calib/DATA_Gaussmeter/DataFile_161009_155547.csv"],
#                  ["fm = 0.548",
#                   "fm = 0.574",
#                   "reference"])
#
#
#    # Compare SC+FM and SC only at 450 mT
#    comparescans( "cloak_sbu_test_across" ,
#                  "Cloaking at 0.45 T (field measured in front of prototype)",
#                  ["data-calib/DATA_Gaussmeter/DataFile_161009_180155.csv",
#                   "data-calib/DATA_Gaussmeter/DataFile_161009_193724.csv",
#                   "data-calib/DATA_Gaussmeter/DataFile_161020_204436.csv"],
#                  ["fm = 0.548",
#                   "fm = 0.574",
#                   "reference"])


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
    comparescans( "cloak_sbu_compare_sc_1" ,
                  "Cloaking at 40 mT (field measured 1 cm in front of prototype)",
                  ["data-calib/DATA_Gaussmeter/DataFile_170213_205815.csv",
                   "data-calib/DATA_Gaussmeter/DataFile_170213_201438.csv",
                   "data-calib/DATA_Gaussmeter/DataFile_170214_210939.csv"],
                  ["reference",
                   " 4-layer SC + $\mu_r$ = 2.43",
                   "45-layer SC + $\mu_r$ = 2.43"])
