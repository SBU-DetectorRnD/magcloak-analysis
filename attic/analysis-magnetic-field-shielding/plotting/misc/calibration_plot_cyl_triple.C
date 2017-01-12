int n=2;
bool onetoone_leg = false;

void calibration_plot_single(std::string ten_file_txt, std::string ten_file1_txt, std::string title, TCanvas &c00, TLegend &leg)
{
//Measurement Files


	std::ifstream ten_file(ten_file_txt.c_str()); //first run no cooling
	std::ifstream ten_file1(ten_file1_txt.c_str()); //second run with cooling

        std::string str;
        double ti, xi, yi;
        vector<double> ten_time, ten_field, ten_voltage;
        vector<double> ten_time_1, ten_field_1, ten_voltage_1;

        while (std::getline(ten_file, str))
        {
                if(ten_file >> ti >> xi >> yi)
                {
                        ten_time.push_back(ti); //Time
                        ten_voltage.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
                        ten_field.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
                }
        }

        while (std::getline(ten_file1, str))
        {
                if(ten_file1 >> ti >> xi >> yi)
                {
                        ten_time_1.push_back(ti); //Time
                        ten_voltage_1.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
                        ten_field_1.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
                }
        }

	TCanvas *c1 = new TCanvas();
        gr1 = new TGraph(ten_time.size(),&(ten_voltage[0]),&(ten_field[0]));
                gr1->Draw("AP");
                gr1->GetXaxis()->SetTitle("Voltage (mV)");
                gr1->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr1->SetTitle("Voltage from Power Supply vs. Magnetic Field");
                gr1->SetMarkerColor(kRed);
        TF1 *fit1 = new TF1("fit1","pol1",13,300);
        gr1->Fit("fit1","R");
        TF1 *f1 = gr1->GetFunction("fit1");
                f1->SetName("f1");
                f1->SetLineColor(kRed);
                f1->SetRange(0,300);
                f1->Draw("SAME");
        c1->Update();

        gr2 = new TGraph(ten_time_1.size(),&(ten_voltage_1[0]),&(ten_field_1[0]));
                gr2->Draw("p SAME");
                gr2->GetXaxis()->SetTitle("Voltage (mV)");
                gr2->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr2->SetTitle("Voltage from Power Supply vs. Magnetic Field");
                gr2->SetMarkerColor(kBlue);
/*        TF1 *fit2 = new TF1("fit2","pol1",60,300);
        gr2->Fit("fit2","R");
        TF1 *f2 = gr2->GetFunction("fit2");
                f2->SetName("f2");
                f2->SetLineColor(kBlue-3);
                f2->SetRange(0,300);
                f2->Draw("SAME");
*/        c1->Update();

        leg3 = new TLegend(0.2,0.8,0.48,0.9);
                leg3->AddEntry(gr1,"Room Temperature","p");
                leg3->AddEntry(gr2,"Cryo","p");
        leg3->Draw();

	TCanvas *c2 = new TCanvas();
	gr4 = new TGraph(ten_time.size(),&(ten_voltage[0]),&(ten_field[0]));
                gr4->Draw("AP");
                gr4->SetMarkerColor(kRed);
	TF1 *fit4 = new TF1("fit4","pol1",11,2000);
        gr4->Fit("fit4","R");
        TF1 *f4 = gr4->GetFunction("fit4");
                f4->SetName("f4");
                f4->SetLineColor(kBlue-3);
                f4->SetRange(0,2000);
                f4->Draw("SAME");
	c2->Update();

        TCanvas *c3 = new TCanvas();
        gr55 = new TGraph(ten_time.size(),&(ten_voltage[0]),&(ten_field[0]));
                gr55->Draw("AP");
                gr55->SetMarkerColor(kGreen-3);
        TF1 *fit5 = new TF1("fit5","pol2",9,11);
        gr55->Fit("fit5","R");
        TF1 *f5 = gr55->GetFunction("fit5");
                f5->SetName("f5");
                f5->SetLineColor(kRed);
                f5->SetRange(9,11);
                f5->Draw("SAME");

        vector<double> ten_voltageTOfield_1;

        for(int i = 0; i < ten_voltage_1.size(); i++)
        {
//                      ten_voltageTOfield_1.push_back(f4->Eval(ten_voltage_1[i]));

                if(ten_voltage_1[i] < 2)
                {
                        ten_voltageTOfield_1.push_back(0.00);
                }
                if(ten_voltage_1[i] < 11)
                {
                        ten_voltageTOfield_1.push_back(f5->Eval(ten_voltage_1[i]));
                }
                else
                {
                        ten_voltageTOfield_1.push_back(f4->Eval(ten_voltage_1[i]));
                }
	}


/*	
	vector<double> ten_voltageTOfield_1;

	for(int i = 0; i < ten_voltage_1.size(); i++)
	{
			ten_voltageTOfield_1.push_back(f4->Eval(ten_voltage_1[i]));
	}
*/

	c00.cd();
        gr12 = new TGraph(ten_time_1.size(),&(ten_voltageTOfield_1[0]),&(ten_field_1[0]));
                if (n<2) 
		{
			gr12->Draw("AP");
		}
		else
		{
			gr12->Draw("P SAME");
		}
                gr12->GetXaxis()->SetTitle("Applied Field (mT)");
                gr12->GetXaxis()->SetRangeUser(0,125);
                gr12->GetYaxis()->SetTitle("Magnetic Field within Shield (mT)");
                gr12->GetYaxis()->SetRangeUser(0,125);
                gr12->SetTitle(title.c_str());
		if(n == 5) n++; //I dont like yellow on graphs
                gr12->SetMarkerColor(n);
        TF1 *onetoone = new TF1("onetoone","pol1",0,150);
		onetoone->SetParameter(0,0);
		onetoone->SetParameter(1,1);
		onetoone->SetLineColor(kBlack);
		onetoone->SetLineStyle(7);
	onetoone->Draw("SAME");
        leg4 = new TLegend(0.2,0.8,0.48,0.9);
		leg4->AddEntry(gr12,title.c_str(),"p");
                leg4->AddEntry(onetoone,"1:1 Reference Line","l");
        leg4->Draw();
	c00.SetFixedAspectRatio();
	c00.Update();
	
	n++;
	if(onetoone_leg == false)
	{
		leg.AddEntry(onetoone,"1:1 Reference Line","l");
		onetoone_leg = true;
	}
	leg.AddEntry(gr12,title.c_str(),"p");


	c1->Close();
	c2->Close();
	c3->Close();

//	gr2->SetMarkerColor(kRed);
//	gr2->Draw("p SAME");
//	onetoone->Draw("l SAME");

//	gr1->SetMarkerColor(kBlue);
//	gr1->Draw("p SAME");

	cout << "All done with: " << title << endl;
}

