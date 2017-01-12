import numpy as np
import matplotlib.pyplot as plt

# B vs z scans using the long superconductor shield inside liquid Nitrogen bath in beam pipe segment surrounded by five steering magnets

# update plot style
plt.rcParams.update({'axes.labelsize' : 18})
plt.rcParams.update({'xtick.labelsize' : 18})
plt.rcParams.update({'ytick.labelsize' : 18})

# conversion of probe positions given in mm to positon relative to center of cryostat
# NOTE: 'low' end of cryostat / superconductor is where the probe is all the way retracted
# out of the system, 'high' end is whne the probe is completely inserted

cstat_center = 0
cstat_length = 0
sc_dist_end_of_cstat_low = 0
sc_dist_end_of_cstat_high = 0

pos_shield_zstart = (-0. / 2.0 ) + sc_dist_end_of_cstat_low
pos_shield_zend = (0. / 2.0 ) - sc_dist_end_of_cstat_high

# B vs z room temperature magnets ON
data_BvsZ_warm_MagnetOn_1 = np.genfromtxt('data_bvz_copy/DataFile_160614_104404_MagnetOn_noCryo.txt', delimiter='\t')

# B vs z room temperature magnets OFF
data_BvsZ_warm_MagnetOff_1 = np.genfromtxt('data_bvz_copy/DataFile_160614_102654_noMagnet_noCryo.txt', delimiter='\t')

# Cooled, magnets ON
data_BvsZ_cryo_MagnetOn_1 = np.genfromtxt('data_bvz_copy/DataFile_160613_164327_MagnetOn_Cryo.txt', delimiter='\t')

# Cooled, magnets ON
data_BvsZ_cryo_MagnetOn_2 = np.genfromtxt('data_bvz_copy/DataFile_160613_170851_MagnetOn_Cryo.txt', delimiter='\t')


# print sums
print data_BvsZ_warm_MagnetOff_1 [0:120,1].sum()
print data_BvsZ_warm_MagnetOn_1[0:120,1].sum()

print data_BvsZ_cryo_MagnetOff_1 [0:120,1].sum()
print data_BvsZ_cryo_MagnetOn_1[0:120,1].sum()


# Magnet ON, T_cryo, B v z:
# Magnet OFF, T_cryo, B v z: 

# B vs Z
plt.plot( -1.0*(data_BvsZ_warm_MagnetOff_1[:,0]-cstat_center) , -1*data_BvsZ_warm_MagnetOff_1[:,1], 'bo' , label='MagnetOff (warm)')
plt.plot( -1.0*(data_BvsZ_warm_MagnetOn_1[:,0]-cstat_center) , -1*data_BvsZ_warm_MagnetOn_1[:,1], 'go' , label='MagnetON (warm)')
plt.plot( -1.0*(data_BvsZ_cryo_MagnetOff_1[:,0]-cstat_center) , -1*(data_BvsZ_cryo_MagnetOff_1[:,1]+(-0.04981)), 'co' , label='MagnetOff (cryo)')

plt.plot( -1.0*(data_BvsZ_cryo_MagnetOn_1[:,0]-cstat_center) , -1*data_BvsZ_cryo_MagnetOn_1[:,1], 'ro' , label='MagnetON (cryo)')
plt.plot( -1.0*(data_BvsZ_cryo_MagnetOff_2[:,0]-cstat_center) , -1*data_BvsZ_cryo_MagnetOff_2[:,1], 'yo' , label='MagnetOff2 (cryo)')
plt.plot( -1.0*(data_BvsZ_cryo_MagnetOn_2[:,0]-cstat_center) , -1*data_BvsZ_cryo_MagnetOn_2[:,1], 'ko' , label='MagnetON2 (cryo)')

plt.axvline(x=pos_shield_zstart, ymin=0.0, ymax = 1.0, linewidth=1, color='k')
plt.axvline(x=pos_shield_zend, ymin=0.0, ymax = 1.0, linewidth=1, color='k')

#plt.axis([60, 240, 0, 11])

plt.xlabel('z [mm]')
plt.ylabel('B [mT]')

plt.legend(loc='best')

plt.show()


# Prepare differnece plots
# warm
zpos_warm = np.intersect1d(data_BvsZ_warm_MagnetOn_1[:,0], data_BvsZ_warm_MagnetOff_1[:,0])

diff_warm = np.zeros( ( zpos_warm.shape[0], 2 ) )
diff_warm[:,0] = zpos_warm

for z in zpos_warm:
    newval = data_BvsZ_warm_MagnetOn_1[ data_BvsZ_warm_MagnetOn_1[:,0] == z,1] - data_BvsZ_warm_MagnetOff_1[ data_BvsZ_warm_MagnetOff_1[:,0] == z,1]
    diff_warm[ diff_warm[:,0] == z , 1 ] = newval[0]

# cryo
zpos_cryo = np.intersect1d(data_BvsZ_cryo_MagnetOn_1[:,0], data_BvsZ_cryo_MagnetOff_1[:,0])

diff_cryo = np.zeros( ( zpos_cryo.shape[0], 2 ) )
diff_cryo[:,0] = zpos_cryo

for z in zpos_cryo:
    newval = data_BvsZ_cryo_MagnetOn_1[ data_BvsZ_cryo_MagnetOn_1[:,0] == z,1] - data_BvsZ_cryo_MagnetOff_2[ data_BvsZ_cryo_MagnetOff_2[:,0] == z,1]
    diff_cryo[ diff_cryo[:,0] == z , 1 ] = newval[0]

# plot difference plots
plt.plot( -1.0*(diff_warm[:,0]-cstat_center) , -1*diff_warm[:,1], 'ro' , label='room temperature')
plt.plot( -1.0*(diff_cryo[:,0]-cstat_center) , -1*diff_cryo[:,1], 'bo' , label='lN2 temperature')

plt.axvline(x=pos_shield_zstart, ymin=0.0, ymax = 1.0, linewidth=2, color='0.3', linestyle='dashed')
plt.axvline(x=pos_shield_zend, ymin=0.0, ymax = 1.0, linewidth=2, color='0.3', linestyle='dashed')

plt.axis([-800, 800, -0.1, 5])
plt.locator_params(axis = 'x', nbins = 5)
plt.locator_params(axis = 'y', nbins = 10)

plt.xlabel('z [mm]')
plt.ylabel('B [mT]')

plt.legend(loc='best')

plt.savefig('plots/BvZ_2.png', bbox_inches='tight')
plt.savefig('plots/BvZ_2.eps', bbox_inches='tight')
plt.show()
