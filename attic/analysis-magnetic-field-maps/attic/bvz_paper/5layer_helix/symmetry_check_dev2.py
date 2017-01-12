import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import matplotlib.gridspec as gridspec


#list of input data file
ifiles = [ 'Mean_Set2.txt',
            ]
#ifiles = ['.txt']


#list of the output graphs
ofiles = 'Graph_symmetry_Set2.png'

#list of the titles you want for each graph
title = 'Set 2'

#where we define 0 to be in our helmholtz setup
readjust = -2
zero = 151-readjust

legends = ['77K']

#loop through each input file
for ind in range(0,len(ifiles)):
    #turn each input file into a matrix
    print(ifiles[ind])
    M = np.genfromtxt(ifiles[ind])

    #the first column of the matrix is the position. 
    x = M[:,0]-zero
    #second column is the field
    y = M[:,1]
    ind_center = np.where(x==0)
    y_center = y[ind_center]
    print(y_center)
    y_center=1
    y = y/y_center
    #3rd column is the std dev
    sig_y = M[:,2]/y_center

    #we say that the std dev in the x direction is 0.5 mm
    sig_x = 0.1*np.ones(sig_y.size)
    
    #calculate total standard deviation of a point from std_x and std_y
    dx = np.gradient(x)
    dy = np.gradient(y)
    sig_t = np.sqrt(sig_y**2 + (sig_x*dy/dx)**2)

    #find indeces where x is positive
    positive = np.where(x>0)
    #find indeces where x is negative
    negative = np.where(x<0)

    #plot the positive portion of the graph
    plt.errorbar(x[positive], y[positive], yerr = sig_t[positive], marker =
            '.', label = 'z > 0, B =' + legends[ind])
    #flip over the negative portion of the graph
    plt.errorbar(-x[negative], y[negative], yerr = sig_t[negative],marker =
            '.', label = 'z < 0, B = ' + legends[ind])

plt.xlabel('|z| [mm]')
plt.ylabel('B [mT]')
plt.title(title)
plt.legend(loc = 'best')
plt.savefig(ofiles)