int calibration_plot_cyl_triple()
{

        gStyle->SetOptStat(0);
	std::string title = "Applied Field vs. Internal FIeld of Small SC Tubes";

	double ymin, ymax, xmax, xmin;
	ymin = 0;
	ymax = 100;
	xmin = 0;
	xmax = 100;

	TCanvas *c00 = new TCanvas();
	c00->cd();
        TH1F *blank = new TH1F("blank",title.c_str(),10,xmin,xmax);
                blank->GetYaxis()->SetRangeUser(ymin,ymax);
                blank->GetXaxis()->SetTitle("Applied Field (mT)");
                blank->GetYaxis()->SetTitle("Internal Field (mT)");
                blank->GetXaxis()->SetNdivisions(505);
                blank->GetYaxis()->SetNdivisions(505);
                blank->SetLineColor(0);
        blank->Draw();

	c00->SetGridx(1);
	c00->SetGridy(1);

        leg = new TLegend(0.2,0.9,0.7,0.75);
                leg->SetBorderSize(1);
/*
        TTree *t = new TTree();
        t->ReadFile("./DataFiles/1layer_wide_sc_fit_results.txt","Bo:w:Bi:r:t:y:u:i:o:p");
        t->Draw("Bi:Bo","","pl SAME");
	leg->AddEntry(t,"Rapheal's Measurement","pl");
*/

	calibration_plot_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160629_120947.txt",	//Room temperature calibration 
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160629_123602.txt",	//Before baking
		"Single Piece No Overlap (6/28)",
		*c00,
		*leg);	//Title of plot


	calibration_plot_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160629_132435.txt",	//Room temperature calibration 
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160629_134030.txt",	//Before baking
		"22 mm Half Cylinders with Overlap",
		*c00,
		*leg);	//Title of plot
/*
	calibration_plot_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160628_183924.txt",	//Room temperature calibration 
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160628_190527.txt",	//Before baking
		"1 layer AMSC SC tape wrapped in same direction as tape, 6/20/16",
		*c00,
		*leg);	//Title of plot
*/

	calibration_plot_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160628_201006.txt",	//Room temperature calibration 
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160628_203112.txt",	//Before baking
		"Single Piece With Overlap (6/28)",
		*c00,
		*leg);	//Title of plot




        leg->Draw();

	return 0;
}
