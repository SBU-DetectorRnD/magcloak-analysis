import numpy as np
import matplotlib.pyplot as plt
import os

files_1hr = ['DataFile_150426_163239_1mt_1hr.txt',
        'DataFile_150426_152825_5mt_1hr.txt',
        'DataFile_150426_173401_10mt_1hr.txt',
        'DataFile_150426_123508_25mt_1hr.txt',
        'DataFile_150426_184558_37.6mt_1hr.txt']

field_list = ['1', '5', '10', '25', '37.6']

results_file = 'results.txt'

if os.path.isfile(results_file):
    os.remove(results_file)

with open(results_file, "a") as myfile:
    myfile.write("bext\tsig_bext\ti\tsig_i\tratio\tsig_ratio\n")

def plot_hist(file, field):
    M = np.genfromtxt(file)
    t = M[:,0]
    i = M[:,1]
    b = M[:,2]

    u_field = np.mean(b)
    sig_field = np.std(b)

    u_current = np.mean(i)
    sig_current = np.std(i)

    u_ratio = np.mean(b/i)
    sig_ratio = np.std(b/i)

    with open(results_file,"a") as myfile:
        myfile.write("%s\t%s\t%s\t%s\t%s\t%s\n" %(u_field, \
                sig_field, u_current, sig_current, u_ratio,\
                sig_ratio))

    plt.plot(t, b,'.')
    plt.xlabel('Time [s]')
    plt.ylabel('Field [mT]')
    plt.title(field+ ' mT Field Stability')
    plt.savefig(field+'mT_field_stability_1hr_time.png')
    plt.close()

    plt.plot(t, i,'.')
    plt.xlabel('Time [s]')
    plt.ylabel('Current [A]')
    plt.title(field+ ' mT Current Stability')
    plt.savefig(field+'mT_current_stability_1hr_time.png')
    plt.close()

    plt.plot(t, b/i,'.')
    plt.xlabel('Time [s]')
    plt.ylabel('Field/Current [mT/A]')
    plt.title(field+ ' mT ratio Stability')
    plt.savefig(field+'mT_ratio_stability_1hr_time.png')
    plt.close()

    plt.hist(b)
    plt.xlabel('Field [$\mu$T]')
    plt.title(field + ' Field Stability, $\mu$ = %1.3f mT, $\sigma$ = \
%1.3f$\mu$T' %(u_field,sig_field*1000))
    plt.savefig(field + 'mt_field_stability_1hr_hist.png')
    plt.close()

    plt.hist(i)
    plt.xlabel('Current [A]')
    plt.title(field + ' Current Stability, $\mu$ = %1.3f A, $\sigma$ = \
%1.3f A' %(u_current,sig_current))
    plt.savefig(field + 'mt_current_stability_1hr_hist.png')
    plt.close()

    plt.hist(b/i)
    plt.xlabel('Field/Current [mT/A]')
    plt.title(field + ' Ratio Stability, $\mu$ = %1.3f mT/A, $\sigma$ = \
%1.3f mT/A' %(u_ratio,sig_ratio))
    plt.savefig(field + 'mt_ratio_stability_1hr_hist.png')
    plt.close()



for (file, field) in zip(files_1hr, field_list):
    plot_hist(file, field)

M = np.genfromtxt('DataFile_150426_121729_zero_field.txt')
plt.plot(M[:,0], M[:,2],'.')
plt.xlabel('Time [s]')
plt.ylabel('Field [mT]')
plt.title('Zero Field Stability')
plt.savefig('zero_field_stability_10min_time.png')
plt.close()

plt.hist(M[:,2]*1000)
plt.xlabel('Field [mT]')
u = np.mean(M[:,2])*1000
sig = np.std(M[:,2]*1000)
plt.title('Zero Field Stability, $\mu$ = %1.3f $\sigma$ = %1.3f' %(u,sig))
plt.xlabel('Field [$\mu$T]')
plt.savefig('zero_field_stability_10min_hist.png')
plt.close()

M = np.genfromtxt('DataFile_150426_122737_zero_field_during_pouring.txt')

plt.plot(M[:,0], M[:,2],'.')
plt.xlabel('Time [s]')
plt.ylabel('Field [mT]')
plt.title('Zero Field Stability during Pouring')
plt.savefig('zero_field_stability_during_pour.png')
plt.close()

