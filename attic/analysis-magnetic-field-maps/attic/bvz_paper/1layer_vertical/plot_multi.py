import numpy as np
import matplotlib.pyplot as plt

#SC State trial
M = np.genfromtxt('Mean_Set9.txt')
plt.xlabel('z (mm)')
plt.ylabel('B (mT)')
x = M[:,0]
y = M[:,1]
sig_y = M[:,2]
sig_x = .5
plt.errorbar(x,y, sig_y, sig_x, marker = '.')

#SC State model
N = np.genfromtxt('A77_2mT.txt')
x = N[:,0]
y = N[:,1]*5/2
sig_y = M[:,2]
sig_x = .5
plt.errorbar(x,y, sig_y, sig_x, marker = '.')


plt.show()
