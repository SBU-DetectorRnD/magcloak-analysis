#include "exist_test.h"
#include "vector_math.h"

//#include "../fit_nils/TimeDependenceResult.h"
//#include "../fit_nils/analyzeTimeDependence.C"


//Counter for each region. Makes sure that the TGraphs and Histograms do not have the same name and avoid memory errors.
int region_count = 1;

//Unpacks a standard .root file into 3 c++ vectors by way of a root tree
void unpack_file(std::string file, vector<double> &time, vector<double> &AppliedField, vector<double> &InternalField)
{


	TFile f(file.c_str(),"READ");
	TTree *t = (TTree*)f.Get("t");
	//Datapoints need to be drawn before they can be filled.
	t->Draw("Calibration:Bo:Bi","","goff");

        for(int i = 0; i < t->GetEntries(); i++)
        {
		time.push_back((t->GetV1())[i]);		//Time
		AppliedField.push_back((t->GetV2())[i]);	//Applied field already calibrated
		InternalField.push_back((t->GetV3())[i]);	//Internal Magnetic Field (mT)
	}

}


//Multiples by constant factor which is specified at call time. Also returns this factor.
double current_conversion(double calibration, vector<double> &current, vector<double> &AppliedField)
{
	for(int i = 0; i < current.size(); i++)
	{
		AppliedField.push_back(calibration*current[i]);
	}
	return calibration;
}

//Subtracts a constant average value from a vector based on a select number of points at the beginning.
void remove_zero_offset(int start, int finish, vector<double> &InternalField)
{
int j = 0;
cout << j << endl; j++; 
	double offset_average = 0.00;
	for (int i = start; i < finish; i++)
	{
		offset_average = offset_average + InternalField[i];
	}
	offset_average = offset_average / (finish - start);
cout << j << endl; j++; 

	for(int i = 0; i < InternalField.size(); i++)
	{
		InternalField[i] = InternalField[i] - offset_average;
	}
cout << j << endl; j++; 
}

//Takes the 'gradient' of a vector by looking at simple point to point derivitive
void gradient_vector(const vector<double> &current, const vector<double> &time, vector<double> &current_grad)
{
	int size = current.size();
	for(int i = 0; i < size - 1 ; i++)
	{
		current_grad.push_back( (current[i+1] - current[i]) / (time[i+1] - time[i]) );
	}

}

//If there is a time dependance of the field, this function will fit it and use that fit to extrapolate out its long term behaviour. If no time dependance just take the mean and stdev
void extrapolate_field(const vector<double> &InternalField, const vector<double> &time, double start, double end, vector<double> &InternalField_stable, vector<double> &InternalField_err, double extrapolation_time)
{

	//Converts the integer region_count into a string, which is appended to the ends of the names of the graphs in this section to avoid duplicate names within the same program
	std::stringstream ss;
	ss << region_count;
	std::string region_string;
	ss >> region_string;

	//Get a vector which is only the portion of internal field we are looking at
	vector<double> InternalField_partial;
	for (int i = start; i < end; i++)
	{
		InternalField_partial.push_back(InternalField[i]);
	}

	//Get properties of that vector
	double mean_lin = mean_vector(InternalField_partial);
	double stdev_lin = standard_deviation_vector(InternalField_partial);

	TCanvas *c1 = new TCanvas();

	//If the final value in the vector is more than 1 sigma above the mean, we say that the vector has a time dependance
	bool stdev_test = true;
	if(TMath::Abs(InternalField[end]) > (TMath::Abs(InternalField[start]) + standard_deviation_vector(InternalField, start, end))) stdev_test = false;

	//For low fields (before the leakage starts) simply take mean/stdev. For high fields a fit an extrapolation is required.
	if(stdev_test)
	{
		mean_lin = mean_vector(InternalField_partial, extrapolation_time-60,extrapolation_time+60);
		stdev_lin = standard_deviation_vector(InternalField_partial, extrapolation_time-60,extrapolation_time+60);
		InternalField_stable.push_back(mean_lin);
		InternalField_err.push_back(stdev_lin);
		
	}
	else
	{
		mean_lin = mean_vector(InternalField_partial);
		stdev_lin = standard_deviation_vector(InternalField_partial);
		TGraph *gr1 = new TGraph((end-start),&time[start],&InternalField[start]);
		gr1->Draw("ap");
		// average and stdev
		InternalField_stable.push_back(mean_lin);
		InternalField_err.push_back(stdev_lin);

		TLine *mean_line_lin = new TLine(gr1->GetXaxis()->GetXmin(), mean_lin, gr1->GetXaxis()->GetXmax(), mean_lin);
			mean_line_lin->SetLineColor(kBlue-1);
			mean_line_lin->Draw("SAME");
		TLine *stdev_line_lin = new TLine(gr1->GetXaxis()->GetXmin(), mean_lin - stdev_lin, gr1->GetXaxis()->GetXmax(), mean_lin - stdev_lin);
			stdev_line_lin->SetLineColor(kBlue-2);
			stdev_line_lin->SetLineStyle(7);
			stdev_line_lin->Draw("SAME");
		TLine *stdev_line2_lin = new TLine(gr1->GetXaxis()->GetXmin(), mean_lin + stdev_lin, gr1->GetXaxis()->GetXmax(), mean_lin + stdev_lin);
			stdev_line2_lin->SetLineColor(kBlue-2);
			stdev_line2_lin->SetLineStyle(7);
			stdev_line2_lin->Draw("SAME");

		cout << "Average: " << mean_lin << "+/-" << stdev_lin << endl;
	}

	c1->Close();
	region_count++;

}

