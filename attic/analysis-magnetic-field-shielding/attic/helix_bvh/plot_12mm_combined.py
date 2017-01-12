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


mpl.rcParams['lines.markersize']=4
mpl.rcParams['lines.linewidth']=1

y_ind = 2
sig_y_ind = 3

layer1_sheath = 'shielding_results/1layer_sheath_superpower.txt'
layer1_helix = 'shielding_results/1layer_helix_fit_results.txt'
layer2_helix_adjacent = 'shielding_results/2layer_helix_adjacent_fit_results.txt'
layer2_helix_spaced = 'shielding_results/2layer_helix_spaced_fit_results.txt'
layer4_helix_adjacent = 'shielding_results/4layer_helix_adjacent_fit_results.txt'
layer4_helix_spaced = 'shielding_results/4layer_helix_spaced_fit_results.txt'
layer5_helix_cu = 'shielding_results/5layer_helix_adjacent_cu.txt'
layer5_helix_no_cu = 'shielding_results/5layer_no_cu_fit_results.txt'
layer1_wide = 'shielding_results/1layer_wide_sc_fit_results.txt'

files1 = [layer1_sheath, layer1_helix, 
        layer2_helix_adjacent,layer2_helix_spaced,
         layer4_helix_adjacent,layer4_helix_spaced ]


legend1 = ['1 layer sheath', '1 layer helix',  
        '2 layer adjacent','2 layer separate', 
         '4 layer adjacent','4 layer spaced',
        '5 layer adjacent', '5 layer adjacent w/o Cu']

marker1 = ['.', 'd', '*', 'x', 's', 'o']
style1 = [':','-','-',':','-', ':']

gs = gridspec.GridSpec(1,2,  width_ratios=[1.6,1])
plt.figure(figsize=figsize(1))
ax1 = plt.subplot(gs[0])

for ind in range(0,len(files1)):
    M = np.genfromtxt(files1[ind])
    xcut = 27
    ind_cut = np.where(M[:,0]<xcut)[0]
    plt.errorbar(M[ind_cut,0], M[ind_cut,y_ind], M[ind_cut,sig_y_ind], label =
            legend1[ind],
            marker = marker1[ind], ls = style1[ind])

plt.legend(bbox_to_anchor=(0.45,1), bbox_transform=plt.gcf().transFigure)
despine()
plt.ylim((0,25))
plt.xlim(xmax=27)
plt.xlabel('Bo (mT)')
plt.ylabel('Bi (mT)')
plt.tight_layout()

files2 = [layer1_sheath,layer1_wide, layer5_helix_cu, layer5_helix_no_cu]

ax2 = plt.subplot(gs[1])

color2 = [colors['c1'], colors['c7'],colors['c8'],colors['c9']]
marker2 = ['.',  '*', 's', 'd']
style2 = [':','-',':','-']
legend2 = ['1 layer sheath', '1 layer wide',  
        '5 layer w/ Stabilizer','5 layer w/o Stabilizer']



for ind in range(0,len(files2)):
    M = np.genfromtxt(files2[ind])
    xlow = 10
    xhigh = 27
    ind_cut = (np.where((M[:,0] < xhigh) & (M[:,0] > xlow)))[0]
    plt.errorbar(M[ind_cut,0], M[ind_cut,y_ind], M[ind_cut,sig_y_ind], label =
            legend2[ind],
            marker = marker2[ind], ls = style2[ind], color =color2[ind])


plt.legend(bbox_to_anchor=(1,1), bbox_transform=plt.gcf().transFigure)
despine()
plt.ylim((0,25))
plt.xlim(xmax=27)
plt.xlabel('Bo (mT)')
ax2.spines['left'].set_visible(False)
ax2.xaxis.set_ticks_position('bottom')
ax2.yaxis.set_ticks_position('none')
ax2.yaxis.set_visible(False)
plt.tight_layout()


plt.savefig('helix_wrapping_combined.png')
plt.savefig('helix_wrapping_combined.pgf')
plt.close()

