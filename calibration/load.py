#!/usr/bin/env python3

from os.path import expanduser, join, normpath

import numpy as np

class LeaflistError(Exception):
    '''Exception for errors involving the leaflist of a data file.'''

# Root leaflist variable type to NumPy dtype conversion dictionary.
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

def MegaVIEW(fname, drop=True, droppath=None):
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
    out : ndarray
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



def Gaussmeter(fname, drop=True, droppath=None):
    '''Reads in Gaussmeter LabVIEW data file.
    
    Parameters
    ----------
    fname : str
        Name of data file to read in if drop is True, path of data
        file to read in otherwise.
    drop: bool, optional
        If True, treats fname as the name of a file in the
        "DATA_Gaussmeter" directory of DropBox.
        If false, passes fname to open and np.genfromtxt unaltered.
    droppath : str, optional
        The path of the directory containing the Dropbox directory.
        If None, the Dropbox directory is assumed to be located in the
        user's home directory.
        Has no effect if drop is False.
    
    Returns
    -------
    out : ndarray
        "Data read form the text file" by numpy.genfromtxt
        (np.genfromtxt).
    '''
    # Prepend "Data_Gaussmeter" path to fname if drop is True.
    if drop:
        if (droppath is None):
            fname = join(
                expanduser('~'),
                'Dropbox',
                'Stony Brook Research Team Folder',
                'LabVIEW',
                'DATA_Gaussmeter',
                fname
            )
        else:
            fname = join(
                droppath,
                'Dropbox',
                'Stony Brook Research Team Folder',
                'LabVIEW',
                'DATA_Gaussmeter',
                fname
            )


    # Count the entries on each line. If 5, the file is a B vs z. If 3, B vs I. Anything else is an error                                                
    with open(fname, 'r') as f:
        line1 = ""
        for line in f:
            if (line[0] != '#'):
                line1 = line
                break

    ncols = len(line1.split())


    # Name columns as dtype (these files have no header).
    dtype = []
    t = 'F'

    # If "Field vs time / current" measurement
    if ( ncols == 3 ):
        print ("Found 3 Columns")
        dtype = [('time', leaf_type[t]),('multi', leaf_type[t]),('B1', leaf_type[t])]

    # If "Field vs position" measurement
    elif ( ncols == 5 ):
        print ("Found 5 Columns")
        dtype = [('pos', leaf_type[t]),('B1', leaf_type[t]),('B1_sdev', leaf_type[t]),('multi', leaf_type[t]),('multi_sdev', leaf_type[t])]

    else:
        print ("Found neither 3 nor 5 columns, exit")
        sys.exit(1)


    # Read in and return data.
    return np.genfromtxt(
        fname,
        dtype,
        skip_header=False,
        )