int Summary_File_Single( std::string data_file_path, std::string summary_file_path, const vector<std::string> &data_file_name_vector, std::string calibration_string, std::string negation, const vector<int> line_stop)
{
	std::string str = data_file_name_vector[0];
	std::string data_file_root = (str.erase(str.size()-4,4)).append("root");
	TFile f((summary_file_path+"SummaryOf_"+data_file_root).c_str(),"RECREATE");
	TTree *t = new TTree("t","Shielding Measurement with Long Time Extrapolation");
	double InternalField_i, InternalField_err_i;
	double ExternalField_i, ExternalField_err_i;
	double current_i, current_err_i;
	double conversion_factor;

	TBranch *b_InternalField = t->Branch("Bi",&InternalField_i,"Field inside SC/D");
	TBranch *b_InternalField_err = t->Branch("BiErr",&InternalField_err_i,"Error in Field inside SC/D");
	TBranch *b_ExternalField = t->Branch("Bo",&ExternalField_i,"Field outside SC/D");
	TBranch *b_ExternalField_err = t->Branch("BoErr",&ExternalField_err_i,"Error in Field outside SC/D");
	TBranch *b_calibration = t->Branch("Calibration",&conversion_factor,"Calibration Factor A<->mT/D");
	TBranch *b_current = t->Branch("I",&current_i,"Current through shunt/D");
	TBranch *b_current_err = t->Branch("IErr",&current_err_i,"Error in Current through shunt/D");
	
	for(int file_int = 0; file_int < data_file_name_vector.size(); file_int++)
	{
		std::string data_file_name = data_file_name_vector[file_int];
		cout << "       Executing DataCut function for file: " << data_file_name << endl;
		std::string data_file = data_file_path+data_file_name;
		vector<double> time, current, InternalField;
		vector<double> AppliedField;

		//If the line to stop at was set to 0, then unpack the whole file into the vector<doubles> above. Else unpack only to the line line_stop
		if(!exists_test(data_file))
		{
			cerr << "ERROR: " << data_file << "  Not found." << endl;
			return -1;
		}
		if(line_stop[file_int] == 0)
		{
			unpack_file(data_file, time, AppliedField, InternalField);
		}
		else
		{
			unpack_file(data_file, time, AppliedField, InternalField, line_stop);
		}	

		//If option is set, negate internal field
		if(negation != "True")
		{
			for(int i = 0; i < InternalField.size(); i++)
			{
				InternalField[i] = InternalField[i]*-1.00;
			}
		}

		conversion_factor = time[0];

		int region_start, region_end;	//Vectors which define the regions. region_start[i] and region_end[i] will define the beginning and end of one region.
		region_start = 0;
		region_end = InternalField.size();

	//	double extrapolation_time = 1.577*(10**7); 		//Seconds past the end of the region to extrapolate to. Half a year.
		double extrapolation_time = 60*10;			//Seconds past the end of the region to extrapolate to.
		vector<double> InternalField_stable, InternalField_err;
		vector<double> ExternalField_stable, ExternalField_err;


		bool stdev_test = false;
		//Extrapolate out the field values for the stable regions. Store them in the appropriate vectors. For the applied field simply take the mean/stdev.
		cout << "              For Region " << region_count << " (starting at t = " << TMath::Nint(time[region_start]) << "): ";

		//If the final value in the vector is more than 1 sigma above the mean, we say that the vector has a time dependance. Check +/- 5 points to make sure and remove risk of errors
		for(int i = 0; i < 5; i++)
		{
			if(TMath::Abs(InternalField[region_end - i]) > (TMath::Abs(InternalField[region_start + i]) + standard_deviation_vector(InternalField, region_start, region_end))) stdev_test = true;
		}


		extrapolate_field(InternalField, time, region_start, region_end, InternalField_stable, InternalField_err, extrapolation_time);

		ExternalField_stable.push_back( mean_vector(AppliedField, region_start, region_end) );
		ExternalField_err.push_back( standard_deviation_vector(AppliedField, region_start, region_end) );

		//Write all of this data to a root tree within a .root summary file	

		for(int i = 0; i < InternalField_stable.size(); i++)
		{
			InternalField_i = InternalField_stable[i];
			InternalField_err_i = InternalField_err[i];
			ExternalField_i = ExternalField_stable[i];
			ExternalField_err_i = ExternalField_err[i];
			current_i = ExternalField_i / conversion_factor;
			current_err_i = ExternalField_err_i / conversion_factor;
			t->Fill();
		}

		f.Write();
	}
		f.Close();

	return 0;
}
