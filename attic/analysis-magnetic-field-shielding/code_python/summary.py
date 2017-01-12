import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from multiprocessing import Pool

def load_summary(summary_file):
    with open(summary_file) as f:
        line = f.readline()
        legend = line[1:-1]
        new_legend = []
        for c in legend:
            if c =='_':
                new_legend.append(' ')
            else:
                new_legend.append(c)
        legend = new_legend
        legend = str(''.join(legend))
        del new_legend
    
    summary = np.genfromtxt(summary_file, comments='#', delimiter='\t')
    
    b0 = summary[:,0]
    sig_b0 = summary[:,1]
    b_leak = summary[:,2]
    sig_b_leak = summary[:,3]
    bshield = summary[:,4]
    sig_bshield = summary[:,5]
    shielding_factor = summary[:,6]
    sig_shielding_factor = summary[:,7]
    fit_type = summary[:,8]
    return (legend, b0, sig_b0, b_leak, sig_b_leak, bshield, sig_bshield,
            shielding_factor, sig_shielding_factor, fit_type)

def plot_internal_trend(fit_file, title):
    for j in range(0, len(fit_file)):
        [legend, b0, b0_err, b_leak, b_leak_err, bshield, bshield_err, 
            shielding_factor, shielding_factor_err, 
             fit_type] = load_summary(fit_file[j])
        plt.errorbar(b0, b_leak, b_leak_err, fmt = '.', label = legend)
    plt.ylabel('$B_{leak}$(1/2 year) [mT]')
    plt.xlabel('$B_{0}$ [mT]')
    plt.legend(loc='best')
    plt.title('Internal Field')
#    plt.semilogx()
#    plt.loglog()
    plt.savefig("internal/" + title + "_internal.png")
    plt.close()

def plot_shielding(fit_file, title):
    for j in range(0,len(fit_file)):
        [legend, b0, b0_err, b_leak, b_leak_err, bshield, bshield_err,
        shielding_factor, shielding_factor_err,  
        fit_type] = load_summary(fit_file[j])
        plt.errorbar(b0, bshield, bshield_err, fmt = '.', label =
                legend)
    plt.ylabel('$B_{shield}(day)$ [mT]')
    plt.xlabel('$B_{0} [mT]$')
    plt.title('Shielded Field' )
    plt.legend(loc='best')
#    plt.semilogx()
#    plt.loglog()
    plt.gca().set_ylim(bottom=0)
    plt.grid(True, which = 'both')
    plt.savefig("shielding/" + title + "_shielding_trend.png")
    plt.close()     

def plot_shielding_factor(fit_file, title):
    for j in range(0,len(fit_file)):
        [legend, b0, b0_err, b_leak, b_leak_err, bshield, bshield_err,
        shielding_factor, shielding_factor_err,  
        fit_type] = load_summary(fit_file[j])
        plt.errorbar(b0, shielding_factor, shielding_factor_err, 
                fmt = '.', label = legend)
    plt.ylabel('Shielding Factor(day) [mT]')
    plt.xlabel('$B_{0} [mT]$')
    plt.title('Shielding Factor')
#    plt.ylim([0,1])
    plt.legend(loc='best')
#    plt.semilogx()
    plt.savefig("shielding_factor/" + title + "_shielding_factor_trend.png")
    plt.close() 

def plot_fit_type(fit_file,title):
    for j in range(0,len(fit_file)):
        [legend, b0, b0_err, b_leak, b_leak_err, bshield, bshield_err,
        shielding_factor, shielding_factor_err,  
        fit_type] = load_summary(fit_file[j])
        plt.plot(b0, fit_type, 'o' ,
                 label = legend)
    plt.ylabel('Fit Type')
    plt.xlabel('$B_{ext} [mT]$')
    plt.title('Fit Type')
#    plt.semilogx()
    plt.legend(loc='best')
    plt.savefig("fit_type/" + title + "_fit_type_trend.png")
    plt.close() 

