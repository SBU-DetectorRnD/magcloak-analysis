#include "headers/get_calibration.h"

int get_calibration_factor()
{

	std::string file_path = "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160726_133846.txt";

	double calibration_factor = get_calibration(file_path);
	if (calibration_factor == 0) 
	{
		cerr << "Calibration Not Found" << endl;
		return -1;
	}

	cout << endl << "The Calibration Factor for this setup is: " << TMath::Abs(calibration_factor) << " mT/mV" << endl;
	cout << "Consult the shunt resistor you are using for the mV<->A conversion factor (most commonly 1:1)" << endl;

	return 0;
}