plt.hist(M[:,2]*1000)
plt.xlabel('Field [$\mu$T]')
u = np.mean(M[:,2])*1000
sig = np.std(M[:,2]*1000)
plt.title('Zero Field Stability during Pouring, $\mu$ = %.3f $\sigma$ = %.3f' %(u,sig))
plt.savefig('zero_field_stability_during_pour_hist.png')
plt.close()

files = ['DataFile_150426_133614_25mt_10min_1.txt',
        'DataFile_150426_134749_25mt_10min_2.txt',
        'DataFile_150426_135949_25mt_10min_3.txt']

legend_1 = ['Trial 1', 'Trial 2', 'Trial 3']

for ind, file in enumerate (files):
    M = np.genfromtxt(file)
    plt.plot(M[:,0], M[:,2], label = legend_1[ind])

plt.title('25 mT Field Stability for 10 min')
plt.xlabel('Time [s]')
plt.ylabel('Field [mT]')
plt.legend(loc = 'best')
plt.savefig('25mt_10min_field.png')
plt.close()

for ind, file in enumerate (files):
    M = np.genfromtxt(file)
    plt.plot(M[:,0], M[:,1], label = legend_1[ind])

plt.xlabel('Time [s]')
plt.ylabel('Current [A]')
plt.legend(loc = 'best')
plt.savefig('25mt_10min_current.png')
plt.close()

for ind, file in enumerate (files):
    M = np.genfromtxt(file)
    plt.plot(M[:,0], M[:,2]/M[:,1], label = legend_1[ind])

plt.xlabel('Time [s]')
plt.ylabel('Ratio [mT/A]')
plt.legend(loc = 'best')
plt.savefig('25mt_10min_ratio.png')
plt.close()


for ind, file in enumerate (files):
    M = np.genfromtxt(file)
    u = np.mean(M[:,2]/M[:,1])
    sig = np.std(M[:,2]/M[:,1])
    plt.hist(M[:,2]/M[:,1], label = legend_1[ind] + ', $\mu$ = %.3f, $\sigma$ = %.4f' %(u, sig))

plt.xlabel('Ratio [mT/A]')
plt.title('25 mT Ratio Stability')
plt.legend(loc = 'best')
plt.savefig('25mt_10min_ratio_hist.png')
plt.close()

for ind, file in enumerate (files):
    M = np.genfromtxt(file)
    u = np.mean(M[:,2])
    sig = np.std(M[:,2])
    plt.hist(M[:,2]/M[:,1], label = legend_1[ind] + ', $\mu$ = %.3f, $\sigma$ = %.4f' %(u, sig))

plt.xlabel('Field [mT]')
plt.title('25 mT Field Stability')
plt.legend(loc = 'best')
plt.savefig('25mt_10min_field_hist.png')
plt.close()


for ind, file in enumerate (files):
    M = np.genfromtxt(file)
    u = np.mean(M[:,1])
    sig = np.std(M[:,1])
    plt.hist(M[:,2]/M[:,1], label = legend_1[ind] + ', $\mu$ = %.3f, $\sigma$ = %.4f' %(u, sig))

plt.xlabel('Current [A]')
plt.title('25 mT Current Stability')
plt.legend(loc = 'best')
plt.savefig('25mt_10min_current_hist.png')
plt.close()



M = np.genfromtxt('DataFile_150426_141305_zero_field_1hr.txt')
plt.plot(M[:,0], M[:,2],'.')
plt.xlabel('Time [s]')
plt.ylabel('Field [mT]')
plt.title('Zero Field Stability')
plt.savefig('zero_field_stability_1hr_time.png')
plt.close()

plt.hist(M[:,2]*1000)
plt.xlabel('Field [$\mu$T]')
u = np.mean(M[:,2])
sig = np.std(M[:,2])

with open(results_file,"a") as myfile:
    myfile.write("%s\t%s\t%s\t%s\t%s\t%s\n" %(u, \
            sig, 0, 0, np.nan, np.nan))

plt.title('Zero Field Stability, $\mu$ = %1.3f $\sigma$ = %1.3f'\
        %(u*1000,sig*1000))
plt.xlabel('Field [$\mu$T]')
plt.savefig('zero_field_stability_1hr_hist.png')
plt.close()



