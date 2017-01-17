#include "../plotting/headers/exist_test.h"
#include "../plotting/headers/vector_math.h"
#include "../plotting/headers/get_calibration.h"

//#include "../fit_nils/analyzeData.C"


//Unpacks a .txt file into 3 c++ vectors by way of a root tree
void unpack_file(std::string file, vector<double> &time, vector<double> &current, vector<double> &InternalField)
{

	TTree *t = new TTree();
	if(exist_test(file))
	{
		t->ReadFile(file.c_str(),"time:current:field");
	}
	else
	{
		return -1;
	}

	//Datapoints need to be drawn before they can be filled.
	t->Draw("time:current:field","","goff");

        for(int i = 0; i < t->GetEntries(); i++)
        {
		time.push_back((t->GetV1())[i]);		//Time
		current.push_back((t->GetV2())[i]);		//Current in mA 
		InternalField.push_back((t->GetV3())[i]);	//Internal Magnetic Field (mT)
	}

}


//Unpacks a .txt file into 5 c++ vectors by way of a root tree
void unpack_file(std::string file, vector<double> &position, vector<double> &field, vector<double> &err_field, vector<double> &current, vector<double> &err_current)
{

	TTree *t = new TTree();
	if(exist_test(file))
	{
		t->ReadFile(file.c_str(),"position:field:err_field:current:err_current");
	}
	else
	{
		return -1;
	}

	//Datapoints need to be drawn before they can be filled.
	t->Draw("position:field:err_field","","goff");

        for(int i = 0; i < t->GetEntries(); i++)
        {
		position.push_back((t->GetV1())[i]);		//position
		field.push_back((t->GetV2())[i]);		//field (mT)
		err_field.push_back((t->GetV3())[i]);	//err in field
	}


	t->Draw("current:err_current","","goff");

        for(int i = 0; i < t->GetEntries(); i++)
        {
		current.push_back((t->GetV1())[i]);		//current
		err_current.push_back((t->GetV2())[i]);		//error in current
	}

}
