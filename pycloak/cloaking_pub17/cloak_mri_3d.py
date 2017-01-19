import os
import sys

import numpy as np
import pandas as pd

from matplotlib import cm

from mpl_toolkits.mplot3d import Axes3D

from matplotlib.collections import PolyCollection
import matplotlib.pyplot as plt
from matplotlib import colors as mcolors

from matplotlib.colors import LinearSegmentedColormap

# Settings:
# Choose input file list
setlist = "filelist_mri_fieldmap_sc_45L_450mT.txt"
#setlist = "filelist_mri_fieldmap_sc_45L_fm_618_450mT.txt"

# Choose output file name for plot
figname = "plots/cloak_mri_3d_sc_45L.png"
#figname = "plots/cloak_mri_3d_sc_45L_fm_618.png"

# set up main frame
fig = plt.figure()
ax = fig.gca(projection='3d')

# function to choose color
def cc(arg):
    return mcolors.to_rgba(arg, alpha=0.6)

# list of histogram vertizes to create polygons
vertizes = []

# list of z positions of slices
zslices = []

# loop over all files in input list
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

            fname = pars[0]
            data = pd.read_csv(fname);

            # 3D filled plot
            a = np.array(data['x'].values)
            b = np.array(abs(data['B3']).values)

            # Find nominal B field
            Bnominal = data['B2'].mean()
            print (Bnominal)

            # Add point with nominal B field before first and after last vertex so that
            # filled polygon area shows difference to nominal field
            a = np.insert( a, 0, a[0] )
            a = np.insert( a, len(a), a[-1]+5 )

            b = np.insert( b, 0, Bnominal )
            b = np.insert( b, len(b), Bnominal )

            # append vertizes to list of vertizes
            vertizes.append( list(zip(a,b)) )

            # append z position of this measurement slice
            zslices.append( data['z'].values[0] )
            print(data['z'].values[0])


# set color range
colors = LinearSegmentedColormap('colormap', cm.jet._segmentdata.copy(), len(zslices))

# Prepare 3D polygons
poly = PolyCollection(vertizes, edgecolors='black', linewidths='1', facecolors = [colors(len(zslices)-1-i) for i in range(len(zslices))])
poly.set_alpha(0.5)
ax.add_collection3d(poly, zs=zslices, zdir='y')

# Add cylinder to indicate cloak position
# Cylinder
x=np.linspace(-1, 1, 100)
z=np.linspace(420, 460, 100)
Xc, Zc=np.meshgrid(x, z)
Yc = np.sqrt(1-Xc**2)

# scale dimensions
Xc*=2000
Yc*=500
Ycm = -1*Yc
Yc += 6540
Ycm += 6540

# Draw parameters
rstride = 20
cstride = 10
ax.plot_surface(Xc, Yc, Zc, alpha=0.5, rstride=rstride, cstride=cstride, color='grey')
ax.plot_surface(Xc, Ycm, Zc, alpha=0.5, rstride=rstride, cstride=cstride, color='grey')


# set view angle
ax.view_init(20, -45)

# set axis parameters
ax.set_xlabel('x-position')
ax.set_xlim3d(-12000, 12000)
ax.set_ylabel('z-position')
ax.set_ylim3d(min(zslices),max(zslices))
ax.set_zlabel('B (mT)')
ax.set_zlim3d(420,455)

plt.savefig(figname)
plt.show()
