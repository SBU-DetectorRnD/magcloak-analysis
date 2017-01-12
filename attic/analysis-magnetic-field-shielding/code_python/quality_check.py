import numpy as np

def initial_quality(pre_cool_offset, calibration):
    M = np.genfromtxt(pre_cool_offset)
    N = np.genfromtxt(calibration)

    calibration_current = N[:,1]
    calibration_field = N[:,2]
    offset = np.mean(M[:,2])

    if abs(offset) > 0.02:
        print('Initial magnetic field too large!')
        return False 

    for i in range(0,np.size(calibration_current)):
        if calibration_field[i] < -1:
            print('Calibration is negative')
            return False

    return True

def calibration_range(calibration, i):
    N = np.genfromtxt(calibration)

    calibration_current = N[:,1]
    if (np.mean(i) < calibration_current[0] or np.mean(i) >
            calibration_current[-1]):
        print("%s \t %s \t %s" %(np.mean(i), calibration_current[0],
            calibration_current[-1]))
        print("Current out of range for calibration.")
        return False
    return True

def fit_quality(p,perr):
    if p[0]+perr[0] < 0:
        print("Decreasing fitted log")
        return False
    if p[1] + perr[1] < 0:
        print("Negative time offset")
        return False
    return True

#def extrapolation_quality(extrapolate_val, bshield, shielding_factor, b0):
#    if (extrapolate_val.s/extrapolate_val.n > 1 or
#            bshield.s/bshield.n > 1 or
#            shielding_factor.s/shielding_factor.n >0.5 or
#            extrapolate_val.n > b0.n):
#        print("Fitting error too large")
#        return False
#    return True
#

def extrapolation_quality(extrapolate_val, b0):
    if (extrapolate_val.n + extrapolate_val.s> b0.n):
        print("Extrapolated value larger than incident field")
        print("Extrapolated value: " + str(extrapolate_val.n))
        print("Incident Field: " + str(b0.n))
        return False
    elif abs(extrapolate_val.n) < extrapolate_val.s:
        print("%s\t%s" %(extrapolate_val.n, extrapolate_val.s))
        print("Fitting error is too high.")
        return False
    return True



