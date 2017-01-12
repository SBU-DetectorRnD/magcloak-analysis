import numpy as np
from scipy.interpolate import interp1d
import matplotlib.pyplot as plt
from scipy.interpolate import PchipInterpolator
import sys

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
        'c8':'#F15854',
        'c9':'#4D4D4D',}

fig = plt.figure(figsize=figsize(1))
#summary files
#would be just one, but we have to combine data from 2 runs
ifile_1 = 'shielding_results/1layer_wide_sc_fit_results.txt'
ifile_1s = 'shielding_results/wide_tape_1layer_sheath_fit_results.txt'

M1 = np.genfromtxt(ifile_1)
M1s = np.genfromtxt(ifile_1s)

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
    
#define physical variables
def physical_variables(M):
    Bext = M[:,0]
    sig_Bext = M[:,1]
    Bleak = M[:,2]
    sig_Bleak = M[:,3]
    sig = sig_tot(Bext, Bleak, sig_Bext, sig_Bleak)
    return Bext, Bleak, sig

def data_plot(M,description, c, marker, alph = 1):
    Bext = M[:,0]
    sig_Bext = M[:,1]
    Bleak = M[:,2]
    sig_Bleak = M[:,3]
    sig = sig_tot(Bext, Bleak, sig_Bext, sig_Bleak)
    plt.errorbar(Bext,Bleak,sig, color = c, fmt = marker, label = description,
            alpha = alph)
    
Bext1, Bleak1, sig1 = physical_variables(M1s)
f = interp1d(Bext1,Bleak1)
f_sig = interp1d(Bext1,sig1)

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
xnew = np.linspace(Bext1[1], Bext1[-1])
y1, sig1 = extrapolate_n(xnew, f, f_sig, 1)
y2, sig2 = extrapolate_n(xnew, f, f_sig, 2)
y3, sig3 = extrapolate_n(xnew, f, f_sig, 3)
y4, sig4 = extrapolate_n(xnew, f, f_sig, 4)
y5, sig5 = extrapolate_n(xnew, f, f_sig, 5)
y10, sig10 = extrapolate_n(xnew, f, f_sig, 10)
y20, sig20 = extrapolate_n(xnew, f, f_sig, 20)
y30, sig30 = extrapolate_n(xnew, f, f_sig, 30)
y40, sig40 = extrapolate_n(xnew, f, f_sig, 40)
y45, sig45 = extrapolate_n(xnew, f, f_sig, 45)
y50, sig50 = extrapolate_n(xnew, f, f_sig, 50)
#
##plot results
plt.fill_between(xnew, y5-sig5, y5+sig5, facecolor = colors['c3'], alpha = 1,
        edgecolor = 'none')
plt.fill_between(xnew, y10-sig10, y10+sig10, facecolor =colors['c4'] , alpha = 1,
        edgecolor = 'none')
plt.fill_between(xnew, y20-sig20, y20+sig20, facecolor = colors['c5'], alpha = 1,
        edgecolor = 'none')
plt.fill_between(xnew, y30-sig30, y30+sig30, facecolor = colors['c6'], alpha = 1,
        edgecolor = 'none')
plt.fill_between(xnew, y40-sig40, y40+sig40, facecolor = colors['c7'], alpha = 1,
        edgecolor = 'none')
plt.fill_between(xnew, y45-sig45, y45+sig45, facecolor = colors['c8'], alpha = 1,
        edgecolor = 'none')
#
data_plot(M1s, 'Measured 1 layer, sheath/dipole',colors['c1'], 'x')
data_plot(M1, 'Measured 1 layer, tube/Helmholtz', colors['c2'], '.', 0.5)
plt.text(610,520,'n=5')
plt.text(610,460,'n=10')
plt.text(610,360,'n=20')
plt.text(610,250,'n=30')
plt.text(610,120,'n=40')
plt.text(610,40,'n=45')
##plt.plot([],[], color = 'yellow', linewidth = 10, label = 'extrapolated 5 layers')
##plt.plot([],[], color = 'cyan', linewidth = 10, label = 'extrapolated 10 layers')
##plt.plot([],[], color = 'magenta', linewidth = 10, label = 'extrapolated 20 layers')
##plt.plot([],[], color = 'blue', linewidth = 10, label = 'extrapolated 30 layers')
##plt.plot([],[], color = 'green', linewidth = 10, label = 'extrapolated 40 layers')
##plt.plot([],[], color = 'orange', linewidth = 10, label = 'extrapolated 45 layers')
#
despine()
plt.legend(loc = 'upper left')
plt.ylim(ymin=-.5)
plt.xlim(xmax=650)
plt.ylabel('Bi (mT)')
plt.xlabel('Bo (mT)')
#plt.tight_layout()
#plt.xlim(xmax=50)
#plt.ylim(ymax = 50)
#plt.semilogx()
plt.tight_layout()
plt.savefig('extrapolating_performance_dipole.png')
plt.savefig('extrapolating_performance_dipole.pgf')
plt.close()

#
