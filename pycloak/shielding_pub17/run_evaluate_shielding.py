#!/usr/bin/env python3

import evaluate_shielding as es

# 45 layer SC shield in MRI magnet
es.evaluate_shielding_for_set( setlist = "filelist_mri_shielding_45layer.txt", results_file = "results/mri_shielding_45layer.csv")


# 1 layer AMSC wide tape sheath in dipole (rc2015)
es.evaluate_shielding_for_set( setlist = "filelist_dipole_shielding_sheath_1layer.txt", results_file = "results/dipole_shielding_sheath_1layer.csv")

# 1-4 layers Cylinder in Helmholtz
es.evaluate_shielding_for_set( setlist = "filelist_helmholtz_1layer_rc2015.txt", results_file = "results/helmholtz_1layer_rc2015.csv")
es.evaluate_shielding_for_set( setlist = "filelist_helmholtz_2layer_rc2015.txt", results_file = "results/helmholtz_2layer_rc2015.csv")
es.evaluate_shielding_for_set( setlist = "filelist_helmholtz_3layer_rc2015.txt", results_file = "results/helmholtz_3layer_rc2015.csv")
es.evaluate_shielding_for_set( setlist = "filelist_helmholtz_4layer_rc2015.txt", results_file = "results/helmholtz_4layer_rc2015.csv")


# 2 layers Van de Graaff prototype
es.evaluate_shielding_for_set( setlist = "filelist_shielding_vdg2layer.txt", results_file = "results/shielding_vdg2layer.csv")

