int RampCorrellation_single(std::string file_name, TGraphErrors &graph, TGraphErrors &graph2, vector<double> &vec_errors)
{

	TTree *t = new TTree();
	const std::string file_path = "/home/jlab/github/MagCloak_Analysis/calibration/data-calib/DATA_MegaVIEW/";
//	std::string file_name = "DataFile_2016-12-08_06-59-11.csv";
	std::string file = file_path+file_name;

	t->ReadFile(file.c_str());
//	t->Print();

	TCanvas *c = new TCanvas();
	t->Draw("TMath::Abs(B1/B2):time");
	TGraph *gh = (TGraph*)c->GetListOfPrimitives()->FindObject("Graph");
	double ratio_mean = gh->GetMean(2);
	double ratio_std = gh->GetRMS(2);

	t->Draw("Bnom:time");
	TGraph *gh1 = (TGraph*)c->GetListOfPrimitives()->FindObject("Graph");
	double nom_mean = gh1->GetMean(2);
	double nom_std = 0;

//	cout << "At " << nom_mean << " mT, B1/B2 is: " << ratio_mean << " +/- " << ratio_std << endl;
	
	c->Close();

	int n = graph.GetN();
	graph.SetPoint(n,nom_mean,ratio_mean);
	graph.SetPointError(n,nom_std,ratio_std);

//-------------------------------------------------------------------------------

	TCanvas *c2 = new TCanvas();
	t->Draw("B3:time");
	TGraph *gh2 = (TGraph*)c2->GetListOfPrimitives()->FindObject("Graph");
	double B3_mean = -1*(gh2->GetMean(2));
	double B3_std = gh2->GetRMS(2);
	
	int n2 = graph2.GetN();
	graph2.SetPoint(n2, nom_mean, B3_mean);
	graph2.SetPointError(n2, nom_std, B3_std);
	
	c2->Close();

	if(nom_mean < 500) vec_errors.push_back(B3_std);

	return 0;
}

int SteadyStateCorrellation()
{

	TGraphErrors *graph = new TGraphErrors();
	TGraphErrors *graph2 = new TGraphErrors();
	vector<double> vec_errors;

	vector<std::string> file_names = 
		{ //	"DataFile_2016-12-07_23-20-47.csv",
			"DataFile_2016-12-08_00-00-39.csv",
			"DataFile_2016-12-08_00-24-50.csv",
			"DataFile_2016-12-08_00-38-48.csv",
			"DataFile_2016-12-08_01-01-59.csv",
			"DataFile_2016-12-08_01-18-01.csv",
			"DataFile_2016-12-08_01-35-56.csv",
			"DataFile_2016-12-08_01-52-12.csv",
			"DataFile_2016-12-08_02-12-19.csv",
			"DataFile_2016-12-08_02-31-44.csv",
			"DataFile_2016-12-08_02-50-58.csv",
			"DataFile_2016-12-08_03-10-16.csv",
			"DataFile_2016-12-08_03-29-53.csv",
			"DataFile_2016-12-08_03-49-37.csv",
			"DataFile_2016-12-08_04-09-26.csv",
			"DataFile_2016-12-08_04-29-26.csv",
			"DataFile_2016-12-08_04-49-57.csv",
			"DataFile_2016-12-08_05-34-24.csv",
			"DataFile_2016-12-08_05-58-20.csv",
			"DataFile_2016-12-08_06-22-12.csv",
			"DataFile_2016-12-08_06-46-48.csv",
			"DataFile_2016-12-08_07-11-10.csv"
			};

	for(int i = 0; i < file_names.size(); i++)
	{
		RampCorrellation_single(file_names[i], *graph, *graph2, vec_errors);
	}

	TCanvas *c = new TCanvas();
		graph->GetXaxis()->SetTitle("Nominal External Field (mT)");
		graph->GetYaxis()->SetTitle("| B_{1} / B_{2} |");
	graph->Draw();

	double ratio_mean = graph->GetMean(2);
	double ratio_std = graph->GetRMS(2);
	cout << "Average value: " << graph->GetMean(2) << " +/- " << ratio_std << endl;


	TCanvas *c2 = new TCanvas();
		graph2->GetXaxis()->SetTitle("Nominal External Field (mT)");
		graph2->GetYaxis()->SetTitle("B_{3} (mT)");
	graph2->Draw();

	double vec_average = 0;
	double vec_max = 0, vec_min = 999999999999999;
	for(int i =0; i< vec_errors.size(); i++)
	{	
//		cout << vec_errors[i] << endl;
		if(vec_errors[i]>vec_max) vec_max = vec_errors[i];
		if(vec_errors[i]<vec_min) vec_min = vec_errors[i];
		vec_average = vec_average+vec_errors[i];
	}
	vec_average = vec_average/vec_errors.size();
	cout << "Average Error in (Non time dependant) measureemnts: " << vec_average << " mT" << endl;
	cout << "     (Min/Max = " << vec_min << " / " << vec_max << ")" << endl;

	return 0;
}
