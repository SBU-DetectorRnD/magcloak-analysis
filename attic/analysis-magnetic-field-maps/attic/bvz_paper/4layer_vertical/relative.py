#this is for increments of 2
#if different increments wanted, make sure all files have the same number of increments

import numpy as np
import matplotlib.pyplot as plt

plt.xlabel ('z (mm)')
plt.ylabel ('Percent Error')

n = 5.0 #this is number of files

A = np.genfromtxt('DataFile_150629_123455.txt')          #add each file name here in parenthesis
B = np.genfromtxt('DataFile_150629_124804.txt')          #make sure the number of files equals n
C = np.genfromtxt('DataFile_150629_125906.txt')
D = np.genfromtxt('DataFile_150629_131517.txt')
E = np.genfromtxt('DataFile_150629_132600.txt')

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

rely = (sigy[:]/y[:])*100                 #the percent error


plt.plot(x,rely,'.')    #scatter plot of relative errors

plt.show()
