#this is for increments of 2
#if different increments wanted, make sure all files have the same number of increments

import numpy as np
import matplotlib.pyplot as plt

plt.xlabel ('z (mm)')
plt.ylabel ('B (mT)')

n = 5.0 #this is number of files

A = np.genfromtxt('DataFile_150629_170842.txt')          #add each file name here in parenthesis
B = np.genfromtxt('DataFile_150629_172229.txt')          #make sure the number of files equals n
C = np.genfromtxt('DataFile_150629_174508.txt')
D = np.genfromtxt('DataFile_150629_180027.txt')
E = np.genfromtxt('DataFile_150629_181359.txt')

x = A[:,0]

a = A[:,1]
b = B[:,1]
c = C[:,1]
d = D[:,1]
e = E[:,1]

y = (a[:] + b[:] + c[:] + d[:] + e[:])/n   #this is the mean value for the files

s1 = (y[:] - a[:])**2                      #for the standard deviation
s2 = (y[:] - b[:])**2
s3 = (y[:] - c[:])**2
s4 = (y[:] - d[:])**2
s5 = (y[:] - e[:])**2

sigy = np.sqrt((1/n)*(s1[:]+s2[:]+s3[:]+s4[:]+s5[:])) #the standard deviation
print len(y)  #number of data
print y[:]    #mean values
print sigy[:] #error values

M = np.column_stack([x,y,sigy])
print(M)
plt.errorbar(x,y,sigy,marker='.')    #graphing with errorbars

plt.show()

np.savetxt('Mean_Set9.txt',M)