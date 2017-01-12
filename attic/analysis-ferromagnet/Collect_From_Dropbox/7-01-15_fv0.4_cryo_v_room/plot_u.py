import numpy as np
import matplotlib.pyplot as plt
from uncertainties import unumpy
from uncertainties import ufloat
import sys

ri_meas = 'ri.txt'
ro_meas = 'ro.txt'

calibration = 'DataFile_150618_104728_calibration.txt'
fm_scan = ['DataFile_150701_144151_fm_scan_cryo.txt',
        'DataFile_150701_140506_fm_scan_room.txt']

#initial_mag = ['DataFile_150610_200847_initial_mag_placeholder.txt']
fm_scan_output_graph = 'uvb_f40.png'
#fm_profile_graph = '80_20_permeability_b_vs_z.png'
plot_title = 'Epoxy/Steel, fv = 0.4, z = 0 mm'

#profile = ['DataFile_141219_145353_b_vs_z.txt']

accidental_offset = [-0.012,0] 

description = ['lN2 Temperature', 'Room Temperature']

results_file = ['uvb_f40_cryo.txt', 'uvb_f40_room.txt']
colors = ['blue', 'red']

ri = np.genfromtxt(ri_meas)
ro = np.genfromtxt(ro_meas)

a = ufloat(np.mean(ri), np.std(ri)) #inner radius value,err
b = ufloat(np.mean(ro), np.std(ro)) #outer radius value,err
print(a)

c = a/b
print("thickness ratio: " + str(c.n))

u_cloak = (1/c.n**2+1)/(1/c.n**2-1)
print(u_cloak)
f1 = plt.figure(1)
f2 = plt.figure(2)

def plot_u(cal_file, fm_file,  description, accidental_offset,
        results_file, col):
    M = np.genfromtxt(cal_file)
    N = np.genfromtxt(fm_file)

    i_helm = M[:,1] #current applied to helmholtz for calibration measurement
    b_helm = M[:,2] #field applied to helmholtz coil for calibration measurement
    p, cov = np.polyfit(i_helm, b_helm, 1,  cov=True) #fit a line to calibration measurement so that we get a calibration

    
    i_fm = N[:,1] #current applied to helmmholtz for shielding measurement
    ind_i_fm_up = np.where(np.gradient(i_fm)>0)
    b_fm = unumpy.uarray(N[:,2],0.0005) - accidental_offset #field measured inside of ferromagnet shield


    B_earth = np.polyval(p,0) #We get the Earths magnetic field from i=0 of the Helmholtz calibration
#    B_fm_no_i = unumpy.uarray(np.mean(O[:,2]), np.std(O[:,2])) #Get average and error for initial magnetization

#    mag = B_fm_no_i - B_earth #initial magnetization is the field inside of the ferromagnet before any field is applied minus the earths magnetic field 
    mag = 0 #initial magnetization is the field inside of the ferromagnet before any field is applied minus the earths magnetic field 

    Bin = b_fm - mag #internal magnetization is the measured internal field minus the initial magnetization. This correction might not be necessary for a soft ferromagnet
    
    Bin_nom = unumpy.nominal_values(Bin)
    Bin_err = unumpy.std_devs(Bin)

    Bext = unumpy.uarray(np.polyval(p,i_fm), 0.0005) #external field
    Bext_nom = unumpy.nominal_values(Bext)
    Bext_err = unumpy.std_devs(Bext)

    B = Bin/Bext
    c = a/b
    u = (B*c**2 + B -2 -2*unumpy.sqrt(B**2*c**2 - B*c**2 - B + 1))/(B*c**2-B)
#    for ind, i in enumerate(u):
#        if i < 0:
#            i = (B[ind]*c**2 + B[ind] -2 +2*unumpy.sqrt(B[ind]**2*c**2 - B[ind]*c**2 - B[ind] + 1))/(B[ind]*c**2-B[ind])
    u_up = u[ind_i_fm_up]
    B_ind = u*Bext
    B_ind_up = u_up*Bext[ind_i_fm_up]


    B_ind_nom = unumpy.nominal_values(B_ind)
    B_ind_err = unumpy.std_devs(B_ind)
    
    B_ind_up_nom = unumpy.nominal_values(B_ind_up)
    B_ind_up_err = unumpy.std_devs(B_ind_up)


    u_nom = unumpy.nominal_values(u)
    print(u_nom)
    u_err = unumpy.std_devs(u)


    u_up_nom = u_nom[ind_i_fm_up]
    u_up_err = u_err[ind_i_fm_up]

    #cakculate uerr with just point to point uncertainties. I define this as just
    #uncertainty from the field measurements
    u_pp = (B*c.n**2 + B -2 -2*unumpy.sqrt(B**2*c.n**2 - B*c.n**2 - B +
        1))/(B*c.n**2-B)

    #calculate uerr from just geometry uncertainties
    u_geom = (unumpy.nominal_values(B)*c**2 + unumpy.nominal_values(B) -2 -2*unumpy.sqrt(unumpy.nominal_values(B)**2*c**2 - unumpy.nominal_values(B)*c**2 - unumpy.nominal_values(B) +
        1))/(unumpy.nominal_values(B)*c**2-unumpy.nominal_values(B))

    ##obtain uncertainties from field
    u_err_pp = unumpy.std_devs(u_pp)

    ##obtain uncertainties from geometry
    u_err_geom = unumpy.std_devs(u_geom)


    with open(results_file, "w") as myfile:
        myfile.write('#Bext, sig_Bext, Bi, sig_Bi, ur, sig_ur, sig_ur_pp, sig_ur_corr\n')
        for j in range(0, len(u_nom)):
            myfile.write('%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' %(
                Bext_nom[j], Bext_err[j], Bin_nom[j], Bin_err[j],
                u_nom[j], u_err[j], u_err_pp[j], u_err_geom[j]))
    
    plt.figure(1)
    plt.errorbar(Bext_nom[ind_i_fm_up], u_up_nom, u_up_err, marker = '.', label
            = description, color = col)



for i in range(0,len(fm_scan)):
    plot_u(calibration, fm_scan[i],  description[i],
            accidental_offset[i], results_file[i], colors[i])

plt.figure(1)
plt.tick_params(labelsize=18)
plt.xlabel('B$_o$ [mT]', fontsize = 20)
plt.ylabel('$\mu_r$', fontsize = 22)
plt.title(plot_title)
plt.legend(loc = 'best')
#plt.hlines(u_cloak,0,55, linewidth = 3, linestyles = 'dashed' )
#plt.text(25,2.62,'Perfect Cloak')
#plt.semilogy()
plt.grid()
plt.tight_layout()
plt.savefig(fm_scan_output_graph) 
plt.close()

