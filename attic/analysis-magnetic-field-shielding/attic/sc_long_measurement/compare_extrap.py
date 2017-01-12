import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from uncertainties import unumpy
from uncertainties import ufloat

M = np.genfromtxt('results_log_24h.txt')
N = np.genfromtxt('results_power_24h.txt')

x = M[:-1,0]
log_1day = M[:-1,1]
sig_log_1day = M[:-1,2]
log_1year = M[:-1,3]
sig_log_1year = M[:-1,4]
chi2red_log = M[:-1,5]

pow_1day = N[:-1,1]
sig_pow_1day = N[:-1,2]
pow_1year = N[:-1,3]
sig_pow_1year = N[:-1,4]
chi2red_pow = N[:-1,5]

extrap_1day = np.mean([log_1day[-1], pow_1day[-1]])

gs = gridspec.GridSpec(2,1, height_ratios = [4,1])
ax1 = plt.subplot(gs[0])
plt.errorbar(x, log_1day, sig_log_1day, fmt='.', 
        label = '1 day log fit')
plt.errorbar(x, pow_1day, sig_pow_1day, fmt='.', 
        label = '1 day power fit')
plt.errorbar(x, log_1year, sig_log_1year, fmt='.', 
        label = '1/2 year log fit')
plt.errorbar(x, pow_1year, sig_pow_1year, fmt='.', 
        label = '1/2 year power fit')
plt.hlines(extrap_1day, 90, 600)
plt.legend(loc = 'best')
plt.ylabel('$B_{leak}$ [mT]')
plt.title('Extrapolated Values, 1 layer SC, B0 = 34.3 mT')
#plt.semilogx()
plt.grid()

ax2 = plt.subplot(gs[1])
plt.plot(x,chi2red_log, '.')
plt.plot(x,chi2red_pow, '.')
plt.ylabel('$\chi_r^2$')
plt.xlabel('Run time [s]')
#plt.semilogx()
#plt.semilogy()
plt.grid()
plt.savefig('extrapolation_comparison_linx.png')
plt.close()

########################################33
bleak_day_min = log_1day-sig_log_1day
bleak_day_max = pow_1day+sig_pow_1day

bleak_year_max = pow_1year+sig_pow_1year
bleak_year_min = log_1year-sig_log_1year

sig_bleak_day = (bleak_day_max - bleak_day_min)/2
sig_bleak_year = (bleak_year_max - bleak_year_min)/2

bleak_day = np.mean([log_1day, pow_1day], axis=0)
bleak_year = np.mean([log_1year, pow_1year], axis=0)

gs = gridspec.GridSpec(2,1, height_ratios = [3,1])
ax3 = plt.subplot(gs[0])
plt.errorbar(x, bleak_day, sig_bleak_day, fmt='o', 
        label = 'day fit')
plt.errorbar(x, bleak_year, sig_bleak_year, fmt='o', 
        label = '1/2 year fit')
plt.legend(loc = 'best')
plt.grid()
plt.semilogx()
plt.title('Extrapolated Values, 1 layer SC, B0 = 34.3 mT')
plt.ylabel('$B_{leak}$ [mT]')

ax4 = plt.subplot(gs[1])
plt.plot(x, sig_bleak_day/bleak_day*100,'o')
plt.plot(x, sig_bleak_year/bleak_year*100,'o')
plt.grid()
plt.xlabel('Run time [s]')
plt.ylabel('$\delta$ B [%]')
#plt.semilogx()
plt.savefig('averaged_extrapolated_value_linx.png')
plt.close()
###########################################
dev = (bleak_day/extrap_1day-1)*100
plt.plot(x,dev,'o')
plt.xlabel('Run time [s]')
plt.ylabel('$B_{meas}(day)/B_{extrap}(day) $[%]')
plt.title('Averaging Extrapolated Values')
#plt.semilogx()
plt.savefig('averaged_extrapolated_vs_measured_linx.png')
plt.close()
