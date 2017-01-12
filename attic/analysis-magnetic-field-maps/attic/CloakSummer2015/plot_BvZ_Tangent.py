import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
import matplotlib.gridspec as gridspec

colors = {'c1': '#5DA5DA', 
        'c2':'#FAA43A',
        'c3':'#60BD68',
        'c4':'#F17CB0',
        'c5':'#B2912F',
        'c6':'#B276B2',
        'c7':'#DECF3F',
        'c8':'#F15854',
        'c9':'#4D4D4D'}

def despine():
    ax = plt.gca()
    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)
    ax.yaxis.set_ticks_position('left')
    ax.xaxis.set_ticks_position('bottom')

#mpl.rcParams['lines.markersize']=3
#mpl.rcParams['lines.linewidth']=1

def figsize(scale):
    fig_width_pt = 345                          # Get this from LaTeX using \the\textwidth
    inches_per_pt = 1.0/72.27                       # Convert pt to inch
    golden_mean = (np.sqrt(5.0)-1.0)/2.0            # Aesthetic ratio (you could change this)
    fig_width = fig_width_pt*inches_per_pt*scale    # width in inches
    fig_height = fig_width*golden_mean              # height in inches
    fig_size = [fig_width,fig_height]
    return fig_size

plt.figure(figsize=figsize(1))

#Vertical Lines (2)
plt.plot((-58,-58),(0.0,12.0),'k--')
plt.plot((58,58),(0.0,12.0),'k--')

plt.axis([-80,80,0,12])

center = 153
L = np.genfromtxt('data/DataFile_150707_145757.txt')
plt.xlabel('Z (mm)')
plt.ylabel('By (mT)')
x = L[:,0] - center
y = abs(L[:,1])
sig_y = L[:,2]
plt.errorbar(x,y, sig_y, marker = '.', label = 'Reference Field',fmt='o')

R = np.genfromtxt('data/DataFile_150707_164646.txt')
x = R[:,0] - center
y = abs(R[:,1])
sig_y = R[:,2]
plt.errorbar(x,y, sig_y, marker = '.', label = 'SC',fmt='o')

N = np.genfromtxt('data/DataFile_150707_222718.txt')
x = N[:,0] - center
y = abs(N[:,1])
sig_y = N[:,2]
plt.errorbar(x,y, sig_y, marker = '.', label = 'FM',fmt='o')

O = np.genfromtxt('data/DataFile_150708_014502.txt')
x = O[:,0] - center
y = abs(O[:,1])
sig_y = O[:,2]
plt.errorbar(x,y, sig_y, marker = '.', label = 'SC+FM',fmt='o')

plt.legend(loc = 'best')
despine()
plt.tight_layout()

plt.savefig('plots/Graph_BvZ_Tangent')
plt.savefig('plots/Graph_BvZ_Tangent.pgf')
plt.close()
