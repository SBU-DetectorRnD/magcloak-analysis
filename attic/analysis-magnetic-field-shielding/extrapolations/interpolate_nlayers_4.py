# Copy interpolate_nlayers.py and start editing it to analyze new set of data

import numpy as np
from scipy.interpolate import interp1d
import matplotlib.pyplot as plt
from scipy.interpolate import PchipInterpolator

def figsize(scale):
    fig_width_pt = 345                          # Get this from LaTeX using \the\textwidth
    inches_per_pt = 1.0/72.27                       # Convert pt to inch
    golden_mean = (np.sqrt(5.0)-1.0)/2.0            # Aesthetic ratio (you could change this)
    fig_width = fig_width_pt*inches_per_pt*scale    # width in inches
    fig_height = fig_width*golden_mean              # height in inches
    fig_size = [fig_width,fig_height]
    return fig_size

def despine():
    ax = plt.gca()
    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)
    ax.yaxis.set_ticks_position('left')
    ax.xaxis.set_ticks_position('bottom')

colors = {'c1': '#5DA5DA',
        'c2':'#FAA43A',
        'c3':'#60BD68',
        'c4':'#F17CB0',
        'c5':'#B2912F',
        'c6':'#B276B2',
        'c7':'#DECF3F',
        'c7':'#DECF3F',
        'c8':'#F15854',
        'c9':'#4D4D4D',}

fig = plt.figure(figsize=figsize(1))
#summary files
#ifile_1 = 'shielding_results_sep2016/BintVsBext_sep2016_helmholtz_sample1_1layer.txt'
ifile_1 = 'shielding_results_sep2016/BintVsBext_sep2016_helmholtz_sample2_1layer.txt'
ifile_2 = 'shielding_results_sep2016/BintVsBext_sep2016_helmholtz_sample2_2layer.txt'
# ifile_1 = 'shielding_results_sep2016/BintVsBext_raphael_helmholtz_1layer.txt'
# ifile_2 = 'shielding_results_sep2016/BintVsBext_raphael_helmholtz_2layer.txt'
ifile_3 = 'shielding_results_sep2016/BintVsBext_raphael_helmholtz_3layer.txt'
ifile_4 = 'shielding_results_sep2016/BintVsBext_raphael_helmholtz_4layer.txt'

#numerically calculates a derivative of whichever degree
def deriv(x,y, degree=1):
    if (degree == 1):
        dy = np.gradient(y, edge_order=2)
        dx = np.gradient(x, edge_order=2)
        #I'll figure out a better way to handle dx=0. It happens when we have
        #many layers and the internal field doesn't change
        if np.any(dx==0):
            print('dx = 0')
            return 0
        return dy/dx
    else:
        return deriv(x,deriv(x,y,degree-1))

#here to calculate the maximum of the 4th derivative in a given interval
def maxderiv4(x, f_int):
    der4_max = np.zeros(x.size)
    for ind in range(0,x.size-1):
        x_int = np.linspace(x[ind], x[ind+1],100)
        der4 = deriv(x_int,f_int(x_int),4)
        der4_max[ind] = np.max(der4[4:-4])
    return der4_max


#calculate total error from error in x and y
def sig_tot(x, y, sig_x, sig_y):
    return (sig_y**2 + (deriv(x,y)*sig_x)**2)**(1/2)

#read summary file
M1 = np.genfromtxt(ifile_1)
M2 = np.genfromtxt(ifile_2)
M3 = np.genfromtxt(ifile_3)
M4 = np.genfromtxt(ifile_4)

#define physical variables
def physical_variables(M):
    Bext = M[:,0]
    sig_Bext = M[:,1]
    Bleak = M[:,2]
    sig_Bleak = M[:,3]
    sig = sig_tot(Bext, Bleak, sig_Bext, sig_Bleak)
    return Bext, Bleak, sig

def data_plot(M,description, marker):
    Bext = M[:,0]
    sig_Bext = M[:,1]
    Bleak = M[:,2]
    sig_Bleak = M[:,3]
    sig = sig_tot(Bext, Bleak, sig_Bext, sig_Bleak)
    plt.errorbar(Bext,Bleak,sig, fmt = marker, label = description)

Bext1, Bleak1, sig1 = physical_variables(M1)
#interpolate B_int(B_ext)
f = PchipInterpolator(Bext1, Bleak1)
f_der = PchipInterpolator(Bext1,Bleak1).derivative()
f_sig = PchipInterpolator(Bext1, sig1)

#extrapolate B_leak to however many layers we desire
def extrapolate_n(x, f_leak, f_leak_sig, n_layers):
    y = f(x)
    sig = f_sig(x)

    for i in range(1,n_layers):
        x_dummy = y
        y = f(x_dummy)
        sig_y = f_sig(x_dummy)
        sig = sig_tot(x_dummy, y, sig, sig_y)
    return y, sig


#### from here on out, yn stands for the leaked field for n number of layers
xnew = np.linspace(Bext1[0], Bext1[-1])
y1, sig1 = extrapolate_n(xnew, f, f_sig, 1)
y2, sig2 = extrapolate_n(xnew, f, f_sig, 2)
y3, sig3 = extrapolate_n(xnew, f, f_sig, 3)
y4, sig4 = extrapolate_n(xnew, f, f_sig, 4)
y5, sig5 = extrapolate_n(xnew, f, f_sig, 5)
y10, sig10 = extrapolate_n(xnew, f, f_sig, 10)
y20, sig20 = extrapolate_n(xnew, f, f_sig, 20)

#plot results
data_plot(M1, 'measured 1 layer', 'D')
despine()
plt.ylim(ymin=-.5)
plt.ylabel('Bi (mT)')
plt.xlabel('Bo (mT)')
plt.legend(loc = 'best')
plt.ylim(ymax = 40)
plt.tight_layout()
# plt.savefig('extrapolating_performance0.png')

#plt.show()

plt.plot(xnew, y1-sig1, color = colors["c1"])
plt.plot(xnew, y1+sig1, color = colors["c1"])
# plt.savefig('extrapolating_performance1.png')

#plt.fill_between(xnew, y1-sig1, y1+sig1, facecolor = 'blue', alpha = 0.5)
plt.fill_between(xnew, y2-sig2, y2+sig2, alpha = 0.5, color = colors["c2"])
plt.fill_between(xnew, y3-sig3, y3+sig3, alpha = 0.5, color = colors["c3"])
plt.fill_between(xnew, y4-sig4, y4+sig4, alpha = 0.5, color = colors["c4"])
plt.plot([],[], color = colors["c2"], linewidth = 10, label = 'extrapolated 2 layers')
plt.plot([],[], color = colors["c3"], linewidth = 10, label = 'extrapolated 3 layers')
plt.plot([],[], color = colors["c4"], linewidth = 10, label = 'extrapolated 4 layers')
plt.legend(loc = 'best')
# # plt.savefig('extrapolating_performance2.png')
#
data_plot(M2, 'measured 2 layer', 's')
data_plot(M3, 'measured 3 layer', 'o')
data_plot(M4, 'measured 4 layer', '*')
plt.legend(loc = 'best')
plt.show()
#plt.semilogx()
# plt.savefig('extrapolating_performance3.png')
# plt.savefig('extrapolating_performance3.pgf')
# plt.close()
