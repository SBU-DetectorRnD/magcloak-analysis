import os
import sys
#import glob
#import fcm
#import fcm.statistics as stats
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.collections import PolyCollection
from matplotlib.colors import LinearSegmentedColormap

# set up main frame
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
#fig, ax = plt.subplots()

# list of histogram vertizes to create polygons
vertizes = []

# list of z positions of slices
zslices = []

# loop over all files in input list
setlist = "filelist_mri_fieldmap_sc_45L_450mT.txt"

f = open(setlist, "r")
parlines = f.read().splitlines()

# skip all lines that are comments ('#') or don't match required number of parameters
for i, parline in enumerate(parlines):

    if not ( parline.startswith("#") ):
        pars = parline.split()

        if len(pars) != 2:
            print ("Skipping line:")
            print (parline)

        else:
            print (pars)
            # example plot
            fname = pars[0]
            data = pd.read_csv(fname);
            #print (data['z'].head())

            # 2D filled plot
            #Bnominal = np.zeros( len(data['x']))
            #Bnominal+=450
            #ax.plot( data['x'], abs(data['B3']), color='b', marker='o')
            #ax.fill_between( data['x'], abs(data['B3']), Bnominal)

            # 3D filled plot
            a = np.array(data['x'].values)
            b = np.array(abs(data['B3']).values)

            # Add point with nominal B field before first and after last vertex so that
            # filled polygon area shows difference to nominal field
            Bnominal = 450

            a = np.insert( a, 0, a[0] )
            a = np.insert( a, len(a), a[-1]+5 )

            b = np.insert( b, 0, Bnominal )
            b = np.insert( b, len(b), Bnominal )

            # append vertizes to list of vertizes
            vertizes.append( list(zip(a,b)) )

            # append z position of this measurement slice
            zslices.append( data['z'].values[0] )


# set color range
colors = LinearSegmentedColormap('colormap', cm.jet._segmentdata.copy(), len(zslices))

# Prepare 3D polygons
vertizes = np.array( vertizes )
n, p, d = vertizes.shape
maxz = np.max(vertizes[:, 0])

poly = PolyCollection(vertizes, facecolors = [colors(i) for i in range(n)])
poly.set_alpha(0.7)
ax.add_collection3d(poly, zs=zslices, zdir='y')

# set axis parameters
ax.set_xlabel('x-position')
ax.set_xlim3d(-12000, 12000)
ax.set_ylabel('z-position')
ax.set_ylim3d(min(zslices),max(zslices))
ax.set_zlabel('B (mT)')
ax.set_zlim3d(420,455)

plt.savefig('plots/cloak_mri_3d.png')
plt.show()
