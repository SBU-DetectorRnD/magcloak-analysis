#!/usr/bin/env python3

import evaluate_shielding as es

# 45 layer SC shield in MRI magnet
es.evaluate_shielding_for_set( setlist = "filelist_mri_shielding_45layer.txt", results_file = "results/mri_shielding_45layer.csv")


# 1 layer AMSC wide tape sheath in dipole (rc2015)
es.evaluate_shielding_for_set( setlist = "filelist_dipole_shielding_sheath_1layer.txt", results_file = "results/dipole_shielding_sheath_1layer.csv")
