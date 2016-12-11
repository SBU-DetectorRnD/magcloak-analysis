#!/usr/bin/env python3

import numpy as np

def basic_map(B, R, S, F, C, main=['B1', 'B1', 'B1', 'B1', 'B1']):
    '''Basic performance calculations for field map.
    
    Parameters
    ----------
    B : ndarray
        Background -- measurement with (xxx)
            * external magnetic field absent,
            * shield absent, and
            * ferromagnet absent.
    R : ndarray
        Reference -- measurement with (Mxx)
            * external magnetic field applied,
            * shield absent, and
            * ferromagnet absent.
    S : ndarray
        Shielding -- measurement with (MSx)
            * external magnetic field applied,
            * shield active, and
            * ferromagnet absent.
    F : ndarray
        Ferromagnet -- measurement with (MxF)
            * external magnetic field applied,
            * shield absent, and
            * ferromagnet present.
    C : ndarray
        Cloaking -- measurement with (MSF)
            * external magnetic field applied,
            * shield active, and
            * ferromagnet present.
    
    Returns
    -------
    bmap : dictionary of ndarrays
        Dictionary containing performance metrics.
    '''
    if check_pos(B, R, S, F, C):
        applied = R[main[1]] - B[main[0]]
        leakage = S[main[2]] - B[main[0]]
        shielding = R[main[1]] - S[main[2]]
        cloaking = C[main[4]] - R[main[1]]
        bmap = np.empty(
            len(B['x']),
            dtype=[
                ('x', B['x'].dtype),
                ('y', B['y'].dtype),
                ('z', B['z'].dtype),
                ('applied', applied.dtype),
                ('leakage', leakage.dtype),
                ('shielding', shielding.dtype),
                ('cloaking', cloaking.dtype),
                ],
            )
        bmap['x'] = B['x']
        bmap['y'] = B['y']
        bmap['z'] = B['z']
        bmap['applied'] = applied
        bmap['leakage'] = leakage
        bmap['shielding'] = shielding
        bmap['cloaking'] = cloaking
        return bmap
    else:
        return {}

def check_pos(*args):
    '''Checks that the positions for the arguments match.'''
    N = len(args)
    for i in range(1, N):
        if not np.array_equal(
                args[0][['x', 'y', 'z']], args[i][['x', 'y', 'z']]):
            return False
    return True
