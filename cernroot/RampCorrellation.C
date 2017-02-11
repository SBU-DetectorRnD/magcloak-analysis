int RampCorrellation_single(std::string file_name, TGraphErrors &graph)
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
	cout << gh->GetMean(2) << " +/- " << ratio_std << endl;

	t->Draw("Bnom:time");
	TGraph *gh1 = (TGraph*)c->GetListOfPrimitives()->FindObject("Graph");
	double nom_mean = gh1->GetMean(2);
	double nom_std = 0;
	
	c->Close();

	int n = graph.GetN();
	graph.SetPoint(n,nom_mean,ratio_mean);
	graph.SetPointError(n,nom_std,ratio_std);

	return 0;
}

int RampCorrellation()
{

	TGraphErrors *graph = new TGraphErrors();
	vector<std::string> file_names = 
		{"DataFile_2016-12-07_23-43-34.csv",
		"DataFile_2016-12-08_00-19-16.csv",
		"DataFile_2016-12-08_00-28-19.csv",
		"DataFile_2016-12-08_00-51-10.csv",
		"DataFile_2016-12-08_01-10-28.csv",
		"DataFile_2016-12-08_01-23-17.csv",
		"DataFile_2016-12-08_01-44-18.csv",
		"DataFile_2016-12-08_02-05-18.csv",
		"DataFile_2016-12-08_02-24-38.csv",
		"DataFile_2016-12-08_02-44-02.csv",
		"DataFile_2016-12-08_03-03-13.csv",
		"DataFile_2016-12-08_03-22-56.csv",
		"DataFile_2016-12-08_03-42-12.csv",
		"DataFile_2016-12-08_04-02-11.csv",
		"DataFile_2016-12-08_04-22-19.csv",
		"DataFile_2016-12-08_04-42-59.csv",
		"DataFile_2016-12-08_05-03-32.csv",
		"DataFile_2016-12-08_05-24-36.csv",
		"DataFile_2016-12-08_05-46-57.csv",
		"DataFile_2016-12-08_06-10-42.csv",
		"DataFile_2016-12-08_06-34-32.csv",
		"DataFile_2016-12-08_06-59-11.csv"};

	for(int i = 0; i < file_names.size(); i++)
	{
		RampCorrellation_single(file_names[i], *graph);
	}

	TCanvas *c = new TCanvas();
		graph->GetXaxis()->SetTitle("Nominal External Field (mT)");
		graph->GetYaxis()->SetTitle("| B_{1} / B_{2} |");
	graph->Draw();

	return 0;
}
