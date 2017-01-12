import numpy as np
import matplotlib.pyplot as plt

#Vertical Lines marking FM layer region (4)
plt.plot((-31.750/2.0,-31.750/2.0),(0,12),'k--')
plt.plot(( 31.750/2.0, 31.750/2.0),(0,12),'k--')
plt.plot((-39.116/2.0,-39.116/2.0),(0,12),'k--')
plt.plot(( 39.116/2.0, 39.116/2.0),(0,12),'k--')

# center of cylinder in x - different for different setups
xc_1 = 48
xc_2 = 48+5.5

# set axis range
plt.axis([-100, 100, 0, 12])

L = np.genfromtxt('data/DataFile_150707_151753.txt')
plt.tick_params(labelsize=20)
plt.xlabel('x (mm)', fontsize = 20)
plt.ylabel('By (mT)', fontsize = 20)
x = L[:,0]
y = abs(L[:,1])
sig_y = L[:,2]
plt.errorbar(x-xc_1,y, sig_y, marker = '.', label = 'Reference Field',fmt='o')

R = np.genfromtxt('data/DataFile_150707_170047.txt')
x = R[:,0]
y = abs(R[:,1])
sig_y = R[:,2]
plt.errorbar(x-xc_1,y, sig_y, marker = '.', label = 'SC',fmt='o')

N = np.genfromtxt('data/DataFile_150707_230334.txt')
x = N[:,0]
y = abs(N[:,1])
sig_y = N[:,2]
plt.errorbar(x-xc_2,y, sig_y, marker = '.', label = 'FM',fmt='o')

O = np.genfromtxt('data/DataFile_150708_020249.txt')
x = O[:,0]
y = abs(O[:,1])
sig_y = O[:,2]
plt.errorbar(x-xc_2,y, sig_y, marker = '.', label = 'SC+FM',fmt='o')

plt.legend(loc = 'best')

plt.tight_layout()
plt.savefig('plots/Graph_BvX_Tangent')
plt.close()

#plt.show()
