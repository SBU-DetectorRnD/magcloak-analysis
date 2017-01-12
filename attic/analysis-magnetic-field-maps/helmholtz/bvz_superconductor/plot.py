import numpy as np
import matplotlib.pyplot as plt

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
        'c8':'#DECF3F',
        'c9':'#F15854',
        'c10':'#4D4D4D',}


reference5mt = 'reference_5mT.txt'
reference40mt = 'reference_40mT.txt'
layer1_helix = '1layer_helix.txt'
layer2_helix = '2layer_helix.txt'
layer4_helix = '4layer_helix.txt'
layer1_vertical = '1layer_vertical.txt'
layer4_vertical_5mt = '4layer_vertical_5mT.txt'
layer4_vertical_40mt = '4layer_vertical_40mT.txt'

helix_wrap = [reference5mt, layer1_helix, layer2_helix, layer4_helix,
        layer1_vertical]
wide_tape = [reference5mt, layer1_vertical, layer4_vertical_5mt, reference40mt,
        layer4_vertical_40mt]

description_helix = ['Reference Field', '1 layer helix', '2 layer helix', 
        '4 layer helix', '1 layer wide tape']

description_wide =  ['Reference Field: 5 mT', '1 wide tape: 5 mT', 
        '4 layer wide tape: 5 mT', 
        'Reference Field: 40 mT', '4 layer wide tape: 40 mT']

ls_helix = ['-',':','--','-.','-']
lw_helix = [1,2,2,2,2]
ls_wide = ['-',':','--','-','-.']
lw_wide = [1,2,2,1,2]

center = [151,0, 151, 151, 151]

fig = plt.figure(figsize=figsize(1))
for ind in range(0,len(helix_wrap)):
    M = np.genfromtxt(helix_wrap[ind])
    x = M[:,0]- center[ind]
    y = M[:,1]
    sig_y = M[:,2]
    plt.errorbar(x,y,yerr = sig_y, label = description_helix[ind], 
            ls = ls_helix[ind], lw = lw_helix[ind])
    
plt.legend(loc = 'center')

#plt.title('Field Leaking through Superconducting Tubes')
plt.ylabel('Bi (mT)')
plt.xlabel('z (mm)')
plt.ylim([-0.1,5.2]) 
plt.tight_layout()
despine()
plt.savefig('helix_shielding.png')
plt.savefig('helix_shielding.pgf')
plt.close()

fig = plt.figure(figsize=figsize(1))
for ind in range(0,len(wide_tape)):
    M = np.genfromtxt(wide_tape[ind])
    x = M[:,0]-151
    y = M[:,1]
    sig_y = M[:,2]
    plt.errorbar(x,y,yerr = sig_y, label = description_wide[ind],
            ls = ls_wide[ind], lw = lw_wide[ind])
plt.legend(loc = 'center')

#plt.title('Field Leaking through Superconducting Tubes')
plt.ylabel('Bi [mT]')
plt.xlabel('z [mm]')
plt.ylim([-0.1,41]) 
despine()
plt.tight_layout()
plt.savefig('vertical_shielding.png')
plt.savefig('vertical_shielding.pgf')
plt.close()



