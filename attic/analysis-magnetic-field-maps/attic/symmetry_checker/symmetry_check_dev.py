import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import matplotlib.gridspec as gridspec


#list of input data file
ifiles = ['data/DataFile_141118_122119.txt']


#list of the output graphs
ofiles = ['room temperature before.png']

#list of the titles you want for each graph
title = ['room temperature before']

#where we define 0 to be in our helmholtz setup
readjust = 3 
zero = 151-readjust

#loop through each input file
for ind in range(0,len(ifiles)):
    #turn each input file into a matrix
    M = np.genfromtxt(ifiles[ind])

    #the first column of the matrix is the position. 
    x = M[:,0]-zero
    #second column is the field
    y = M[:,1]
    #3rd column is the std dev
    sig_y = M[:,2]

    #we say that the std dev in the x direction is 0.5 mm
    sig_x = 0.5*np.ones(sig_y.size)
    
    #calculate total standard deviation of a point from std_x and std_y
    dx = np.gradient(x)
    dy = np.gradient(y)
    sig_t = np.sqrt(sig_y**2 + (sig_x*dy/dx)**2)

    #find indeces where x is positive
    positive = np.where(x>0)
    #find indeces where x is negative
    negative = np.where(x<0)

    #plot the positive portion of the graph
    plt.errorbar(x[positive], y[positive], yerr = sig_t[positive], marker = '.', label = 'z > 0')
    #flip over the negative portion of the graph
    plt.errorbar(-x[negative], y[negative], yerr = sig_t[negative],marker = '.', label = 'z < 0')
    plt.xlabel('|z| [mm]')
    plt.ylabel('By [mT]')
    plt.title(title[ind])
    plt.legend(loc = 'best')
    plt.savefig(ofiles[ind])
