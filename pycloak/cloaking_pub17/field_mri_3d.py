import os
import sys

import numpy as np
import pandas as pd

from matplotlib import cm

from mpl_toolkits.mplot3d import Axes3D

from matplotlib.collections import PolyCollection
import matplotlib.pyplot as plt
import matplotlib as mpl

from matplotlib import colors as mcolors
from matplotlib import cm 

from matplotlib.colors import LinearSegmentedColormap

# get data
data = pd.read_csv("data-calib/DATA_MegaVIEW/DataFile_2016-12-09_18-23-53.csv")

# process data to make it work for surface / wireframe plot
XB = np.unique(data['x'].values)
YB = np.unique(data['z'].values)

# create meshgrid X and Y from XB, YB
X, Y = np.meshgrid(XB,YB)

# calculate mean of every group of five rows
# (MegaVIEW recorded 5 readings at each position)
data_mean = data.groupby(np.arange(len(data))//5).mean()

# Get B-field column
ZB = abs(data_mean['B1'].values)

# reshape ZB to match 2-d dimensions of X and Y
Z = ZB.reshape(len(YB),len(XB))


##### Plot 1: Scatter

# set up main frame
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# plot data
ax.scatter(data['x'],data['z'],abs(data['B1']), marker='o')

# set view angle
ax.view_init(50, 45)

## set axis parameters
ax.set_xlabel('x-position')
##ax.set_xlim3d(-120, 120)
ax.set_ylabel('z-position')
##ax.set_ylim3d(-50,190)
ax.set_zlabel('B (mT)')
##ax.set_zlim3d(420,455)

plt.savefig("plots/mri_fieldmap_empty_1400mT.png")
plt.show()


##### Plot 2: Surface / Wireframe

# set up main frame
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

#ax.plot_wireframe(X, Y, Z, rstride=20, cstride=20, alpha=1.0)
ax.plot_surface(X, Y, Z, cmap=cm.jet, rstride=1, cstride=1)

# set view angle
ax.view_init(50, 45)

## set axis parameters
ax.set_xlabel('x-position')
##ax.set_xlim3d(-120, 120)
ax.set_ylabel('z-position')
##ax.set_ylim3d(-50,190)
ax.set_zlabel('B (mT)')
##ax.set_zlim3d(420,455)

plt.savefig("plots/mri_fieldmap_empty_1400mT_surf.png")
plt.show()
