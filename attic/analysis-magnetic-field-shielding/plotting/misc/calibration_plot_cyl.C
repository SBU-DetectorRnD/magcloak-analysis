void calibration_plot_single(std::string ten_file_txt, std::string ten_file1_txt, std::string ten_file2_txt, std::string title)
{
//Measurement Files


	std::ifstream ten_file(ten_file_txt.c_str()); //first run no cooling
	std::ifstream ten_file1(ten_file1_txt.c_str()); //second run with cooling
//	std::ifstream ten_file2(ten_file2_txt.c_str()); //second run with cooling

        std::string str;
        double ti, xi, yi;
        vector<double> ten_time, ten_field, ten_voltage;
        vector<double> ten_time_1, ten_field_1, ten_voltage_1;
//        vector<double> ten_time_2, ten_field_2, ten_voltage_2;

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
/*
        while (std::getline(ten_file2, str))
        {
                if(ten_file2 >> ti >> xi >> yi)
                {
                        ten_time_2.push_back(ti); //Time
                        ten_voltage_2.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
                        ten_field_2.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
                }
        }
*/
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
                gr2->SetTitle("Voltage from Power Supply vs. Magnetic Field --- 10 minutes");
                gr2->SetMarkerColor(kBlue);
        TF1 *fit2 = new TF1("fit2","pol1",60,300);
        gr2->Fit("fit2","R");
        TF1 *f2 = gr2->GetFunction("fit2");
                f2->SetName("f2");
                f2->SetLineColor(kBlue-3);
                f2->SetRange(0,300);
                f2->Draw("SAME");
        c1->Update();
/*
        gr3 = new TGraph(ten_time_2.size(),&(ten_voltage_2[0]),&(ten_field_2[0]));
                gr3->Draw("p SAME");
                gr3->GetXaxis()->SetTitle("Voltage (mV)");
                gr3->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr3->SetTitle("Voltage from Power Supply vs. Magnetic Field");
                gr3->SetMarkerColor(kGreen);
        TF1 *fit3 = new TF1("fit3","pol1",60,300);
        gr3->Fit("fit3","R");
        TF1 *f3 = gr3->GetFunction("fit3");
                f3->SetName("f2");
                f3->SetLineColor(kGreen-3);
                f3->SetRange(0,300);
                f3->Draw("SAME");
        c1->Update();
*/
        leg3 = new TLegend(0.2,0.8,0.48,0.9);
                leg3->AddEntry(gr1,"Room Temperature","p");
                leg3->AddEntry(gr2,"Cryo","p");
//                leg3->AddEntry(gr3,"Cryo After Baking","p");
        leg3->Draw();

	TCanvas *c2 = new TCanvas();
	gr4 = new TGraph(ten_time.size(),&(ten_voltage[0]),&(ten_field[0]));
                gr4->Draw("AP");
                gr4->SetMarkerColor(kRed);
	gr5 = new TGraph(ten_time.size(),&(ten_voltage[0]),&(ten_field[0]));
                gr5->Draw("P");
                gr5->SetMarkerColor(kRed);
	TF1 *fit4 = new TF1("fit4","pol1",11,300);
        gr5->Fit("fit4","R");
        TF1 *f4 = gr5->GetFunction("fit4");
                f4->SetName("f4");
                f4->SetLineColor(kBlue-3);
                f4->SetRange(11,300);
                f4->Draw("SAME");
        TF1 *fit5 = new TF1("fit5","pol2",9,11);
        gr4->Fit("fit5","R");
        TF1 *f5 = gr4->GetFunction("fit5");
                f5->SetName("f5");
                f5->SetLineColor(kGreen-3);
                f5->SetRange(9,11);
                f5->Draw("SAME");
	c2->Update();
	
	vector<double> ten_voltageTOfield_1;

	for(int i = 0; i < ten_voltage_1.size(); i++)
	{
		if(ten_voltage_1[i] < 2)
		{
			ten_voltageTOfield_1.push_back(0.00);
		}
		else if(ten_voltage_1[i] < 11)
		{
			ten_voltageTOfield_1.push_back(f5->Eval(ten_voltage_1[i]));
		}
		else
		{
			ten_voltageTOfield_1.push_back(f4->Eval(ten_voltage_1[i]));
		}
	}
/*
	vector<double> ten_voltageTOfield_2;

	for(int i = 0; i < ten_voltage_2.size(); i++)
	{
		if(ten_voltage_2[i] < 2)
		{
			ten_voltageTOfield_2.push_back(0.00);
		}
		else if(ten_voltage_2[i] < 11)
		{
			ten_voltageTOfield_2.push_back(f5->Eval(ten_voltage_2[i]));
		}
		else
		{
			ten_voltageTOfield_2.push_back(f4->Eval(ten_voltage_2[i]));
		}
	}
*/
	vector<double>  shielded_overlap, shielded_non;
	for (int i = 0; i < ten_voltage_1.size(); i++)
	{
		shielded_overlap.push_back(ten_voltageTOfield_1[i] - ten_field_1[i]);
	}
/*	for (int i = 0; i < ten_voltage_2.size(); i++)
	{
		shielded_non.push_back(ten_voltageTOfield_2[i] - ten_field_2[i]);
	}
*/	
	TCanvas *c3 = new TCanvas();
        gr12 = new TGraph(ten_time_1.size(),&(ten_voltageTOfield_1[0]),&(ten_field_1[0]));
        //gr12 = new TGraph(ten_time_1.size(),&(ten_voltageTOfield_1[0]),&(shielded_overlap[0]));
        //gr12 = new TGraph(ten_time_1.size(),&(shielded_overlap[0]),&(ten_field_1[0]));
                gr12->Draw("AP");
                gr12->GetXaxis()->SetTitle("Applied Field (mT)");
                gr12->GetXaxis()->SetRangeUser(0,125);
                //gr12->GetYaxis()->SetTitle("Magnetic Field within Shield (mT)");
                gr12->GetYaxis()->SetTitle("B_{o} - B_{i} (mT)");
                gr12->GetYaxis()->SetRangeUser(0,125);
                gr12->SetTitle(title.c_str());
                gr12->SetMarkerColor(kBlue);
/*        gr6 = new TGraph(ten_time_2.size(),&(ten_voltageTOfield_2[0]),&(ten_field_2[0]));
        //gr6 = new TGraph(ten_time_2.size(),&(ten_voltageTOfield_2[0]),&(shielded_non[0]));
        //gr6 = new TGraph(ten_time_2.size(),&(shielded_non[0]),&(ten_field_2[0]));
		gr6->SetMarkerColor(kRed);
		gr6->Draw("p SAME");
*/        TF1 *onetoone = new TF1("onetoone","pol1",0,150);
		onetoone->SetParameter(0,0);
		onetoone->SetParameter(1,1);
		onetoone->SetLineColor(kBlack);
		onetoone->SetLineStyle(7);
	onetoone->Draw("SAME");

        TTree *t = new TTree();
        t->ReadFile("./DataFiles/1layer_wide_sc_fit_results.txt","Bo:w:Bi:r:t:y:u:i:o:p");
        TCanvas *ctemp = new TCanvas();
t->Draw("Bi:Bo:r","","pl");
c2->cd();	
	TGraphErrors *r = new TGraphErrors(t->GetEntries(),t->GetV2(),t->GetV1());
for ( int i = 0; i < t->GetEntries(); i++ )
r->SetPointError(i,0, t->GetV3()[i]);
r->Draw("Psame");

        leg4 = new TLegend(0.2,0.8,0.48,0.9);
//		leg4->AddEntry(gr2,"Shielding Before Baking","p");
//		leg4->AddEntry(gr6,"Shielding After Baking","p");
		leg4->AddEntry(gr12,"Overlapping Shielding","p");
//		leg4->AddEntry(gr6,"Non-Overlapping Shielding","p");
                leg4->AddEntry(onetoone,"1:1 Reference Line","l");
		leg4->AddEntry(t,"Rapheal's Measurement","pl");
	leg4->SetBorderSize(1);
        leg4->Draw();


//	gr2->SetMarkerColor(kRed);
//	gr2->Draw("p SAME");
//	onetoone->Draw("l SAME");

//	gr1->SetMarkerColor(kBlue);
//	gr1->Draw("p SAME");

	cout << "All done with: " << title << endl;
}

int calibration_plot_cyl()
{

	//Overlapping sample	
	calibration_plot_single(
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160610_151411.txt",	//Room temperature calibration 
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160610_153114.txt",	//Before baking
		"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160610_160144.txt", 	//After baking
		" ");	//Title of plot

	return 0;
}
