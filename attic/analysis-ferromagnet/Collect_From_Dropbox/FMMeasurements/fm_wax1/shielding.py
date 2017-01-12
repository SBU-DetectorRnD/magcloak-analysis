import numpy as np
import pylab
from uncertainties import unumpy
from uncertainties import ufloat
a = ufloat(29.9/2,1/2)
th = ufloat(0.24,0.05)
b = a+th

cal_file = 'helmholtz_calibration.txt'
fm_file = 'fm_shielding.txt'
#offset_file = 'initial_magnetization.txt'

M = np.genfromtxt(cal_file)
N = np.genfromtxt(fm_file)
#O = np.genfromtxt(offset_file)

i_helm = -M[:,1]
b_helm = -M[:,2] 

i_fm = N[:,1]-0.33
b_fm = unumpy.uarray(N[:,2],0.001)

p, cov = np.polyfit(i_helm, b_helm, 1,  cov=True)

#B_earth = np.polyval(p,0)
B_earth = 0
#B_fm_no_i = unumpy.uarray(np.mean(O[:,2]), np.std(O[:,2]))

#mag = B_fm_no_i - B_earth
mag=0
Bin = b_fm - mag
Bext = unumpy.uarray(np.polyval(p,i_fm), 0.001)


accept = np.where(Bext-Bin > 0)

print(accept)
Bin = Bin[accept]
Bext = Bext[accept]
print(Bin)
print(Bext)

u=(-2*Bext*b**2 + Bin*a**2 + Bin*b**2 - 2*unumpy.sqrt(b**2*(Bext**2*b**2 - Bext*Bin*a**2 - Bext*Bin*b**2 + Bin**2*a**2)))/(Bin*(a**2 - b**2))

u_nom = unumpy.nominal_values(u)
u_err = unumpy.std_devs(u)
print(u)
#
#pylab.errorbar(unumpy.nominal_values(Bext), u_nom, u_err)
#pylab.errorbar(i_fm, u_nom, u_err, fmt = '.')
#pylab.errorbar(i_fm, u_nom, u_err, color = 'b')
#pylab.xlabel('Applied Field [mT]')
#pylab.ylabel('Relative permeability')
#pylab.title('Relative permeability measurement')
#pylab.show()
