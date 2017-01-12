import numpy as np
import matplotlib.pyplot as plt

i_file = 'Mean_Set5.txt'
o_file = 'Mean_Set5.png'
plot_title = 'Set 5'

M = np.genfromtxt(i_file)
x = M[:,0]
y = M[:,1]
sig_y = M[:,2]
sig_x = .5
plt.errorbar(x,y, sig_y, sig_x, marker = '.')
plt.xlabel('z [mm]')
plt.ylabel('B [mT]')
plt.title(plot_title)
plt.savefig(o_file)
plt.close()