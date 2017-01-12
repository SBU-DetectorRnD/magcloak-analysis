#include "../plotting/headers/get_calibration.h"
#include "../plotting/headers/exist_test.h"
#include "DataFile_to_Root.h"

int DataFile_to_Root(
		     std::string file_name = "DataFile_160910_180219.txt",
		     std::string calibration_name = "DataFile_160726_133846.txt"
		     )
{
        //File path and names
        std::string file_path = "data_local/";
				std::string file_path_out = "data_calib/";

	std::string file = file_path + file_name;
	std::string calibration = file_path + calibration_name;

	//Checks to make sure that the files exist at the specified paths
	if(!exist_test(file))
	{
		cerr << "ERROR: DataFile: " << file << " not found." << endl;
		return -1;
	}
	if(!exist_test(calibration))
	{
		cerr << "ERROR: Calibration File: " << file << " not found." << endl;
		return -1;
	}

	//Get calibration factor from file assuming 1:1 mA<->mT conversion
//	double calibration_factor = TMath::Abs(get_calibration_quiet(calibration));
	//double calibration_factor = get_calibration_quiet(calibration);
	double calibration_factor = get_calibration_quiet(calibration);
	double calibration_sign = 1;
	if ( calibration_factor < 0 )
		{
			calibration_sign = -1;
			calibration_factor;
		}

	vector<double> time, current, err_current, InternalField, err_InternalField, position;
	vector<double> ExternalField, err_ExternalField;

	//Count the entries on each line. If 5, the file is a B vs z. If 3, B vs I. Anything else is an error
	std::ifstream filestream(file.c_str());
	std::string s;
	getline(filestream,s);
	int w = 0;
//	int l = strlen(s.c_str());

	TString ts(s);
	TString tok;
	Ssiz_t tokfrom = 0;
	while (ts.Tokenize(tok,tokfrom,"\t"))
		w++;

//	for(int i = 0; i < l; i++)
//	{
//		if ((s[i-1] == '	' || s[i-1] == NULL) && (s[i] != '	')) w++;
//		if (s[i] == NULL) break;
//	}

	//Unpack the file into vectors based on its type
	if(w == 5)
	{
		unpack_file(file, position, InternalField, err_InternalField, current, err_current);
	}
	else if (w == 3)
	{
		unpack_file(file, time, current, InternalField);
	}
	else
	{
		cerr << "ERROR: File does not match known types" << endl;
		return -1;
	}

	//Some files have information which is not represented in other files. For these, fill the vectors with a value which indicates they carry no information.
	if(time.size() < 1)
	{
		for(int i = 0; i < position.size(); i++)
		{
			time.push_back(99999);
		}
	}
	else if (position.size() < 1)
	{
		for(int i = 0; i < time.size(); i++)
		{
			position.push_back(99999);
			err_current.push_back(99999);
			err_InternalField.push_back(99999);
		}

	}

	//Create a root file to store the data taken from the files
	std::string file_root = (file_name.erase(file_name.size()-3,3)).append("root");
//	TFile f((file_path+file_root).c_str(),"RECREATE");
	TFile f((file_path_out+file_root).c_str(),"RECREATE");
	TTree *t = new TTree("t","Conversion of .txt DataFile to ROOT tree");
	double InternalField_i, InternalField_err_i;
	double ExternalField_i, ExternalField_err_i;
	double current_i, current_err_i;
	double position_i, time_i;

	TBranch *b_Position = t->Branch("z",&position_i,"Position of Probe/D");
	TBranch *b_Time = t->Branch("time",&time_i,"Time since start of measurement/D");
	TBranch *b_InternalField = t->Branch("Bi",&InternalField_i,"Field measured inside SC/D");
	TBranch *b_InternalField_err = t->Branch("BiErr",&InternalField_err_i,"Error in Field inside SC/D");
	TBranch *b_ExternalField = t->Branch("Bo",&ExternalField_i,"Field calculated outside SC/D");
	TBranch *b_ExternalField_err = t->Branch("BoErr",&ExternalField_err_i,"Error in Field outside SC/D");
	TBranch *b_calibration = t->Branch("Calibration",&calibration_factor,"Calibration Factor A<->mV and mT/D");
	TBranch *b_current = t->Branch("I",&current_i,"Current through shunt/D");
	TBranch *b_current_err = t->Branch("IErr",&current_err_i,"Error in Current through shunt/D");

	//Fill the output file branches with the data from the file
        for(int i = 0; i < InternalField.size(); i++)
        {
		position_i = position[i];
		time_i = time[i];
		InternalField_i = InternalField[i]*calibration_sign;
		InternalField_err_i = err_InternalField[i];
		ExternalField_i = current[i]*calibration_sign*calibration_factor;
		ExternalField_err_i = err_current[i]*calibration_factor;
		current_i = current[i];
		current_err_i = err_current[i];
		t->Fill();
        }

        f.Write();
        f.Close();

	cout << file_root << " successfully created with " << InternalField.size() << " entries." << endl;

	return 0;
}
