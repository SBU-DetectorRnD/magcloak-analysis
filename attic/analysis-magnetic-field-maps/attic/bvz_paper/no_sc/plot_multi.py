import numpy as np
import matplotlib.pyplot as plt

#SC State trial
M = np.genfromtxt('DataFile_150512_161003_3layer.txt')
plt.xlabel('z (mm)')
plt.ylabel('B (mT)')
x = M[:,0]
y = M[:,1] - 0.034
sig_y = M[:,2]
sig_x = .5
plt.errorbar(x,y, sig_y, sig_x, marker = '.')

#SC State model
N = np.genfromtxt('A77_9.5mT.txt')
x = N[:,0]
y = N[:,1]*8/9.5
sig_y = M[:,2]
sig_x = .5
plt.errorbar(x,y, sig_y, sig_x, marker = '.')


plt.show()
