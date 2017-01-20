import numpy as np
import matplotlib.pyplot as plt
from uncertainties import unumpy
from uncertainties import ufloat
import sys

fm_length = 111.7 
zero = [163, 163]

fm_profile_graph = 'fv30_b_vs_z.png'
plot_title = 'Epoxy/Steel, fv = 0.30, Bo = 27 mT'

profile = ['DataFile_150506_171251_bvz_25mt.txt',
            'DataFile_150618_110552_f30_bvz.txt',
            ]

accidental_b_offset = [0,0,0] 
accidental_z_offset = [0,0,0] 
scaling = [27/25,1,1]

description = ['Applied Field', 'fv = 0.3']

def plot_prof(profile, description, accidental_b_offset, zero, scaling):
    M = np.genfromtxt(profile)
    plt.errorbar(M[:,0] - zero , (M[:,1] - accidental_b_offset)*scaling, M[:,2], 
            marker ='o', label = description)


for i in range(0,len(description)):
    plot_prof(profile[i], description[i], accidental_b_offset[i],
            zero[i], scaling[i])

plt.xlabel('z [mm]')
plt.ylabel('B [mT]')
plt.title(plot_title)
plt.legend(loc = 'best')
plt.vlines(-fm_length/2, 22.5,27.3, linestyles = 'dashed', linewidth = 3)
plt.vlines(fm_length/2, 22.5, 27.3, linestyles = 'dashed', linewidth = 3)
plt.savefig(fm_profile_graph) 
plt.close()
