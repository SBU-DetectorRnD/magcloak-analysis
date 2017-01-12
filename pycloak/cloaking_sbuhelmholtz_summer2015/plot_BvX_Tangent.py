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

def figsize(scale):
    fig_width_pt = 345                          # Get this from LaTeX using \the\textwidth
    inches_per_pt = 1.0/72.27                       # Convert pt to inch
    golden_mean = (np.sqrt(5.0)-1.0)/2.0            # Aesthetic ratio (you could change this)
    fig_width = fig_width_pt*inches_per_pt*scale    # width in inches
    fig_height = fig_width*golden_mean              # height in inches
    fig_size = [fig_width,fig_height]
    return fig_size

plt.figure(figsize=figsize(1))


#Vertical Lines marking FM layer region (4)
plt.plot((-31.750/2.0,-31.750/2.0),(0,12),'k--')
plt.plot(( 31.750/2.0, 31.750/2.0),(0,12),'k--')
plt.plot((-39.116/2.0,-39.116/2.0),(0,12),'k--')
plt.plot(( 39.116/2.0, 39.116/2.0),(0,12),'k--')

# center of cylinder in x - different for different setups
xc_1 = 48
xc_2 = 48+5.5

# set axis range
plt.axis([-100, 100, 0, 12])

L = np.genfromtxt('data/DataFile_150707_151753.txt')
plt.xlabel('x (mm)')
plt.ylabel('By (mT)')
x = L[:,0]
y = abs(L[:,1])
sig_y = L[:,2]
plt.errorbar(x-xc_1,y, sig_y, marker = '.', label = 'Reference Field',fmt='o')

R = np.genfromtxt('data/DataFile_150707_170047.txt')
x = R[:,0]
y = abs(R[:,1])
sig_y = R[:,2]
plt.errorbar(x-xc_1,y, sig_y, marker = '.', label = 'SC',fmt='o')

N = np.genfromtxt('data/DataFile_150707_230334.txt')
x = N[:,0]
y = abs(N[:,1])
sig_y = N[:,2]
plt.errorbar(x-xc_2,y, sig_y, marker = '.', label = 'FM',fmt='o')

O = np.genfromtxt('data/DataFile_150708_020249.txt')
x = O[:,0]
y = abs(O[:,1])
sig_y = O[:,2]
plt.errorbar(x-xc_2,y, sig_y, marker = '.', label = 'SC+FM',fmt='o')

plt.legend(loc = 'best')
despine()
plt.tight_layout()
plt.savefig('plots/Graph_BvX_Tangent')
plt.savefig('plots/Graph_BvX_Tangent.pgf')
plt.close()

#plt.show()
