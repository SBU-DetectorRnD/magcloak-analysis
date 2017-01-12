import numpy as np
import matplotlib.pyplot as plt

results_file = 'results.txt'

M = np.genfromtxt(results_file)
b = M[:,0]
sig_b = M[:,1]
i = M[:,2]
sig_i = M[:,3]
ratio = M[:,4]
sig_ratio = M[:,5]

#plt.errorbar(i,b,xerr = sig_i, yerr = sig_b, fmt ='.')
plt.plot(b,sig_b,  '.')
plt.xlabel('Applied Field [mT]')
plt.ylabel('$\Delta$B [mT]')
plt.title('Field Measurement Stability')
plt.savefig('field_stability_summary.png')
plt.close()

plt.plot(b,sig_i,  '.')
plt.xlabel('Applied Field [mT]')
plt.ylabel('$\Delta$I [A]')
plt.title('Current Stability')
plt.savefig('current_stability_summary.png')
plt.close()

print(ratio)
print(sig_ratio)
plt.plot(b,sig_ratio,  '.')
plt.xlabel('Applied Field [mT]')
plt.ylabel('$\Delta$B/I [mT/A]')
plt.title('Ratio Stability')
plt.savefig('ratio_stability_summary.png')
plt.close()
