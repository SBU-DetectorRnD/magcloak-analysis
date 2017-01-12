import numpy as np
import pylab
from uncertainties import unumpy
from uncertainties import ufloat
#unumpy and ufloat handles error propagation quite nicely
29.9/2,1/2
a = ufloat(29.9/2,1/2) #inner radius value,err
#th = ufloat(0.24,0.025) #outer radius value,err
#b = a+th
b = ufloat(42.51667/2, 0.844381/2)

cal_file = 'DataFile_140711_162438_helmhotz_calibration.txt' #helmholtz calibration
fm_file = 'DataFile_140716_135753_negative1_scan.txt' #measurement for field inside ferromagnet shield
offset_file = 'DataFile_140716_135617_negative1_initial_offset.txt' #initial magnetization of ferromagnet

M = np.genfromtxt(cal_file)
N = np.genfromtxt(fm_file)
O = np.genfromtxt(offset_file)

b_err_res = 0.0005
b_err_meas = np.std(O[:,2])
b_err = (b_err_res**2 + b_err_meas**2)**(1/2)

i_helm = M[:,1] #current applied to helmholtz for calibration measurement
b_helm = M[:,2] #field applied to helmholtz coil for calibration measurement

i_fm = N[:,1] #current applied to helmmholtz for shielding measurement
b_fm = unumpy.uarray(N[:,2],0.001) #field measured inside of ferromagnet shield

p, cov = np.polyfit(i_helm, b_helm, 1,  cov=True) #fit a line to calibration measurement so that we get a calibration

B_earth = np.polyval(p,0) #We get the Earths magnetic field from i=0 of the Helmholtz calibration
B_fm_no_i = unumpy.uarray(np.mean(O[:,2]), b_err) #Get average and error for initial magnetization

mag = B_fm_no_i - B_earth #initial magnetization is the field inside of the ferromagnet before any field is applied minus the earths magnetic field 
mag = 0

Bin = b_fm - mag #internal magnetization is the measured internal field minus the initial magnetization. This correction might not be necessary for a soft ferromagnet
Bext = unumpy.uarray(np.polyval(p,i_fm), 0.001) #external field
print(Bin)
print(Bext)
#Bin = abs(Bin)
#Bext = abs(Bext)
#print(Bin)
#print(Bext)

Bext_nom = unumpy.nominal_values(Bext)
Bext_err = unumpy.std_devs(Bext)

#calculate u_r
B = Bext/Bin
c = a/b
u=(-2*B + c**2 - 2*unumpy.sqrt(B**2 - B*c**2 - B + c**2) + 1)/(c**2 - 1)
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

with open("uncertainties.txt", "a") as myfile:
    for j in range(0,len(u_nom)):
        myfile.write('%s \t %s \t %s \n' %(Bext_nom[j], u_nom[j], u_err[j]))
