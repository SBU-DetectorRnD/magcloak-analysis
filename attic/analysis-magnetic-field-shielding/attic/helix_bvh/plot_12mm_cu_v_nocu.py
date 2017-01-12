import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl

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

plt.figure(figsize=figsize(1))

layer1_sheath = 'shielding_results/1layer_sheath_superpower.txt'
layer1_helix = 'shielding_results/1layer_helix_fit_results.txt'
layer2_helix_adjacent = 'shielding_results/2layer_helix_adjacent_fit_results.txt'
layer2_helix_spaced = 'shielding_results/2layer_helix_spaced_fit_results.txt'
layer4_helix_adjacent = 'shielding_results/4layer_helix_adjacent_fit_results.txt'
layer4_helix_spaced = 'shielding_results/4layer_helix_spaced_fit_results.txt'
layer5_helix_cu = 'shielding_results/5layer_helix_adjacent_cu.txt'
layer5_helix_no_cu = 'shielding_results/5layer_no_cu_fit_results.txt'
layer1_wide = 'shielding_results/1layer_wide_sc_fit_results.txt'

y_ind = 2
sig_y_ind = 3
files = [layer5_helix_cu, layer5_helix_no_cu]

legend = ['5 layer adjacent', '5 layer adjacent w/o Cu']

colors = ['b','g','r', 'k','c','m', 'b','g','r','k','c', 'm']
marker = [ '.','*', '^', 'v', '<', '>', '*','.','s', 's', '^', 'v', '<', '>', '*']
styles = ['-', '--']

for ind in range(0,len(files)):
    M = np.genfromtxt(files[ind])
    plt.errorbar(M[:,0], M[:,y_ind], M[:,sig_y_ind], label = legend[ind],
            marker = marker[ind], ls = styles[ind])

plt.legend(loc = 'best')
#plt.semilogx()
plt.ylim((-0.01,20))
plt.xlim(xmax= 25)
plt.xlabel('Bo (mT)')
plt.ylabel('Bi (mT)')
despine()
plt.tight_layout()
plt.savefig('helix_wrapping_cu_v_nocu.png')
plt.savefig('helix_wrapping_cu_v_nocu.pgf')
plt.close()

