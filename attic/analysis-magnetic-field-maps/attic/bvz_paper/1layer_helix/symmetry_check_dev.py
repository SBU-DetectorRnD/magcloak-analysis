import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import matplotlib.gridspec as gridspec


#list of input data file
ifiles = ['m.txt']


#list of the output graphs
ofiles = ['avg_sym_check.png']

#list of the titles you want for each graph
title = ['1 layer']

#where we define 0 to be in our helmholtz setup
readjust =  -0
zero = 0-readjust

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

    #find indeces where x is positive
    positive = np.where(x>0)
    #find indeces where x is negative
    negative = np.where(x<0)

    #plot the positive portion of the graph
    plt.errorbar(x[positive], y[positive], yerr = sig_y[positive], marker = '.', label = 'z > 0')
    #flip over the negative portion of the graph
    plt.errorbar(-x[negative], y[negative], yerr = sig_y[negative],marker = '.', label = 'z < 0')
    plt.xlabel('|z| [mm]')
    plt.ylabel('By [mT]')
    plt.title(title[ind])
#    plt.legend(loc = 'best')
    plt.savefig(ofiles[ind])
