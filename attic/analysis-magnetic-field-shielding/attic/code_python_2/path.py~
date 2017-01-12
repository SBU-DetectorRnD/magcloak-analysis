#!/usr/bin/env python

#Method to defines the paths to every necessary file.

import os

masterpath = '/home/raphael/Dropbox/MagneticCloakingDevice/MagneticFieldCloakingDevice/BvsH/'
analysisfolder = masterpath + 'analysis/'
pythonfolder = analysisfolder + 'python/'

def paths(sets, no_delete = False):
    paths = []
    with open(sets) as f:
        line = f.readline()
        while line:
            if line[0] != '#':
                paths.append(line.split())
            line = f.readline()

    description = []
    datafolder = []
    datalist = []
    pre_cool_offset = []
    post_cool_offset = []
    calibration = []
    sig_gaussmeter = []
    sig_multimeter = []
    fit_graphs = []
    fit_file = []
    param_fit_file = []

    for ind, i in enumerate(paths):
        datafolder.append(masterpath + paths[ind][1])
        description.append(paths[ind][0])
        datalist.append(datafolder[ind] + paths[ind][2])
        pre_cool_offset.append(datafolder[ind] + paths[ind][3])
        post_cool_offset.append(datafolder[ind] + paths[ind][4])
        calibration.append(datafolder[ind] + paths[ind][5])
        sig_gaussmeter.append(analysisfolder + paths[ind][6])
        sig_multimeter.append(analysisfolder + paths[ind][7])
        analysis_subfolder = pythonfolder + paths[ind][0] + '/'
        fit_graphs.append(analysis_subfolder + 'fit_graphs/')
        fit_file.append(analysis_subfolder + description[ind] + '_fit_results.txt')
        param_fit_file.append(analysis_subfolder + 'param_fit_results.txt')
    
        if no_delete:
            continue
        if not os.path.isdir(fit_graphs[ind]):
            os.makedirs(fit_graphs[ind])

        if os.path.isfile(fit_file[ind]):
            os.remove(fit_file[ind])

        if os.path.isfile(param_fit_file[ind]):
            os.remove(param_fit_file[ind])
    return (description, datafolder, datalist, pre_cool_offset, 
            post_cool_offset, calibration, sig_gaussmeter, sig_multimeter,
            fit_graphs, fit_file, param_fit_file)
    
