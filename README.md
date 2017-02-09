# MagCloak_Analysis
Analysis code for magnetic cloaking measurements.

How to calibrate data

0) cd ..

1) check out magcloak-data and magcloak-data-calib from Github

2) cd magcloak-data and copy files from Dropbox here if they are missing 

3) cd ../MagCloak_Analysis/calibration

4) create link named 'data' to magcloak-data and link named 'data-calib' to magcloak-data-calib

5) edit file with list of data files you want to calibrate

6) run one of these:
       ipython3 calibrateData_Gaussmeter.py <filelist>
       ipython3 calibrateData_MegaVIEW.py <filelist>


