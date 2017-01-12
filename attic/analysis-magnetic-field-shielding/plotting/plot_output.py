import matplotlib.pyplot as plt
import numpy as np
import sys
import os.path
#Requires pyroot to be installed
import ROOT

# Update figure suplementary titles -- fig.suptitle -- whenever the file
# used by np.genfromtxt (see the following line) is changed.

input_file_path = sys.argv[1]
input_file = sys.argv[2]
#input_file_path = "/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/"
output_file_base = input_file[0:-4]

if(os.path.isfile(input_file_path+input_file)):
	print "       PYTHON: Input file found"
else:
	print "       PYTHON: Input file NOT found. Aborting."
	sys.exit()

t, I, B_int = np.genfromtxt(input_file_path+input_file, unpack=True)

#Uncomment if field is negative
if(sys.argv[3] == "True"):
	B_int = -1*B_int

#I *= 1e-3*50  # [1e-3 V/mV * 50 A/V]
#B_ext = I *  0.839957  # [0.77 mT/A]

#Helmholtz Calibration off Shunt Resistor
if(sys.argv[4] == "Helmholtz"):
	I *= 1.0  # [1e-3 V/mV * 1 A/V]
	calibration = 0.839957
	B_ext = I * calibration

#Solenoid Calibration off Shunt Resistor
elif(sys.argv[4] == "Solenoid"):
	I *= 1.0  # [1e-3 V/mV * 1 A/V]
	calibration =  24.5  # [0.77 mT/A]
	B_ext = I * calibration
#Small (Blue) Dipole Calibration off Shunt Resistor
elif(sys.argv[4] == "Dipole"):
	I *= 1.0  # [1e-3 V/mV * 1 A/V]
	calibration = 19.6124  # [0.77 mT/A]
	B_ext = I * calibration
elif(sys.argv[4] == "DipoleNew"):
	I *= 1.0  # [1e-3 V/mV * 1 A/V]
	calibration = 20.3989  # [0.77 mT/A]
	B_ext = I * calibration
elif(sys.argv[4] == "SmallBlack"):
	I *= 1.0  # [1e-3 V/mV * 1 A/V]
	calibration =  19.5361  # [0.77 mT/A]
	B_ext = I * calibration
else:
	print "ERROR: Calibration not found"
	I *= 0
	B_ext = I *0

# Start zero offset index.
i0 = 0

# Stop zero offset index.
i1 = 10

# Remove background field.
#If you didn't zero your probe in ambient field, you can use this to save your ass.
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
ax.set_xlim(right=300)
ax.set_ylim(-1,300)

#fig.savefig(output_file_base+"_figure_1.png")

#=======================================================================

dB_int = np.gradient(B_int, t)
dI = np.gradient(I, t)

# Starting index
a = 0

# Stopping index

def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

#Default is length of file.
if(int(sys.argv[5]) == 0):
	b = file_len(input_file_path+input_file)
else:
	b = int(sys.argv[5])
	print "       PYTHON: Only processing file to line: " + sys.argv[5]

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

#fig.savefig(output_file_base+"_figure_2.png")

# ======================================================================

dI_threshold = 5e-6
len_threshold = 10

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

xroot = np.zeros(1, dtype=float)
xerrroot = np.zeros(1, dtype=float)
yroot = np.zeros(1, dtype=float)
yerrroot = np.zeros(1, dtype=float)
calibration_A_to_mT = np.zeros(1, dtype=float)
current_average = np.zeros(1, dtype=float)
current_err = np.zeros(1, dtype=float)


f = ROOT.TFile('./SummaryFiles/SummaryOf_'+input_file[:len(input_file)-3]+'root', "recreate")
t = ROOT.TTree("t", "tree")
t.Branch('Bo', xroot, 'Bo/D')
t.Branch('BoErr', xerrroot, 'BoErr/D')
t.Branch('Bi', yroot, 'Bi/D')
t.Branch('BiErr', yerrroot, 'BiErr/D')
t.Branch('calibrationFactor', calibration_A_to_mT, 'calibrationFactor/D')
t.Branch('I', current_average, 'I/D')
t.Branch('IErr', current_err, 'IErr/D')


for i in xrange(len(i0lst)):
    x += [np.mean(B_ext[i0lst[i]:i1lst[i]])]
    xroot[0] = np.mean(B_ext[i0lst[i]:i1lst[i]])
    xerr += [np.std(B_ext[i0lst[i]:i1lst[i]])]
    xerrroot[0] = np.std(B_ext[i0lst[i]:i1lst[i]])
    y += [np.mean(B_int[i0lst[i]:i1lst[i]])]
    yroot[0] = np.mean(B_int[i0lst[i]:i1lst[i]])
    yerr += [np.std(B_int[i0lst[i]:i1lst[i]])]
    yerrroot[0] = np.std(B_int[i0lst[i]:i1lst[i]])
    ax.plot(B_ext[i0lst[i]:i1lst[i]],
            B_int[i0lst[i]:i1lst[i]],
            label="B_ext ~ {:5.2f} mT".format(x[-1]))
    calibration_A_to_mT[0] = calibration
    current_average[0] = (np.mean(B_ext[i0lst[i]:i1lst[i]]))/calibration
    current_err[0] = (np.std(B_ext[i0lst[i]:i1lst[i]]))/calibration
    t.Fill()

f.Write()
f.Close()

fig.suptitle(suptitle)
ax.set_title("Internal vs. External Magnetic Field")
ax.set_xlabel("Estimated External Field [mT]")
ax.set_ylabel("Internal Field [mT]")
ax.set_aspect("equal")
ax.set_xlim(right=300)
ax.set_ylim(-2,300)

#fig.savefig(output_file_base+"_figure_3_without_legend.png")

ax.legend(loc="best")

#fig.savefig(output_file_base+"_figure_3_with_legend.png")

# ======================================================================
# Fourth Plot

#f = open('./SummaryFiles/SummaryOf_'+input_file,'w')
#for i in xrange(len(i0lst)):
#    f.write(str(x[i]))
#    f.write(" ")
#    f.write(str(xerr[i]))
#    f.write(" ")
#    f.write(str(y[i]))
#    f.write(" ")
#    f.write(str(yerr[i]))
#    f.write(" \n")

fig, ax = plt.subplots()
fig.suptitle(suptitle)
ax.errorbar(x, y, xerr=xerr, yerr=yerr)
ax.set_title("Internal vs. External Magnetic Field")
ax.set_xlabel("Estimated External Field [mT]")
ax.set_ylabel("Internal Field [mT]")
ax.set_aspect("equal")
ax.set_xlim(right=120)
ax.set_ylim(-5,120)

#fig.savefig(output_file_base+"_figure_4.png")

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
ax.set_xlim(right=120)
ax.set_ylim(top=120)

xlim = ax.get_xlim()
ylim = ax.get_ylim()
line = [min(xlim[1], ylim[1]), max(xlim[0], ylim[0])]

ax.plot(line, line, color="k", label="one-to-one line")

ax.legend(loc='best')

#fig.savefig(output_file_base+"_figure_5.png")

# ======================================================================

#plt.show()
#plt.close()
