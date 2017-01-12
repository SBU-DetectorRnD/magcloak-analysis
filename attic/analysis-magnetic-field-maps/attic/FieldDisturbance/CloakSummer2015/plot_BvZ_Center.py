import numpy as np
import matplotlib.pyplot as plt

#Vertical Lines (2)
plt.plot((-58,-58),(0.0,12.0),'k--')
plt.plot((58,58),(0.0,12.0),'k--')

plt.axis([-80,80,0,12])

center = 153
L = np.genfromtxt('data/DataFile_150707_142907.txt')
plt.tick_params(labelsize=20)
plt.xlabel('Z (mm)', fontsize = 20)
plt.ylabel('By (mT)', fontsize = 20)
x = L[:,0] - center
y = abs(L[:,1])
sig_y = L[:,2]
plt.errorbar(x,y, sig_y, marker = '.', label = 'Reference Field',fmt='o')

R = np.genfromtxt('data/DataFile_150707_161240.txt')
x = R[:,0] - center
y = abs(R[:,1])
sig_y = R[:,2]
plt.errorbar(x,y, sig_y, marker = '.', label = 'SC',fmt='o')

N = np.genfromtxt('data/DataFile_150707_213349.txt')
x = N[:,0] - center
y = abs(N[:,1])
sig_y = N[:,2]
plt.errorbar(x,y, sig_y, marker = '.', label = 'FM',fmt='o')

O = np.genfromtxt('data/DataFile_150708_010122.txt')
x = O[:,0] - center
y = abs(O[:,1])
sig_y = O[:,2]
plt.errorbar(x,y, sig_y, marker = '.', label = 'SC+FM',fmt='o')

plt.legend(loc = 'best')
plt.tight_layout()

plt.savefig('plots/Graph_BvZ_Center')
