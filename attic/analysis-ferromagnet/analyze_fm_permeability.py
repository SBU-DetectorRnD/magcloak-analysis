# Written by R. Cerphantes

import numpy as np
import matplotlib.pyplot as plt
from uncertainties import unumpy
from uncertainties import ufloat
import sys

#text files of the measured inner and outer radii
ri_meas = 'ri.txt'
ro_meas = 'ro.txt'

#calibration file. Tells you what field corresponds to what current
calibration = 'DataFile_150618_104728_calibration.txt'

#scan of the field inside of ferromagnet tube vs field outside ferromagnet tube
fm_scan = ['DataFile_150701_144151_fm_scan_cryo.txt',
        'DataFile_150701_140506_fm_scan_room.txt']

#output graph for permeability vs external field
fm_scan_output_graph = 'uvb_f40.png'

#title of graph
plot_title = 'Epoxy/Steel, fv = 0.4, z = 0 mm'

#in case you need to add an offset for your magnetic field measurements for
#whatever reason
accidental_offset = [-0.012,0] 

#these will be the legend entries
description = ['lN2 Temperature', 'Room Temperature']

#the files names for the permeaiblity results. The actual numbers, not the
#plots
results_file = ['uvb_f40_cryo.txt', 'uvb_f40_room.txt']

#turn files of tube dimensions into array
ri = np.genfromtxt(ri_meas)
ro = np.genfromtxt(ro_meas)

#take the average and standard deviation of the array of measured radii
a = ufloat(np.mean(ri), np.std(ri)) #inner radius value,err
b = ufloat(np.mean(ro), np.std(ro)) #outer radius value,err

#ratio of inner and outer radius
c = a/b
print("thickness ratio: " + str(c.n))

#calculate the perfect cloak permeability based on the dimensions of your
#ferromagnet
u_cloak = (1/c.n**2+1)/(1/c.n**2-1)
print("Perfect cloak permeability: " + str(u_cloak))

#define a function that will take the ferromagnet scan file and give you the
#permeability results.
def plot_u(cal_file, fm_file,  description, accidental_offset,
        results_file):
    M = np.genfromtxt(cal_file) #turn calibration file into a matrix
    N = np.genfromtxt(fm_file) #turn fm_scan file into a matrix


    i_helm = M[:,1] #current applied to helmholtz for calibration measurement
    b_helm = M[:,2] #field applied to helmholtz coil for calibration measurement
    p, cov = np.polyfit(i_helm, b_helm, 1,  cov=True) #fit a line to calibration measurement so that we get a calibration

    
    i_fm = N[:,1] #current applied to helmmholtz for shielding measurement
    Bin = unumpy.uarray(N[:,2],0.0005) - accidental_offset #field measured inside of ferromagnet shield


    Bin_nom = unumpy.nominal_values(Bin) 
    Bin_err = unumpy.std_devs(Bin)

    Bext = unumpy.uarray(np.polyval(p,i_fm), 0.0005) #external field
    Bext_nom = unumpy.nominal_values(Bext)
    Bext_err = unumpy.std_devs(Bext)

    B = Bin/Bext #ratio of internal to external field

    #calculate permeability
    u = (B*c**2 + B -2 -2*unumpy.sqrt(B**2*c**2 - B*c**2 - B + 1))/(B*c**2-B) 
    print(u)
    u_nom = unumpy.nominal_values(u)
    u_err = unumpy.std_devs(u)

    #cakculate uerr with just point to point uncertainties. I define this as just
    #uncertainty from the field measurements
    u_pp = (B*c.n**2 + B -2 -2*unumpy.sqrt(B**2*c.n**2 - B*c.n**2 - B +
        1))/(B*c.n**2-B)

    u_err_pp = unumpy.std_devs(u_pp)

    #calculate uerr from just geometry uncertainties
    u_geom = (unumpy.nominal_values(B)*c**2 + unumpy.nominal_values(B) -2 -2*unumpy.sqrt(unumpy.nominal_values(B)**2*c**2 - unumpy.nominal_values(B)*c**2 - unumpy.nominal_values(B) +
        1))/(unumpy.nominal_values(B)*c**2-unumpy.nominal_values(B))

    u_err_geom = unumpy.std_devs(u_geom)


    #write results onto a text file
    with open(results_file, "w") as myfile:
        myfile.write('#Bext, sig_Bext, Bi, sig_Bi, ur, sig_ur, sig_ur_pp, sig_ur_corr\n')
        for j in range(0, len(u_nom)):
            myfile.write('%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' %(
                Bext_nom[j], Bext_err[j], Bin_nom[j], Bin_err[j],
                u_nom[j], u_err[j], u_err_pp[j], u_err_geom[j]))
    
    plt.errorbar(Bext_nom, u_nom, u_err, marker = '.', label = description)


#loop through all of the ferromagnet scans
for i in range(0,len(fm_scan)):
    plot_u(calibration, fm_scan[i],  description[i],
            accidental_offset[i], results_file[i])

plt.xlabel('B$_o$ [mT]', fontsize = 20)
plt.ylabel('$\mu_r$', fontsize = 20)
plt.title(plot_title)
plt.legend(loc = 'best')
#plt.hlines(u_cloak,0,55, linewidth = 3, linestyles = 'dashed' )
#plt.text(25,2.62,'Perfect Cloak')
#plt.semilogy()
plt.grid()
plt.savefig(fm_scan_output_graph) 
plt.close()

