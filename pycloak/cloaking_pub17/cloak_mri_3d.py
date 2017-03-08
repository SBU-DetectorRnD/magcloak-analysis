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

# set plotting style
import mycolors
mcol = mycolors.pub17
plt.style.use("../style_pub17/cloak17_paper.mplstyle")

# Settings:
# Choose input file list
setlist = "filelist_mri_fieldmap_sc_45L_450mT.txt"
#setlist = "filelist_mri_fieldmap_sc_45L_fm_618_450mT.txt"

# Choose output file name for plot
figname1 = "plots/eps/cloak_mri_3d_sc_45L.eps"
figname2 = "plots/png/cloak_mri_3d_sc_45L.png"
#figname1 = "plots/eps/cloak_mri_3d_sc_45L_fm_618.eps"
#figname2 = "plots/png/cloak_mri_3d_sc_45L_fm_618.png"

# center of cloak in z direction in mm. center in x,y is 0,0.
zcenter = 130.14600000000002

# set up main frame
fig = plt.figure( figsize=(6,5) )
ax = fig.add_subplot(111, projection='3d')
#ax.set_title("MRI field map")
ax.set_xlabel("x-position (cm)", labelpad=10)
ax.set_ylabel("z-position (cm)", labelpad=10)
ax.set_zlabel("$B_{T}$ (T)", labelpad=10)
plt.locator_params(axis='y',nbins=6)
plt.locator_params(axis='x',nbins=6)
plt.locator_params(axis='z',nbins=4)

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

            a /= 10
            b /= 1000

            # Find nominal B field
            Bnominal = data['B2'].mean()/1000
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
            zslices.append( (data['z'].values[0] - zcenter)/10 )
            print(data['z'].values[0])

# Add cylinder to indicate cloak position
# basic cylinder
x=np.linspace(-1, 1, 100)
z=np.linspace(420, 455, 100)
Xc, Zc=np.meshgrid(x, z)
Yc = np.sqrt(1-Xc**2)

# scale cylinder dimensions
cloakdiameter = 40.64 # 40.64 mm = 1.6 inch
cloakcenterz = 0
Xc*=cloakdiameter/2.
Yc*=cloakdiameter/2.
Ycm = -1*Yc
Yc += 127
Ycm += 127

# draw cylinder
rstride = 20
cstride = 10
#ax.plot_surface(Xc, Yc, Zc, alpha=0.5, rstride=rstride, cstride=cstride, color='grey')
#ax.plot_surface(Xc, Ycm, Zc, alpha=0.5, rstride=rstride, cstride=cstride, color='grey')
#ax.plot_wireframe(Xc, Yc, Zc, alpha=0.5, rstride=rstride, cstride=cstride, color='grey')
#ax.plot_wireframe(Xc, Ycm, Zc, alpha=0.5, rstride=rstride, cstride=cstride, color='grey')

# set color range
colors = LinearSegmentedColormap('colormap', cm.jet._segmentdata.copy(), len(zslices))

# Prepare 3D polygons
#poly = PolyCollection(vertizes, edgecolors='black', linewidths='1', facecolors = [colors(len(zslices)-1-i) for i in range(len(zslices))])
poly = PolyCollection(vertizes, edgecolors='black', linewidths='1', facecolors = [mcol[i] for i in range(len(zslices))])
poly.set_alpha(0.7)
ax.add_collection3d(poly, zs=zslices, zdir='y')

# set view angle
#ax.view_init(20, -45)
ax.view_init(20, 45)

# set axis parameters
#ax.set_xlim3d(-12.0, 12.0)
ax.set_xlim3d(-14.0, 14.0)
#ax.set_ylim3d(2.0,10.0)
ax.set_ylim3d(-11.0,-3.0)
ax.set_zlim3d(0.375,0.455)

plt.savefig(figname1)
plt.savefig(figname2)
plt.show()
