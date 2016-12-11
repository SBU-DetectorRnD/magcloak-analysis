#!/usr/bin/env python3

import numpy as np

def update(mean_arr, std_arr, data, var, dtype, names, a, b=None):
    '''Updates mean and std arrays.
    
    Parameters
    ----------
    mean_arr : ndarray
        Structured array of means to be updated.
    std_arr : ndarray
        Structured array of standard deviations to be updated.
    data : ndarray
        Structured array of data.
    var : str or list of str
        Field names of data that statistical calculations are not
        performed for.
    dtype : dtype
        Data type of the ndarrays mean and std.
    names : list of str
        Field names of data that statistical calculations are performed
        for.
    a : int
        First index of statistical calculation slice.
    b : int, optional
        If not None, the second index of statistical calculation slice.
        If None, the statistical calculations are performed for data[a]
        instead of data[a:b]. 
    '''
    if (b is not None):
        s = slice(a, b)
    else:
        s = slice(a, a+1)
    mean_arr = np.append(
        mean_arr,
        np.array(
            (
                *data[var][a],
                *[np.mean(data[name][s]) for name in names],
                ),
            dtype=dtype,
            ),
        )
    std_arr = np.append(
        std_arr,
        np.array(
            (
                *data[var][a],
                *[np.std(data[name][s]) for name in names],
                ),
            dtype=dtype,
            ),
        )
    return mean_arr, std_arr

def map_stats(data):
    '''Performes statistical calculations for field maps.
    
    Calculates the mean and standard deviation of the readings taken at
    each location of the field map.
    
    Parameters
    ----------
    data : ndarray
        Structured array of map data.
    
    Returns
    -------
    mean_arr : ndarray
        Structured array of map means.
    std_arr : ndarray
        Structured array of map standard deviations.
    '''
    N = len(data)
    var = ['x', 'y', 'z']
    names = list(data.dtype.names)
    for v in var:
        names.remove(v)
    dtype = [
        *[(v, data[v].dtype) for v in var],
        *[(name, data[name].dtype) for name in names],
        ]
    dtype = np.dtype(dtype)
    mean_arr = np.empty(0, dtype=data.dtype)
    std_arr = np.empty(0, dtype=data.dtype)
    a = 0
    while (a < N):
        print('a:', a)
        if (a == N - 1):
            print('    a == N - 1')
            mean_arr, std_arr = update(
                mean_arr, std_arr, data, var, dtype, names, a
                )
            break
        for b in range(a+1, N):
            print('b:', b)
            if not np.array_equal(
                    data[var][a], data[var][b]):
                print('    not equal')
                mean_arr, std_arr = update(
                    mean_arr, std_arr, data, var, dtype, names, a, b
                    )
                break
            elif (b == N - 1):
                print('    b == N - 1')
                mean_arr, std_arr = update(
                    mean_arr, std_arr, data,  var, dtype, names, a, N
                    )
                break
        a = b + 1
    return mean_arr, std_arr
