import numpy as np
import pylab
from uncertainties import unumpy
from uncertainties import ufloat
#unumpy and ufloat handles error propagation quite nicely

a = ufloat(29.9/2,0.25) #inner radius value,err
th = ufloat(0.24,0.05) #outer radius value,err
b = a+th

cal_file = 'calibration3.txt' #helmholtz calibration
fm_file = 'fm_thin_baked.txt' #measurement for field inside ferromagnet shield
offset_file = 'fm_thin_baked_magnetization.txt' #initial magnetization of ferromagnet

M = np.genfromtxt(cal_file)
N = np.genfromtxt(fm_file)
O = np.genfromtxt(offset_file)

i_helm = M[:,1] #current applied to helmholtz for calibration measurement
b_helm = M[:,2] #field applied to helmholtz coil for calibration measurement

i_fm = N[:,1] #current applied to helmmholtz for shielding measurement
b_fm = unumpy.uarray(N[:,2],0.001) #field measured inside of ferromagnet shield

p, cov = np.polyfit(i_helm, b_helm, 1,  cov=True) #fit a line to calibration measurement so that we get a calibration

B_earth = np.polyval(p,0) #We get the Earths magnetic field from i=0 of the Helmholtz calibration
B_fm_no_i = unumpy.uarray(np.mean(O[:,2]), np.std(O[:,2])) #Get average and error for initial magnetization

mag = B_fm_no_i - B_earth #initial magnetization is the field inside of the ferromagnet before any field is applied minus the earths magnetic field 

Bin = b_fm - mag #internal magnetization is the measured internal field minus the initial magnetization. This correction might not be necessary for a soft ferromagnet
Bext = unumpy.uarray(np.polyval(p,i_fm), 0.001) #external field
Bext_nom = unumpy.nominal_values(Bext)
Bext_err = unumpy.std_devs(Bext)


#calculate u_r
u=(-2*Bext*b**2 + Bin*a**2 + Bin*b**2 - 2*unumpy.sqrt(b**2*(Bext**2*b**2 - Bext*Bin*a**2 - Bext*Bin*b**2 + Bin**2*a**2)))/(Bin*(a**2 - b**2))

#obtain nominal value and uncertainty from u
u_nom = unumpy.nominal_values(u)
u_err = unumpy.std_devs(u)

#pylab.errorbar(unumpy.nominal_values(Bext), u_nom, u_err)
pylab.errorbar(Bext_nom, u_nom, u_err, fmt = '.') #plot u_r vs B_ext
pylab.errorbar(Bext_nom, u_nom, u_err, color = 'b') #plot a line as well so that it doesn't look like the values are just jumping around.
pylab.xlabel('$B_{ext}$ [mT]')
pylab.ylabel('Relative permeability')
pylab.title('Relative permeability measurement')
pylab.savefig('permeability_scan.png')
