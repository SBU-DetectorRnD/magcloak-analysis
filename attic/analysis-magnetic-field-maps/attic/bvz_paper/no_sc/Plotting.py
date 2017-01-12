import numpy as np
import matplotlib.pyplot as plt

i_file = 'Mean_Set1_5mT.txt'
o_file = 'Graph_mean_5mT.png'
plot_title = 'Set 1, 5mT'

M = np.genfromtxt(i_file)
x = M[:,0]
y = M[:,1]
sig_y = M[:,2]
sig_x = .5
plt.errorbar(x,y, sig_y, sig_x, marker = '.')
plt.xlabel('z [mm]')
plt.ylabel('By [mT]')
plt.title(plot_title)
plt.savefig(o_file)
plt.close()