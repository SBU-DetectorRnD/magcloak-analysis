import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import matplotlib.gridspec as gridspec


#list of input data file
ifiles = [
        'DataFile_150618_124916_1layer_bvz_5mT.txt',
        'DataFile_150622_134032.txt',
        'DataFile_150622_135510.txt',
        'DataFile_150622_141318.txt',
        'DataFile_150622_144450.txt'
        ]

#list of the output graphs
ofiles = ['bvz.png']

#list of the titles you want for each graph
title = ['B vs z']

#where we define 0 to be in our helmholtz setup
zero = 151

title = '1 layer helix, shielding spatial dependence'

def phys_var(file):
    #turn each input file into a matrix
    M = np.genfromtxt(file)

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
    return (x,y,sig_t)

#loop through each input file
for i in ifiles:
    x,y,sig = phys_var(i)
    plt.errorbar(x, y, yerr = sig, marker = '.')


plt.legend(loc = 'best')
plt.ylabel('B$_{leak}$ [mT]', fontsize = 20)
plt.xlabel('z [mm]', fontsize = 20)
plt.title(title, fontsize = 24)
plt.xlim(-81,81)
plt.ylim(ymin = -0.1)
#plt.ylim(-0.01,10)
plt.savefig('bvz.png')

#x1,y1,sig1 = phys_var(ifiles[0])
#x2,y2,sig2 = phys_var(ifiles[1])
#x3,y3,sig3 = phys_var(ifiles[2])
#x4,y4,sig4 = phys_var(ifiles[3])
#x5,y5,sig5 = phys_var(ifiles[4])
#
#y_avg = np.mean([y1,y2,y3,y4], axis = 0)
#y_std = np.std([y1,y2,y3,y4], axis = 0)
#M = np.column_stack((x1,y_avg,y_std))
##print(M)
#np.savetxt('m.txt', M, fmt='%.5e')
#
#plt.errorbar(x1,y_avg,y_std)
