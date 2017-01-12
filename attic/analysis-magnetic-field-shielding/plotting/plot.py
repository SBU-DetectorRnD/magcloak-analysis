import matplotlib.pyplot as plt
import numpy as np

# Update figure suplementary titles -- fig.suptitle -- whenever the file
# used by np.genfromtxt (see the following line) is changed.

#input_file_path = "/home/anonymous/Dropbox/Magnetic_Cloaking_Device/LabVIEW/DATA_Gaussmeter/"
input_file_path = "/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/"
input_file = "DataFile_160628_141147.txt"
#input_file = "DataFile_160608_135913.txt"
output_file_base = input_file[0:-4]

t, I, B_int = np.genfromtxt(input_file_path+input_file, unpack=True)

I *= 1.0  # [1e-3 V/mV * 1 A/V]
B_ext = I *  0.839957  # [0.77 mT/A]

#I *= 1e-3*50  # [1e-3 V/mV * 50 A/V]
#B_ext = I * 0.77  # [0.77 mT/A]

# Start zero offset index.
i0 = 0

# Stop zero offset index.
i1 = 165

# Remove background field.
B_int -= np.mean(B_int[i0:i1])
# Negate B_int (to account for Hall sensor orienttion).
B_int *= -1

year = 2000 + int(input_file[9:11])
mounth = int(input_file[11:13])
day = int(input_file[13:15])

hour = int(input_file[16:18])
minute = int(input_file[18:20])
second = int(input_file[20:22])

if hour == 0:
    ampm = "a.m."
    hour = 12
elif hour < 12:
    ampm = "a.m."
elif hour < 23:
    ampm = "p.m."
    hour -= 12
elif hour == 24:
    ampm = "a.m."
    hour = 12
else:
    print "Error converting from 24 hour to 12 hour system."

suptitle = "Measurement Taken {0}-{1}-{2} (Starting at {3}:{4}:{5} {6})".format(
    year, mounth, day, hour, minute, second, ampm)

# ======================================================================
# First Plot

fig, ax = plt.subplots()
fig.suptitle(suptitle)
ax.plot(B_ext[i0:], B_int[i0:], label="4 layer wide in epoxy")
ax.set_title("Internal vs. External Magnetic Field")
ax.set_xlabel("Estimated External Field [mT]")
ax.set_ylabel("Internal Field [mT]")
ax.set_aspect("equal")
ax.set_xlim(right=60)
ax.set_ylim(-1,60)

fig.savefig(output_file_base+"_figure_1.png")

#=======================================================================

dB_int = np.gradient(B_int, t)
dI = np.gradient(I, t)

# Starting index
a = 0

# Stopping index
b = 1301 #number of lines in file 

# ======================================================================
# Second Plot

fig, axarr = plt.subplots(4, sharex=True)

fig.suptitle(suptitle)

axarr[0].set_title("Internal Magnetic Field vs. Time")
axarr[0].set_ylabel("Internal Magnetic Field [mT]")
axarr[0].plot(t[a:b], B_int[a:b])

axarr[1].set_title("Time Derivative of Internal Magnetic Field vs. Time")
axarr[1].set_ylabel("Time Deriv. of Int. Mag. Field [mT/s]")
axarr[1].plot(t[a:b], dB_int[a:b])

axarr[2].set_title("Current vs. Time")
axarr[2].set_ylabel("Current [A]")
axarr[2].plot(t[a:b], I[a:b])

axarr[3].set_title("Time Derivative of Current vs. Time")
axarr[3].set_ylabel("Time Deriv. of Current [A/s]")
axarr[3].plot(t[a:b], dI[a:b])

axarr[-1].set_xlabel("Time [s]")

fig.savefig(output_file_base+"_figure_2.png")

# ======================================================================

dI_threshold = 1e-5
len_threshold = 20

i0lst = []
i1lst = []
i = a
while i < b:
    if (abs(dI[i]) < dI_threshold):
        for j in xrange(i+1, b):
            if (abs(dI[j]) >= dI_threshold):
                if (j - i - 1 > len_threshold):
                    i0lst += [i]
                    i1lst += [j]
                i = j + 1
                break
        else:
            if (j - i - 1 > len_threshold):
                i0lst += [i]
                i1lst += [j]
            i = j + 1
            break
    else:
        i += 1

# ======================================================================
# Third Plot 

fig, ax = plt.subplots()

x = []
y = []
xerr = []
yerr = []

for i in xrange(len(i0lst)):
    x += [np.mean(B_ext[i0lst[i]:i1lst[i]])]
    xerr += [np.std(B_ext[i0lst[i]:i1lst[i]])]
    y += [np.mean(B_int[i0lst[i]:i1lst[i]])]
    yerr += [np.std(B_int[i0lst[i]:i1lst[i]])]
    ax.plot(B_ext[i0lst[i]:i1lst[i]],
            B_int[i0lst[i]:i1lst[i]],
            label="B_ext ~ {:5.2f} mT".format(x[-1]))

fig.suptitle(suptitle)
ax.set_title("Internal vs. External Magnetic Field")
ax.set_xlabel("Estimated External Field [mT]")
ax.set_ylabel("Internal Field [mT]")
ax.set_aspect("equal")
ax.set_xlim(right=60)
ax.set_ylim(-2,60)

fig.savefig(output_file_base+"_figure_3_without_legend.png")

ax.legend(loc="best")

fig.savefig(output_file_base+"_figure_3_with_legend.png")

# ======================================================================
# Fourth Plot

fig, ax = plt.subplots()
fig.suptitle(suptitle)
ax.errorbar(x, y, xerr=xerr, yerr=yerr)
ax.set_title("Internal vs. External Magnetic Field")
ax.set_xlabel("Estimated External Field [mT]")
ax.set_ylabel("Internal Field [mT]")
ax.set_aspect("equal")
ax.set_xlim(right=60)
ax.set_ylim(-5,60)

fig.savefig(output_file_base+"_figure_4.png")

# ======================================================================
# Fifth Plot

x = np.asarray(x)
y = np.asarray(y)
xerr = np.asarray(xerr)
yerr = np.asarray(yerr)

fig, ax = plt.subplots()
fig.suptitle(suptitle)
ax.errorbar(x, x-y, xerr=xerr, yerr=np.sqrt(yerr**2+xerr**2),
            label="data")
ax.set_title("Shielded vs. External Magnetic Field")
ax.set_xlabel("Estimated External Field [mT]")
ax.set_ylabel("Estimated Shielded Field [mT]")
ax.set_aspect("equal")
ax.set_xlim(right=60)
ax.set_ylim(top=60)

xlim = ax.get_xlim()
ylim = ax.get_ylim()
line = [min(xlim[1], ylim[1]), max(xlim[0], ylim[0])]

ax.plot(line, line, color="k", label="one-to-one line")

ax.legend(loc='best')

fig.savefig(output_file_base+"_figure_5.png")

# ======================================================================

plt.show()
#plt.close()
