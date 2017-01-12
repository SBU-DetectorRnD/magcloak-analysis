import numpy as np
import matplotlib.pyplot as plt
from uncertainties import unumpy
from uncertainties import ufloat

fm_length = 100
zero = 150

ri_meas = 'ri.txt'
ro_meas = 'ro.txt'

calibration = 'DataFile_141124_081912_calibration.txt'
fm_scan = ['DataFile_141219_144019_fm_scan_upramp.txt']

initial_mag = ['DataFile_141219_143350_initial_mag.txt']
fm_scan_output_graph = '80_20_permeability_scan_zoom.png'
fm_profile_graph = '80_20_permeability_b_vs_z.png'
plot_title = '80/20 Mixture'

profile = ['DataFile_141219_145353_b_vs_z.txt']

accidental_offset = [0] 

description = ['80/20 Mixture']

results_file = ['80_20_powder_results.txt']

ri = np.genfromtxt(ri_meas)
ro = np.genfromtxt(ro_meas)

a = ufloat(np.mean(ri), np.std(ri)) #inner radius value,err
b = ufloat(np.mean(ro), np.std(ro)) #inner radius value,err

def plot_u(cal_file, fm_file, offset_file, description, accidental_offset,
        results_file):
    M = np.genfromtxt(cal_file)
    N = np.genfromtxt(fm_file)
    O = np.genfromtxt(offset_file)

    i_helm = M[:,1] #current applied to helmholtz for calibration measurement
    b_helm = M[:,2] #field applied to helmholtz coil for calibration measurement
    p, cov = np.polyfit(i_helm, b_helm, 1,  cov=True) #fit a line to calibration measurement so that we get a calibration

    i_fm = N[:,1] #current applied to helmmholtz for shielding measurement
    b_fm = unumpy.uarray(N[:,2],0.0005) - accidental_offset #field measured inside of ferromagnet shield


    B_earth = np.polyval(p,0) #We get the Earths magnetic field from i=0 of the Helmholtz calibration
    B_fm_no_i = unumpy.uarray(np.mean(O[:,2]), np.std(O[:,2])) #Get average and error for initial magnetization

    mag = B_fm_no_i - B_earth #initial magnetization is the field inside of the ferromagnet before any field is applied minus the earths magnetic field 

    Bin = b_fm - mag #internal magnetization is the measured internal field minus the initial magnetization. This correction might not be necessary for a soft ferromagnet
    
    Bext = unumpy.uarray(np.polyval(p,i_fm), 0.0005) #external field
    Bext_nom = unumpy.nominal_values(Bext)
    Bext_err = unumpy.std_devs(Bext)

    B = Bext/Bin
    c = a/b
    u=(-2*B + c**2 - 2*unumpy.sqrt(B**2 - B*c**2 - B + c**2) + 1)/(c**2 - 1)

    u_nom = unumpy.nominal_values(u)
    u_err = unumpy.std_devs(u)


    #cakculate uerr with just point to point uncertainties. I define this as just
    #uncertainty from the field measurements
    u_pp=(-2*B + c.nominal_value**2 - 2*unumpy.sqrt(B**2 - B*c.nominal_value**2 - B + c.nominal_value**2) + 1)/(c.nominal_value**2 - 1)
    
    #calculate uerr from just geometry uncertainties
    u_geom=(-2*unumpy.nominal_values(B) + c**2 - 2*unumpy.sqrt(unumpy.nominal_values(B)**2 - unumpy.nominal_values(B)*c**2 - unumpy.nominal_values(B) + c**2) + 1)/(c**2 - 1)

    ##obtain uncertainties from field
    u_err_pp = unumpy.std_devs(u_pp)

    ##obtain uncertainties from geometry
    u_err_geom = unumpy.std_devs(u_geom)

    with open(results_file, "w") as myfile:
        myfile.write('#Bext, sig_Bext, ur, sig_ur, sig_ur_pp, sig_ur_corr\n')
        for j in range(0, len(u_nom)):
            myfile.write('%s\t%s\t%s\t%s\t%s\t%s\n' %(
                Bext_nom[j], Bext_err[j],
                u_nom[j], u_err[j], u_err_pp[j], u_err_geom[j]))

    plt.errorbar(Bext_nom, u_nom, u_err, marker = '.', label = description)


for i in range(0,len(fm_scan)):
    plot_u(calibration, fm_scan[i], initial_mag[i], description[i],
            accidental_offset[i], results_file[i])

plt.xlabel('$B_{ext}$ [mT]')
plt.ylabel('$\mu_r$')
plt.title(plot_title)
plt.legend(loc = 'best')
plt.savefig(fm_scan_output_graph) 
plt.close()

def plot_prof(profile, description, accidental_offset):
    M = np.genfromtxt(profile)
    plt.errorbar(M[:,0] - zero, M[:,1] - accidental_offset, M[:,2], 
            marker ='o', label = description)


for i in range(0,len(description)):
    plot_prof(profile[i], description[i], accidental_offset[i])

plt.xlabel('Distance [mm]')
plt.ylabel('B [mT]')
plt.title(plot_title)
plt.legend(loc = 'best')
plt.vlines(-fm_length/2, 14.5, 21)
plt.vlines(fm_length/2, 14.5, 21)
plt.savefig(fm_profile_graph) 
plt.close()