def plot_stability_increment(increment_file, title):
    M = np.genfromtxt(increment_file)
    b0 = M[0,0]
    sig_b0 = M[0,1]
    extrapolate_time = M[0,2]
    increment_step = M[0,3]
    end_time = M[:,4]
    extrapolate_val = M[:,5]
    sig_extrapolate_val = M[:,6]
    chi2red = M[:,7]

    gs = gridspec.GridSpec(2,1, height_ratios=[2,1])
    ax1 = plt.subplot(gs[0])
    plt.errorbar(end_time/60, extrapolate_val, sig_extrapolate_val, fmt = '.',
            ms = 1,  label = title)
    plt.legend(loc = 'best')
    plt.title('Extrapolation Stability at $B_0$ = %3.2f mT for Increased Measurement Time' %(b0))
    plt.semilogx()
    days = int(extrapolate_time/86400)
    plt.ylabel('Extrapolated $B_{leak}$(t= %s days) [mT]' %(days))

    ax2 = plt.subplot(gs[1])
    plt.plot(end_time/60, chi2red, '.', label = title)
    plt.xlabel('Measurement End-Time [min]')
    plt.ylabel('\chi^2_r')
    plt.loglog()
    plt.savefig("stability_increment_"+title)

def plot_stability_decrement(decrement_file, title):
    M = np.genfromtxt(decrement_file)
    b0 = M[0,0]
    sig_b0 = M[0,1]
    extrapolate_time = M[0,2]
    decrement_step = M[0,3]
    begin_time = M[:,4]
    extrapolate_val = M[:,5]
    sig_extrapolate_val = M[:,6]
    chi2red = M[:,7]

    gs = gridspec.GridSpec(2,1, height_ratios=[2,1])
    ax1 = plt.subplot(gs[0])
    plt.errorbar(begin_time/60, extrapolate_val, sig_extrapolate_val, fmt = '.',
            ms = 1,  label = title)
    plt.legend(loc = 'best')
    plt.title('Extrapolation Stability at $B_0$ = %3.2f mT for Delayed Measurement Time' %(b0))
    plt.ylabel('Extrapolated $B_{leak}$(t=%3i days) [mT]' %(extrapolate_time/86400))

    ax2 = plt.subplot(gs[1])
    plt.plot(begin_time/60, chi2red, '.', label = title)
    plt.xlabel('Measurement Begin Time [min]')
    plt.ylabel('\chi^2_r')
    plt.loglog()
    plt.savefig("stability_decrement_"+title)

def plot_stability_slice(slice_file, title):
    M = np.genfromtxt(slice_file)
    b0 = M[0,0]
    sig_b0 = M[0,1]
    extrapolate_time = M[0,2]
    slice_range = M[0,3]
    begin_time = M[:,4]
    extrapolate_val = M[:,5]
    sig_extrapolate_val = M[:,6]
    chi2red = M[:,7]

    gs = gridspec.GridSpec(2,1, height_ratios=[2,1])
    ax1 = plt.subplot(gs[0])
    plt.errorbar(begin_time/60, extrapolate_val, sig_extrapolate_val, fmt = '.',
            ms = 1,  label = title)
    plt.legend(loc = 'best')
    plt.title('Extrapolation Stability at $B_0$ = %3.2f mT for Delayed \
                    Measurement Time with Slice Range %i' %(b0,slice_range))
    plt.ylabel('Extrapolated $B_{leak}$(t=%3i days) [mT]' %(extrapolate_time/86400))

    ax2 = plt.subplot(gs[1])
    plt.plot(begin_time/60, chi2red, '.', label = title)
    plt.xlabel('Measurement Begin Time [min]')
    plt.ylabel('\chi^2_r')
    plt.loglog()
    plt.savefig("stability_slice_"+title)



def plot_summaries( fit_file, title, param_file):

    plot_internal_trend( fit_file, title)
    plot_shielding( fit_file, title)
    plot_shielding_factor( fit_file, title)
#    plot_chi2r( fit_file, title)
    plot_fit_type( fit_file, title)
#    plot_params(param_file, title)

