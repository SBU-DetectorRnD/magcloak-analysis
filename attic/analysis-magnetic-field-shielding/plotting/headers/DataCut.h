#include "exist_test.h"
#include "vector_math.h"
#include "get_calibration.h"

#include "../fit_time_dependence/TimeDependenceResult.h"
#include "../fit_time_dependence/analyzeTimeDependence.C"


//Counter for each region. Makes sure that the TGraphs and Histograms do not have the same name and avoid memory errors.
int region_count = 1;

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

//Unpacks a .txt file into 3 c++ vectors by way of a root tree. Adds extra control for stopping the execution before the end of the file.
void unpack_file(std::string file, vector<double> &time, vector<double> &current, vector<double> &InternalField, int stop_line)
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
		if (i == stop_line) break;
		time.push_back((t->GetV1())[i]);		//Time
		current.push_back((t->GetV2())[i]);		//Current in mA 
		InternalField.push_back((t->GetV3())[i]);	//Internal Magnetic Field (mT)
	}

}

//Multiples by a constant factor associated with a measurement setup to convert from A to mT
double current_conversion(std::string setup, vector<double> &current, vector<double> &AppliedField)
{

	double calibration = 0.00;

	if(setup == "Helmholtz")
		calibration = 0.839957;
	if(setup == "Solenoid")
		calibration =  24.5;
	if(setup == "Dipole")
		calibration = 19.6124;
	if(setup == "DipoleNew")
		calibration = 20.3989;
	if(setup == "SmallBlack")
		calibration =  19.5361;
	if(setup.find("DataFile") != std::string::npos)
		calibration = get_calibration(setup);

	for(int i = 0; i < current.size(); i++)
	{
		AppliedField.push_back(calibration*current[i]);
	}

	if(calibration == 0.00) cout << "       ERROR: Calibration not found" << endl;
	return calibration;

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

//Algorithm for finding the stable regions in a data set
void find_stable_regions(const vector<double> &current_grad, vector<double> &region_start, vector<double> &region_end, double threshold, double length)
{
	int i, j;

	for(i = 0; i < current_grad.size(); i++)
	{
		if(TMath::Abs(current_grad[i]) >= threshold)
		{
			for(j = i+1; j < current_grad.size(); j++)
			{
				if(TMath::Abs(current_grad[j]) >= threshold)
				{
					if(j - i - 1 > length)
					{
						region_start.push_back(i);
						region_end.push_back(j);
						i = j + 1;
						continue;
					}
				}
			}

			if(j - i - 1 > length)
			{
				region_start.push_back(i);
				region_end.push_back(j);
				i = j + 1;
				continue;
			}
		}
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
	if((TMath::Abs(InternalField[end]) < 1.0) || stdev_test)	//For small data sets, the fits do not work well. 
	{
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

		c1->Close();
	}
	else if((end-start) < 250)
	{
		cout << "Region not large enough for long term fit (" << end - start << " points). Skipping." << endl;
	}
	else
	{
		/*
		int div = 1; //Used for testing fits on different ranges with large datasets. Leave at 1 for actual execution.
		double fraction_to_fit = 0.0;

		TProfile *gr1 = new TProfile(("gr1"+region_string).c_str(),"Profile of dataset",100, time[start], time[end],"");
		for(int i = start; i < end; i++)
		{
			gr1->Fill(time[i], InternalField[i],i);
		}
		gr1->Draw();

		TF1 *logfit = new TF1("logfit","[0]*TMath::Log(x) + [1]", gr1->GetXaxis()->GetXmin() + (gr1->GetXaxis()->GetXmax()-gr1->GetXaxis()->GetXmin())*fraction_to_fit, gr1->GetXaxis()->GetXmax()/div);
			logfit->SetParameter(0,0.01);
			logfit->SetParameter(1,mean_vector(InternalField_partial));
		gr1->Fit("logfit","B R Q");
		gr1->Fit("logfit","B R Q");
		TF1 *f1 = gr1->GetFunction("logfit");
			f1->SetName("f1");
			f1->SetLineColor(kBlue-3);
			f1->SetRange(0,gr1->GetXaxis()->GetXmax());

		TCanvas *c2 = new TCanvas();
		TProfile *gr2 = new TProfile(("gr2"+region_string).c_str(),"Profile of dataset Region",100, time[start], time[end],"");
		for(int i = start; i < end; i++)
		{
			gr2->Fill(time[i], InternalField[i],1);
		}
		gr2->Draw();
		TF1 *powerfit = new TF1("powerfit","[0]*TMath::Power(([4]*x+[1]),[2]) + [3]", (gr1->GetXaxis()->GetXmin() + gr1->GetXaxis()->GetXmax())/2.0, gr1->GetXaxis()->GetXmax()/div);
			powerfit->SetParameter(0,0);
			powerfit->SetParameter(1,0.01);
			powerfit->SetParLimits(1,0,1);
			powerfit->SetParameter(2,0.25);
			powerfit->SetParLimits(2,0,1);
			powerfit->SetParameter(3,-1*mean_vector(InternalField_partial));
			powerfit->SetParameter(4,1);
		gr2->Fit("powerfit","B R Q");
		gr2->Fit("powerfit","B R Q");
		TF1 *f2 = gr2->GetFunction("powerfit");
			f2->SetName("f2");
			f2->SetLineColor(kGreen-3);
			f2->SetRange(0,gr1->GetXaxis()->GetXmax());

		TCanvas *c3 = new TCanvas();
		TProfile *gr3 = new TProfile(("gr3"+region_string).c_str(),"Profile of dataset Region",100, time[start], time[end],"");
		for(int i = start; i < end; i++)
		{
			gr3->Fill(time[i], InternalField[i],1);
		}
		gr3->Draw();
		f1->Draw("SAME");
		f2->Draw("SAME");
		TLine *fit_line = new TLine(gr1->GetXaxis()->GetXmin() + (gr1->GetXaxis()->GetXmax()-gr1->GetXaxis()->GetXmin())*fraction_to_fit, gr1->GetYaxis()->GetXmin(), gr1->GetXaxis()->GetXmin() + (gr1->GetXaxis()->GetXmax()-gr1->GetXaxis()->GetXmin())*fraction_to_fit, gr1->GetYaxis()->GetXmax());
			fit_line->SetLineColor(kBlue-1);
			fit_line->Draw("SAME");

		TCanvas *c4 = new TCanvas();
		TH1F *blank2 = new TH1F(("blank2"+region_string).c_str(),"Time Dependance of Field",10,gr1->GetXaxis()->GetXmin()/100,gr1->GetXaxis()->GetXmax()*10);
			blank2->GetYaxis()->SetRangeUser(-1,gr1->GetYaxis()->GetXmax()*10);
			blank2->GetXaxis()->SetTitle("Time (Seconds)");
			//blank->GetYaxis()->SetTitle("B_{0} - B_{i} (mT)");    //Shielding Factor y-axis
			blank2->GetYaxis()->SetTitle("B_{i} (mT)");              //Internal vs. External Field y-axis
			blank2->GetXaxis()->SetNdivisions(505);
			blank2->GetYaxis()->SetNdivisions(505);
			blank2->SetLineColor(0);
		blank2->Draw();
		gr3->Draw("p SAME");
		f1->Draw("SAME");
		f1->SetRange(gr1->GetXaxis()->GetXmin()/1000,gr1->GetXaxis()->GetXmax()*1000);
		f2->Draw("SAME");
		f2->SetRange(gr1->GetXaxis()->GetXmin()/1000,gr1->GetXaxis()->GetXmax()*1000);

		double extrap_average = (TMath::Abs(f1->Eval(gr1->GetXaxis()->GetXmax() + extrapolation_time)) + TMath::Abs(f2->Eval(gr1->GetXaxis()->GetXmax() + extrapolation_time)))/2.0;
		double extrap_err = TMath::Abs(TMath::Abs(f1->Eval(gr1->GetXaxis()->GetXmax() + extrapolation_time)) - TMath::Abs(f2->Eval(gr1->GetXaxis()->GetXmax() + extrapolation_time)));

		cout << "Extrapolation: " << extrap_average << " +/- " << extrap_err << " after " << extrapolation_time << " seconds / " << extrapolation_time/(60*60) << " hours / " << extrapolation_time/(60*60*24) << " days" << endl;


		InternalField_stable.push_back(extrap_average);
		InternalField_err.push_back(extrap_err);

//		c1->Print(("./plots/c1"+region_string+".png").c_str());
		c1->Close();
//		c2->Print(("./plots/c2"+region_string+".png").c_str());
		c2->Close();
//		c3->Print(("./plots/c3"+region_string+".png").c_str());
		c3->Close();
//		c4->Print(("./plots/c4"+region_string+".png").c_str());
		c4->Close();
	*/
	}
	region_count++;

}

int DataCut( std::string data_file_path, std::string summary_file_path, std::string data_file_name, std::string calibration_string, std::string negation, int line_stop)
{

	cout << "       Executing DataCut function for file: " << data_file_name << endl;
	std::string data_file = data_file_path+data_file_name;
	vector<double> time, current, InternalField;

	//If the line to stop at was set to 0, then unpack the whole file into the vector<doubles> above. Else unpack only to the line line_stop
	if(!exists_test(data_file))
	{
		cerr << "ERROR: " << data_file << "  Not found." << endl;
		return -1;
	}
	if(line_stop == 0)
	{
		unpack_file(data_file, time, current, InternalField);
	}
	else
	{
		unpack_file(data_file, time, current, InternalField, line_stop);
	}	

	//Convert the current into applied field using the known calibration factor
	vector<double> AppliedField;
	double conversion_factor = current_conversion(calibration_string, current, AppliedField);

	//Remove any offset from the ambient field if the probe was zeroed in the mu metal
	remove_zero_offset(0, 10, InternalField);

	//If option is set, negate internal field
	if(negation != "True")
	{
		for(int i = 0; i < InternalField.size(); i++)
		{
			InternalField[i] = InternalField[i]*-1.00;
		}
	}

	//Take the piecewise gradient of the current to split the datafile into the stable regions.
	vector<double> current_grad;
	gradient_vector(current, time, current_grad);

	vector<double> region_start, region_end;	//Vectors which define the regions. region_start[i] and region_end[i] will define the beginning and end of one region.
	double threshold = 0.005;			//Current threshold for datacut. Anything above this indicates the changing of the current and the beginning of a new region
	double length = 200;				//Minimum length of the region to save. Should be large enough to smooth out the curves
	find_stable_regions(current_grad, region_start, region_end, threshold, length);
	if(region_start.size() == 0)
	{
		cerr << "       ERROR: No stable regions. Please adjust derivitive threshold or redo measurement." << endl;
		return -1;
	}
	else
	{
		cout << "       FOUND: " << region_start.size() << " stable region(s)." << endl;
	}


//	double extrapolation_time = 1.577*(10**7); 		//Seconds past the end of the region to extrapolate to. Half a year.
	double extrapolation_time = 100000;			//Seconds past the end of the region to extrapolate to.
	vector<double> InternalField_stable, InternalField_err;
	vector<double> ExternalField_stable, ExternalField_err;

	//Extrapolate out the field values for the stable regions. Store them in the appropriate vectors. For the applied field simply take the mean/stdev.
	for (int i = 0; i < region_start.size(); i++)
	{
		cout << "              For Region " << i+1 << " (starting at t = " << TMath::Nint(time[region_start[i]]) << "): ";
		extrapolate_field(InternalField, time, region_start[i], region_end[i], InternalField_stable, InternalField_err, extrapolation_time);
		ExternalField_stable.push_back( mean_vector(AppliedField, region_start[i], region_end[i]) );
		ExternalField_err.push_back( standard_deviation_vector(AppliedField, region_start[i], region_end[i]) );
	}

	//Write all of this data to a root tree within a .root summary file	
	std::string data_file_root = (data_file_name.erase(data_file_name.size()-3,3)).append("root");
	TFile f((summary_file_path+"SummaryOf_"+data_file_root).c_str(),"RECREATE");
	TTree *t = new TTree("t","Shielding Measurement with Long Time Extrapolation");
	double InternalField_i, InternalField_err_i;
	double ExternalField_i, ExternalField_err_i;
	double current_i, current_err_i;

	TBranch *b_InternalField = t->Branch("Bi",&InternalField_i,"Field inside SC/D");
	TBranch *b_InternalField_err = t->Branch("BiErr",&InternalField_err_i,"Error in Field inside SC/D");
	TBranch *b_ExternalField = t->Branch("Bo",&ExternalField_i,"Field outside SC/D");
	TBranch *b_ExternalField_err = t->Branch("BoErr",&ExternalField_err_i,"Error in Field outside SC/D");
	TBranch *b_calibration = t->Branch("Calibration",&conversion_factor,"Calibration Factor A<->mT/D");
	TBranch *b_current = t->Branch("I",&current_i,"Current through shunt/D");
	TBranch *b_current_err = t->Branch("IErr",&current_err_i,"Error in Current through shunt/D");

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
	f.Close();

	return 0;
}



int DataCut_BetterFit( std::string data_file_path, std::string summary_file_path, const vector<std::string> &data_file_name_vector, std::string calibration_string, std::string negation, const vector<int> line_stop)
{
	std::string str = data_file_name_vector[0];
	std::string data_file_root = (str.erase(str.size()-3,3)).append("root");
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

		//If the line to stop at was set to 0, then unpack the whole file into the vector<doubles> above. Else unpack only to the line line_stop
		if(!exists_test(data_file))
		{
			cerr << "ERROR: " << data_file << "  Not found." << endl;
			return -1;
		}
		if(line_stop[file_int] == 0)
		{
			unpack_file(data_file, time, current, InternalField);
		}
		else
		{
			unpack_file(data_file, time, current, InternalField, line_stop);
		}	

		//Convert the current into applied field using the known calibration factor
		vector<double> AppliedField;
		double conversion_factor = current_conversion(calibration_string, current, AppliedField);

		//If option is set, negate internal field
		if(negation != "True")
		{
			for(int i = 0; i < InternalField.size(); i++)
			{
				InternalField[i] = InternalField[i]*-1.00;
			}
		}

		int region_start, region_end;	//Vectors which define the regions. region_start[i] and region_end[i] will define the beginning and end of one region.
		region_start = 0;
		region_end = InternalField.size();

	//	double extrapolation_time = 1.577*(10**7); 		//Seconds past the end of the region to extrapolate to. Half a year.
		double extrapolation_time = 100000;			//Seconds past the end of the region to extrapolate to.
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


		if(stdev_test)
		{
			cout << "Hello!" << endl;
			//Run Nils' fitting code
			TimeDependenceResult *fitresult = analyzeTimeDependence(data_file, region_start, region_end, false);
				InternalField_stable.push_back(fitresult.extrap_field);
				InternalField_err.push_back((fitresult.extrap_field_err_pos + fitresult.extrap_field_err_neg)/2.0);
		}
		else
		{
			extrapolate_field(InternalField, time, region_start, region_end, InternalField_stable, InternalField_err, extrapolation_time);
		}

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




int DataCut_SepFiles( std::string data_file_path, std::string summary_file_path, const vector<std::string> &data_file_name_vector, std::string calibration_string, std::string negation, const vector<int> line_stop)
{
	std::string str = data_file_name_vector[0];
	std::string data_file_root = (str.erase(str.size()-3,3)).append("root");
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
		std::string data_file = data_file_path+data_file_name_vector[file_int];
		vector<double> time, current, InternalField;

		//If the line to stop at was set to 0, then unpack the whole file into the vector<doubles> above. Else unpack only to the line line_stop
		if(!exists_test(data_file))
		{
			cerr << "ERROR: " << data_file << "  Not found." << endl;
			return -1;
		}
		if(line_stop[file_int] == 0)
		{
			unpack_file(data_file, time, current, InternalField);
		}
		else
		{
			unpack_file(data_file, time, current, InternalField, line_stop);
		}	

		//Convert the current into applied field using the known calibration factor
		vector<double> AppliedField;
		double conversion_factor = current_conversion(calibration_string, current, AppliedField);

		//If option is set, negate internal field
		if(negation != "True")
		{
			for(int i = 0; i < InternalField.size(); i++)
			{
				InternalField[i] = InternalField[i]*-1.00;
			}
		}

		vector<double> region_start, region_end;	//Vectors which define the regions. region_start[i] and region_end[i] will define the beginning and end of one region.
		region_start.push_back(0);
		region_end.push_back(InternalField.size());

	//	double extrapolation_time = 1.577*(10**7); 		//Seconds past the end of the region to extrapolate to. Half a year.
		double extrapolation_time = 100000;			//Seconds past the end of the region to extrapolate to.
		vector<double> InternalField_stable, InternalField_err;
		vector<double> ExternalField_stable, ExternalField_err;

		//Extrapolate out the field values for the stable regions. Store them in the appropriate vectors. For the applied field simply take the mean/stdev.
		for (int i = 0; i < region_start.size(); i++)
		{
			cout << "              For Region " << region_count << " (starting at t = " << TMath::Nint(time[region_start[i]]) << "): ";
			extrapolate_field(InternalField, time, region_start[i], region_end[i], InternalField_stable, InternalField_err, extrapolation_time);
			ExternalField_stable.push_back( mean_vector(AppliedField, region_start[i], region_end[i]) );
			ExternalField_err.push_back( standard_deviation_vector(AppliedField, region_start[i], region_end[i]) );
		}

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
