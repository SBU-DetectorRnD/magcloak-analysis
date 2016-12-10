#!/usr/bin/env python3

from os.path import expanduser, join, normpath

import numpy as np

class LeaflistError(Exception):
    '''Exception for errors involving the leaflist of a data file.'''

leaf_type = {
    'C' : np.str_,
    'B' : np.int8,
    'b' : np.uint8,
    'S' : np.int16,
    's' : np.uint16,
    'I' : np.int32,
    'i' : np.uint32,
    'F' : np.float32,
    'D' : np.float64,
    'L' : np.int64,
    'l' : np.uint64,
    'O' : np.bool_,
}

def MegaVIEW(fname, drop=True, dorppath=None):
    '''Reads in MegaVIEW data file.
    
    Parameters
    ----------
    fname : str
        Name of data file to read in if drop is True, path of data
        file to read in otherwise.
    drop: bool, optional
        If True, treats fname as the name of a file in the
        "DATA_MegaVIEW" directory of DropBox.
        If false, passes fname to open and np.genfromtxt unaltered.
    droppath : str, optional
        The path of the directory containing the Dropbox directory.
        If None, the Dropbox directory is assumed to be located in the
        user's home directory.
        Has no effect if drop is False.
    
    Returns
    -------
    out : narray
        "Data read form the text file" by numpy.genfromtxt
        (np.genfromtxt).
    '''
    # Prepend "Data_MegaVIEW" path to fname if drop is True.
    if drop:
        if (droppath is None):
            fname = join(
                expanduser('~'),
                'Dropbox',
                'Stony Brook Research Team Folder',
                'LabVIEW',
                'DATA_MegaVIEW',
                fname
            )
        else:
            fname = join(
                droppath,
                'Dropbox',
                'Stony Brook Research Team Folder',
                'LabVIEW',
                'DATA_MegaVIEW',
                fname
            )
    # Find formatting line (leaflist).
    with open(fname, 'r') as f:
        skip_header = 0
        for line in f:
            skip_header += 1
            if (line[0] != '#'):
                leaflist = line
                break
    # Parse leaflist into dtype.
    leaflist = leaflist.strip().split(':')
    dtype = []
    t = 'F'
    for leaf in leaflist:
        if (leaf == ''):
            raise LeaflistError('leaf in leaflist is empty.')
        i = leaf.find('/')
        if (i > 0):
            t = leaf[i+1]
            dtype += (leaf[:i], leaf_type[t]),
        elif (i == 0):
            raise LeaflistError(
                'leaf in leaflist does not contain a variable name.'
            )
        else:
            dtype += (leaf, leaf_type[t]),
    # Read in and return data.
    return np.genfromtxt(
        fname,
        dtype,
        skip_header=skip_header,
    )
