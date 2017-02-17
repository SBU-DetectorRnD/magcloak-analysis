import os
import sys

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from matplotlib import cm

# set plotting style
import mycolors
mcol = mycolors.pub17
plt.style.use("../style_pub17/cloak17_paper.mplstyle")

#cb_dark_blue = (0/255,107/255,164/255)
#cb_orange = (255/255, 128/255, 14/255)
#cb_gray1 = (171/255,171/255, 171/255)
#cb_gray2 = (89/255, 8/255, 89/255)
#cb_med_blue = (95/255, 158/255, 209/255)
#cb_brown = (200/255, 82/255, 0/255)
#cb_gray3 = (137/255, 137/255, 137/255)
#cb_light_blue = (162/255, 200/255, 236/255)
#cb_light_orange = (255/255, 188/255, 121/255)
#cb_gray_4 = (207/255, 207/255, 207/255)
#
#colors=[]
#colors.append(cb_dark_blue)
#colors.append(cb_orange)
#colors.append(cb_gray1)
#colors.append(cb_gray2)
#colors.append(cb_med_blue)
#colors.append(cb_brown)
#colors.append(cb_gray3)
#colors.append(cb_light_blue)
#colors.append(cb_light_orange)
#colors.append(cb_gray_4)

scanlist="filelist_sbu_angle.txt"

angle_offset=0 #-2.785
center_offset=142

Bref = 0
nfiles = 0
idx = 0

# open input lines                                                                            
f = open(scanlist, "r")
parlines = f.read().splitlines()

ax = plt.subplot(1, 1, 1)

# mark cloak dimensions
plt.axvline(-2.032, color=mcol[2], linestyle='-',alpha=0.5)
plt.axvline(2.032, color=mcol[2], linestyle='-',alpha=0.5)

#plt.axhline( Bref, color='grey' , linestyle='-', alpha=0.5)
#plt.axvline( 0, color='grey' , linestyle='-', alpha=0.5)

# loop over data files
for parline in parlines:

    if not ( parline.startswith("#") ):
        pars = parline.split()

        if len(pars) == 2:
            angle=round(float(pars[0])-angle_offset)
            print( angle, pars[1] )

            df_i = pd.read_csv(pars[1])

            Bref += (-1*df_i['Bnom'].mean())
            nfiles += 1

#print( "Breference (mT) = ", Bref)

            if ( idx < 5 ):
                plt.plot((df_i['pos']-center_offset)/10,-1*df_i['B1'],label=angle,color=mcol[idx])
            else:
                plt.plot((df_i['pos']-center_offset)/10,-1*df_i['B1'],label=angle,color=mcol[idx],linestyle='--')

            idx +=1 

Bref /= nfiles
print( "Breference (mT) = ", Bref)

# plot cosmetics: set axis parameters
#plt.title("Angular dependence")
plt.xlabel('position (cm)')
plt.ylabel('$B_T$ (mT)')


# add legend
plt.legend(loc = 'upper left', ncol=2, title = "angles $\Theta$ (degrees)", fontsize=12)
ax.get_legend().get_title().set_fontsize(12)

plt.savefig("plots/eps/cloak_sbu_angles.eps")
plt.savefig("plots/png/cloak_sbu_angles.png")
plt.show()

