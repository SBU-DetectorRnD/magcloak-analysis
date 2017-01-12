import numpy as np
import sys
import os
import matplotlib.pyplot as plt

if len(sys.argv) != 3:
    sys.exit("Bad filename input. Abort!")

summary_list = sys.argv[1]
combined_summary = sys.argv[2]

if not os.path.isfile(summary_list):
    sys.exit("Summary file list not found. Abort!")

with open(summary_list) as f:
    summary_file = f.read().splitlines()

M = np.genfromtxt(summary_file[0])

for ind in range(1,len(summary_file)):
    M_new = np.genfromtxt(summary_file[ind])
    M = np.concatenate((M,M_new), axis=0)

M = M[M[:,0].argsort()]
np.savetxt(combined_summary, M, fmt = '%.5f', delimiter = '\t',
        header ='Bo\tsig_Bo\tBi\tsig_bi\tshield\tsig_shield\tsf\tsig_sf\ttime_dependent')

plt.errorbar(M[:,0], M[:,2], M[:,3])
plt.title('Field Leaking through Superconducting Tubes', fontsize=20)
plt.ylabel('Bi [mT]', fontsize=20)
plt.xlabel('z [mm]', fontsize=20)
plt.tight_layout()
plt.savefig(combined_summary[:-4]+'.png')
plt.close()




