#include "headers/Generate_Summary_File.h"

int Generate_Summary_File()
{

	//Vector of data files. Each one should have only one current setting within it.
	vector<std::string> DataFile_list;

	/*
		DataFile_list.push_back("DataFile_160915_173531.root");
		DataFile_list.push_back("DataFile_160915_174843.root");
		DataFile_list.push_back("DataFile_160915_180238.root");
		DataFile_list.push_back("DataFile_160915_181358.root");
		DataFile_list.push_back("DataFile_160915_182606.root");
	*/


	/*
	//1 layer helmholtz
		DataFile_list.push_back("DataFile_160908_175829.root");
		DataFile_list.push_back("DataFile_160908_180903.root");
		DataFile_list.push_back("DataFile_160908_182016.root");
		DataFile_list.push_back("DataFile_160908_183045.root");
		DataFile_list.push_back("DataFile_160908_184120.root");
		DataFile_list.push_back("DataFile_160908_185155.root");
		DataFile_list.push_back("DataFile_160908_190253.root");
		DataFile_list.push_back("DataFile_160908_191341.root");
		DataFile_list.push_back("DataFile_160908_192409.root");
		DataFile_list.push_back("DataFile_160908_193504.root");
		DataFile_list.push_back("DataFile_160908_194637.root");
		DataFile_list.push_back("DataFile_160908_195722.root");
		DataFile_list.push_back("DataFile_160908_200727.root");
	*/

	/*
		//2016 1-layer overlap dipole new
		DataFile_list.push_back("DataFile_160915_132943.root");
		DataFile_list.push_back("DataFile_160915_134304.root");
		DataFile_list.push_back("DataFile_160915_135540.root");
		DataFile_list.push_back("DataFile_160915_140832.root");
		DataFile_list.push_back("DataFile_160915_141955.root");
		DataFile_list.push_back("DataFile_160915_143134.root");
	*/

	
//		//2016 1-layer overlap dipole old
//		DataFile_list.push_back("DataFile_160915_113128.root");
//		DataFile_list.push_back("DataFile_160915_114427.root");
//		DataFile_list.push_back("DataFile_160915_120025.root");
////		DataFile_list.push_back("DataFile_160915_121427.root");
//		DataFile_list.push_back("DataFile_160915_122955.root");
//		DataFile_list.push_back("DataFile_160915_124429.root");
	
	/*
		//2016 2-layer Dipole
		DataFile_list.push_back("DataFile_160915_151651.root");
		DataFile_list.push_back("DataFile_160915_152931.root");
		DataFile_list.push_back("DataFile_160915_154119.root");
		DataFile_list.push_back("DataFile_160915_155312.root");
	*/


		// Raphael test
		DataFile_list.push_back("DataFile_150318_191056.root");

	vector<int> file_stop;
	for(int i = 0; i < DataFile_list.size(); i++)
	{
		file_stop.push_back(0);
	}

	Summary_File_Single( 
//			"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/", 
			"../calibration/data_calib/", 
			"./SummaryFiles/", 
			DataFile_list, 
			"Dipole", 
			"True", 
			file_stop);

	return 0;
}
