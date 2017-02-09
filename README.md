# MagCloak_Analysis
Analysis code for magnetic cloaking measurements.

### How to get the data:

* In parent directory (cd ..), check out magcloak-data-calib from GitHub

* If you want to calibrate data yourself, also check out magcloak-data from GitHub


### How to calibrate data:

* 'cd calibration'

* 'cd data' and copy files from Dropbox here if they are missing 

* 'cd -' (i.e. go back to MagCloak_Analysis/calibration)

* edit file <filelist> with list of data files you want to calibrate

* run one of these depending on code used to record data:
  ** ipython3 calibrateData_Gaussmeter.py <filelist>
  ** ipython3 calibrateData_MegaVIEW.py <filelist>


