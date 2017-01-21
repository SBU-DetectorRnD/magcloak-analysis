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
figname = "plots/cloak_mri_3d_sc_45L_surf.png"
#figname = "plots/cloak_mri_3d_sc_45L_fm_618_surf.png"

# set up main frame
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.tick_params(labelsize=12)
ax.set_xlabel("x-position (mm)",fontsize=12, labelpad=10)
ax.set_ylabel("z-position (mm)",fontsize=12, labelpad=10)
ax.set_zlabel("$B_{T}$ (mT)",fontsize=12, labelpad=10)
plt.locator_params(axis='y',nbins=6)
plt.locator_params(axis='x',nbins=6)
plt.locator_params(axis='z',nbins=4)

ax.set_xlim3d(-120, 120)
ax.set_ylim3d(20,100)
ax.set_zlim3d(400,455)


# loop over all files in input list
f = open(setlist, "r")
parlines = f.read().splitlines()

X = []
Y = []
Z = []

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

            # calculate mean of every group of five rows
            # (MegaVIEW recorded 5 readings at each position)
            data = data.groupby(np.arange(len(data))//5).mean()

            x = np.array(data['x'].values)
            y = np.array(data['z'].values)
            z = np.array(abs(data['B3'].values))

            # assume the x grind in all files is the same
            if (len(X) == 0):
                X = np.array(data['x'].values)                
            else:
                if ( len(X) != len(x) ):
                    print ("Mismatch in x! Skipping file: ", fname)
                    continue

            # assume all y values in one file are the same
            Y.append( y[0] )

            Z.append(z)

X = np.array(X)
Y = np.array(Y)
Z = np.array(Z)

# scale X from mT to T
#Z /= 1000

print ("X  shape: " , X.shape)
print ("Y  shape: " , Y.shape)

Xm, Ym = np.meshgrid(X.tolist(), Y.tolist())

print ("Xm shape: " , Xm.shape)
print ("Ym shape: " , Ym.shape)
print ("Z  shape: " , Z.shape)

ax.plot_surface(Xm, Ym, Z, rstride=1, cstride=1, alpha=0.7, cmap=cm.jet)
#ax.plot_wireframe(Xm, Ym, Z, rstride=1, cstride=1, alpha=1)

# set view angle
ax.view_init(40, -45)

plt.savefig(figname)
plt.show()
