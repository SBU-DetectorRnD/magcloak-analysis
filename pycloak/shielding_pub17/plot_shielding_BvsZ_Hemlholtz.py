import os
import sys

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from matplotlib import cm

# set plotting style                                                                                                                    
import mycolors
mcol = mycolors.pub17
plt.style.use("../style_pub17/cloak17_paper.mplstyle")

def comparescans( datafiles, datanames, legend=True ):

    if ( len( datafiles ) != len( datanames ) ):
        print ("ERROR: Mismatch length datafiles and datanames. Exit.")
        sys.exit(1)

    # create figure
    fig, ax = plt.subplots( figsize=(6,5) )

    # mark cloak dimensions
    plt.axvline(-5.715, color=mcol[2], linestyle='-',alpha=0.5)
    plt.axvline(5.715, color=mcol[2], linestyle='-',alpha=0.5)
    plt.axhline(0.1, color=mcol[2], linestyle='-',alpha=0.5)

    # zcenter is at 71 mm (see elog https://mocha.physics.sunysb.edu:8080/magcloak/1084)
    zcenter = 71

    # second: plot field map data
    for i, (dfile, dname) in enumerate( zip( datafiles, datanames ) ):
        print( dfile, dname )
        data = pd.read_csv(dfile, comment='#')
        ax.plot( (data['pos'] - 71)/10, abs(data['B1'])+0.1, marker='o', label=dname, color=mcol[2-i])

    # plot cosmetics: set axis parameters
    # ax.set_title(figtitle)
    ax.set_xlabel('position (cm)')
    #ax.set_xlim(-12000, 12000)
    ax.set_ylabel('$B_T$ (mT) + 0.1 mT')
    ax.set_ylim(0.08,50)
    ax.set_yscale("log", nonposy='clip')

    # add legend
    # ax.legend(loc = 'upper right')
    ax.text(-4, 25, 'no SC shield',color=mcol[2])
    ax.text(-4, 5, '4-layer SC',color=mcol[1])
    ax.text(-12, 5, '45-layer SC',color=mcol[0])

    plt.savefig("plots/eps/shield_BvZ_Helmholtz.eps")
    plt.savefig("plots/png/shield_BvZ_Helmholtz.png")
    plt.show()

# /// END def comparescans ///


if __name__ == '__main__':


    # Compare SC4 + SC45 BvZ
    comparescans( ["data-calib/DATA_Gaussmeter/DataFile_170214_201515.csv",
                   "data-calib/DATA_Gaussmeter/DataFile_170215_181018.csv",
                   "data-calib/DATA_Gaussmeter/DataFile_170215_184713.csv"],
                  ["no SC shield",
                   " 4-layer SC",
                   "45-layer SC"])


                  
