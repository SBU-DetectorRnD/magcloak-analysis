import numpy as np
import matplotlib.pyplot as plt
from uncertainties import unumpy
from uncertainties import ufloat
#unumpy and ufloat handles error propagation quite nicely

a = ufloat(34.88/2,0) #inner radius value,err
b = ufloat(37.086499999999994/2,  0)
#th = ufloat(1.2,0.05) #outer radius value,err
#b = a+th
#b = ufloat(36.22/2,0) 

cal_file = 'DataFile_140711_162438_helmhotz_calibration.txt' #helmholtz calibration
fm_file = 'DataFile_140715_211052_scan.txt' #measurement for field inside ferromagnet shield
offset_file = 'DataFile_140715_210450_offset.txt' #initial magnetization of ferromagnet

M = np.genfromtxt(cal_file)
N = np.genfromtxt(fm_file)
O = np.genfromtxt(offset_file)

i_helm = M[:,1] #current applied to helmholtz for calibration measurement
b_helm = M[:,2] #field applied to helmholtz coil for calibration measurement

p, cov = np.polyfit(i_helm, b_helm, 1,  cov=True) #fit a line to calibration measurement so that we get a calibration

B_err_meas = np.std(O[:,2])
B_err_res = 0.0005
B_err = (B_err_meas**2 + B_err_res**2)**(1/2)

B_earth = np.polyval(p,0) #We get the Earths magnetic field from i=0 of the Helmholtz calibration
B_fm_no_i = unumpy.uarray(np.mean(O[:,2]), B_err) #Get average and error for initial magnetization

mag = B_fm_no_i - B_earth #initial magnetization is the field inside of the ferromagnet before any field is applied minus the earths magnetic field 

i_fm = N[:,1] #current applied to helmmholtz for shielding measurement
b_fm = unumpy.uarray(N[:,2],B_err) #field measured inside of ferromagnet shield

Bin = b_fm - mag #internal magnetization is the measured internal field minus the initial magnetization. This correction might not be necessary for a soft ferromagnet
Bext = unumpy.uarray(np.polyval(p,i_fm), 0.0005) #external field
Bext_nom = unumpy.nominal_values(Bext)
Bext_err = unumpy.std_devs(Bext)

plt.plot(i_helm, b_helm,'.')
plt.savefig('calibration.png')
plt.close()


#calculate u_r
u=(-2*Bext*b**2 + Bin*a**2 + Bin*b**2 - 2*unumpy.sqrt(b**2*(Bext**2*b**2 - Bext*Bin*a**2 - Bext*Bin*b**2 + Bin**2*a**2)))/(Bin*(a**2 - b**2))

#obtain nominal value and uncertainty from u
u_nom = unumpy.nominal_values(u)
u_err = unumpy.std_devs(u)

#plt.errorbar(unumpy.nominal_values(Bext), u_nom, u_err)
plt.errorbar(Bext_nom, u_nom, u_err, fmt = '.') #plot u_r vs B_ext
plt.errorbar(Bext_nom, u_nom, u_err, color = 'b') #plot a line as well so that it doesn't look like the values are just jumping around.
plt.xlabel('$B_{ext}$ [mT]')
plt.ylabel('Relative permeability')
plt.title('Relative permeability measurement for kapton/steel powder')
plt.savefig('permeability_scan.png')

with open("point_uncertainties.txt", "a") as myfile:
    for j in range(0,len(u_nom)):
        myfile.write('%s \t %s \t %s \n' %(Bext_nom[j], u_nom[j], u_err[j]))
