import numpy as np
import matplotlib.pyplot as plt
from uncertainties import unumpy
from uncertainties import ufloat

fm_length = 12

ri_meas = 'ri.txt'
#ro_meas = 'ro.txt'
th_meas = 'thickness.txt'

ri = np.genfromtxt(ri_meas)
th = np.genfromtxt(th_meas)

a = ufloat(np.mean(ri), np.std(ri)) #inner radius value,err
thickness = ufloat(np.mean(th), np.std(th)) #inner radius value,err
b = a+thickness

results_file = ['430_sheet_results.txt']

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


calibration = 'DataFile_141124_081912_calibration.txt'
fm_scan = ['DataFile_141124_083834_fm_scan_upramp.txt']

initial_mag = ['DataFile_141124_083703_initial_mag.txt']

description = ['Room Temperature 1',
        'Room Temperature 2',
        'lN2 1',
        'lN2 2']

accidental_offset = [0,0,0,0.0]

for i in range(0,len(fm_scan)):
    plot_u(calibration, fm_scan[i], initial_mag[i], description[i],
            accidental_offset[i], results_file[i])

plt.xlabel('$B_{ext}$ [mT]')
plt.ylabel('$\mu_r$')
plt.title('430 Steel Sheet')
plt.legend(loc = 'best')
plt.savefig('430_sheets_scan.png') 

#plt.ylim(3,5)
#plt.savefig('50_50_permeability_scan_zoom.png') 
#plt.close()

#def plot_prof(profile, description, accidental_offset):
#    M = np.genfromtxt(profile)
#    plt.errorbar(M[:,0] - 153.5, M[:,1] - accidental_offset, M[:,2], label = description)
#
#profile = ['DataFile_141111_211629_bvz_helmholtz.txt',
#            'DataFile_141111_204657_room_bvz.txt',
#            'DataFile_141111_193629_cryo_bvz.txt']
#
#description = ['$B_{ext}$', '$B_{in}(T = room)$', '$B_{in}(T = room)$']
#accidental_offset = [0,0.027,0]
#
#for i in range(0,len(description)):
#    plot_prof(profile[i], description[i], accidental_offset[i])
#
#plt.xlabel('Distance [mm]')
#plt.ylabel('B [mT]')
#plt.title('50/50 Steel Powder, Epoxy Mix')
#plt.legend(loc = 'best')
#plt.vlines(-fm_length/2, 16, 22)
#plt.vlines(fm_length/2, 16, 22)
#plt.savefig('50_50_field_profile.png') 
#
