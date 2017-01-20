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

from matplotlib.colors import LinearSegmentedColormap

# Settings:

# set up main frame
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# get data
data = pd.read_csv("data-calib/DATA_MegaVIEW/DataFile_2016-12-09_18-23-53.csv")

# plot data
ax.scatter(data['x'],data['z'],abs(data['B1']), marker='o')

# set view angle
ax.view_init(50, -45)

## set axis parameters
ax.set_xlabel('x-position')
##ax.set_xlim3d(-120, 120)
ax.set_ylabel('z-position')
##ax.set_ylim3d(-50,190)
ax.set_zlabel('B (mT)')
##ax.set_zlim3d(420,455)

plt.savefig("plots/mri_fieldmap_empty_1400mT.png")
plt.show()




###






n = 100000
x = np.random.standard_normal(n)
y = 2.0 + 3.0 * x + 4.0 * np.random.standard_normal(n)
plt.hexbin(x,y)

plt.show()



XB = np.linspace(-1,1,20)
YB = np.linspace(-1,1,20)
X,Y = np.meshgrid(XB,YB)
Z = np.exp(-(X**2+Y**2))
plt.imshow(Z,interpolation='none')

plt.show()

####
#xedges = [0, 1, 3, 5]
#yedges = [0, 2, 3, 4, 6]

#x = np.random.normal(2, 1, 100)
#y = np.random.normal(1, 1, 100)


#H, xedges, yedges = np.histogram2d(x, y, bins=(xedges, yedges))
#H = H.T  # Let each row list bins with common y range.

#xedges = np.unique(data['x'].values)
#yedges = np.unique(data['z'].values)
#
#xedges = xedges.T
#yedges = yedges.T

#xedges = [-123.6, -113.3, -103.0, -92.7, 113.3]
#xedges = [-123.6, -113.3, -103.0, -92.7, -82.4, -72.1, -61.8, -51.5, -41.2, -30.9,
#           -25.75, -20.6, -15.45, -10.3, -5.15, 0., 5.15, 10.3, 15.45, 20.6, 25.75,
#           30.9, 41.2, 51.5, 61.8, 72.1, 82.4, 92.7, 103.0, 113.3]
#yedges = [27.3625, 47.2625, 67.1625, 218.9]

#xedges = range(-125,125,5)
#yedges = range(25,225,10)
#
#print(xedges)
#print(yedges)

##H, xedges, yedges = np.histogram2d(data['x'], data['z'], bins=(xedges,yedges), weights=abs(data['B1']))
#H, xedges, yedges = np.histogram2d(data['x'], data['z'], range=[[-125, 125], [25, 225]], bins=[30,30], weights=abs(data['B1']))
#
#
#fig = plt.figure(figsize=(7, 3))
#
#
#ax = fig.add_subplot(131, title='imshow: square bins')
#plt.imshow(H, interpolation='nearest', origin='low',
#           extent=[xedges[0], xedges[-1], yedges[0], yedges[-1]])
#
#
#ax = fig.add_subplot(132, title='pcolormesh: actual edges',
#                     aspect='equal')
#X, Y = np.meshgrid(xedges, yedges)
#ax.pcolormesh(X, Y, H)
#
#
#ax = fig.add_subplot(133, title='NonUniformImage: interpolated',
#        aspect='equal', xlim=xedges[[0, -1]], ylim=yedges[[0, -1]])
#im = mpl.image.NonUniformImage(ax, interpolation='bilinear')
#xcenters = (xedges[:-1] + xedges[1:]) / 2
#ycenters = (yedges[:-1] + yedges[1:]) / 2
#im.set_data(xcenters, ycenters, H)
#ax.images.append(im)
#plt.show()
