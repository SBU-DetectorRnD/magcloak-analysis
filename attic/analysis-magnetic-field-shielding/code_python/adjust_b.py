import numpy as np
from uncertainties import ufloat
from uncertainties import unumpy
from add_uncertainties import quad_add

def calc_sig_b_dev(b,gaussmeter_file):
    sig_gauss = np.genfromtxt(gaussmeter_file)
    sig_b = np.zeros(len(b))
    for j in range(len(b)):
        for k in range(len(sig_gauss[:,0])):
            if (abs(b[j]) < sig_gauss[k,0]):
                sig_b[j] = quad_add([sig_gauss[k,1], sig_gauss[k,2]])
                break
    return sig_b

def calc_sig_b(b,gaussmeter_file):
    sig_gauss = np.genfromtxt(gaussmeter_file)
    sig_b = np.zeros(len(b))
    for j in range(len(b)):
        if (abs(b[j]) < sig_gauss[0,0]):
            sig_b[j] = quad_add([sig_gauss[0,1],sig_gauss[0,2]])
        if (abs(b[j]) > sig_gauss[0,0] and 
                abs(b[j]) < sig_gauss[1,0]):
            sig_b[j] = quad_add([sig_gauss[1,1],sig_gauss[1,2]])
        if (abs(b[j]) > sig_gauss[1,0]):
            sig_b[j] = quad_add([sig_gauss[2,1],sig_gauss[2,2]])
    return sig_b

def calc_offsets(precool_file, postcool_file):
    M = np.genfromtxt(precool_file)
    N = np.genfromtxt(postcool_file)
    
    precool_offset = ufloat(np.mean(M[:,2]), np.std(M[:,2]))
    postcool_offset = ufloat(np.mean(N[:,2]), np.std(N[:,2]))

    return (precool_offset, postcool_offset)

def calc_b_leak(b, gaussmeter_file, precool_file, postcool_file):
    sig_b = calc_sig_b_dev(b, gaussmeter_file)
    b = unumpy.uarray(b, calc_sig_b(b, gaussmeter_file))
    [b_before_cool, b_after_cool] = calc_offsets(precool_file, postcool_file)
    return b - b_after_cool
