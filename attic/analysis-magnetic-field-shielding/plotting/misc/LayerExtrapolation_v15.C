//Version 2.0 -> Updates calibration curve.

int n = 1;
bool onetoone_leg = false;

int LayerExtrapolation_v15()
{

		std::string ten_file_txt = "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt";	//Room temperature calibration 
//		std::string ten_file_txt = "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt";	//Room temperature calibration 
		std::string ten_file1_txt = "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_222417.txt";	//Before baking
//		std::string ten_file1_txt = "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_114940.txt";	//Before baking
		std::string title = "1 Layer YBCO (lN2)";
		TCanvas *c00 = new TCanvas();
		leg = new TLegend(0.2,0.9,0.7,0.75);
			leg->SetBorderSize(1);

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
        TF1 *fit1 = new TF1("fit1","pol1",0,300);
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
	TF1 *fit4 = new TF1("fit4","pol1",0,2000);
        gr4->Fit("fit4","R");
        TF1 *f4 = gr4->GetFunction("fit4");
                f4->SetName("f4");
                f4->SetLineColor(kBlue-3);
                f4->SetRange(0,2000);
                f4->Draw("SAME");
	c2->Update();

        vector<double> ten_voltageTOfield_1;

        for(int i = 0; i < ten_voltage_1.size(); i++)
        {
                      ten_voltageTOfield_1.push_back(f4->Eval(ten_voltage_1[i]));
	}


	c00->cd();
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
                gr12->GetXaxis()->SetRangeUser(0,100);
                gr12->GetYaxis()->SetTitle("Magnetic Field within Shield (mT)");
                gr12->GetYaxis()->SetRangeUser(0,100);
                gr12->SetTitle(title.c_str());
		if(n == 5) n++; //I dont like yellow on graphs
                gr12->SetMarkerColor(n);
        TF1 *onetoone = new TF1("onetoone","pol1",0,1500);
		onetoone->SetParameter(0,0);
		onetoone->SetParameter(1,1);
		onetoone->SetLineColor(kBlack);
		onetoone->SetLineStyle(7);
	onetoone->Draw("SAME");
	c00->Update();
	
	n++;
	if(onetoone_leg == false)
	{
		leg->AddEntry(onetoone,"1:1 Reference Line","l");
		onetoone_leg = true;
	}
	leg->AddEntry(gr12,title.c_str(),"p");
	std::string title_ext = title + " Linearly Extrapolated";


	c1->Close();
	c2->Close();
//	c3->Close();

//	gr2->SetMarkerColor(kRed);
//	gr2->Draw("p SAME");
//	onetoone->Draw("l SAME");

//	gr1->SetMarkerColor(kBlue);
//	gr1->Draw("p SAME");


	//Second layer

	double internalField_2_i = 0;
	bool ding = true;
	bool dong = true;
	vector<double> appliedField_layer2, internalField_layer2;
	for (int i = 0; i < 1020; i++)
	{
		appliedField_layer2.push_back(i/10.0);
		for(int j = 0; j<ten_voltageTOfield_1.size();j++)
		{
			if(ten_voltageTOfield_1[j] > i/10.0)
			{
				if (ding == true) 
				{
					//cout << ten_field_1[j] << endl;
					for(int k = 0; k < ten_voltageTOfield_1.size();k++)
					{
						if(dong = true)
						{
							if(ten_field_1[j] > ten_voltageTOfield_1[k]) internalField_2_i = ten_field_1[k];	
							dong = false;
						}
					}
					dong = true;
					internalField_layer2.push_back(internalField_2_i);
				}
				ding = false;
			}
		}
		ding = true;
	}

	gr13 = new TGraph(appliedField_layer2.size(),&(appliedField_layer2[0]),&(internalField_layer2[0]));
		gr13->Draw("P SAME");
                gr13->GetXaxis()->SetTitle("Applied Field (mT)");
                gr13->GetXaxis()->SetRangeUser(0,125);
                gr13->GetYaxis()->SetTitle("Magnetic Field within Shield (mT)");
                gr13->GetYaxis()->SetRangeUser(0,125);
                gr13->SetTitle(title.c_str());
                if(n == 5) n++; //I dont like yellow on graphs
                gr13->SetMarkerColor(n);
		n++;
	leg->AddEntry(gr13,"2 Layer Extrapolation","p");


//Third layer

	double internalField_3_i = 0;
	ding = true;
	dong = true;
	vector<double> appliedField_layer3, internalField_layer3;
	for (int i = 0; i < 1020; i++)
	{
		appliedField_layer3.push_back(i/10.0);
		for(int j = 0; j<appliedField_layer2.size();j++)
		{
			if(appliedField_layer2[j] > i/10.0)
			{
				if (ding == true) 
				{
					//cout << ten_field_1[j] << endl;
					for(int k = 0; k < appliedField_layer2.size();k++)
					{
						if(dong = true)
						{
							if(internalField_layer2[j] >appliedField_layer2[k]) internalField_3_i = internalField_layer2[k];	
							dong = false;
						}
					}
					dong = true;
					internalField_layer3.push_back(internalField_3_i);
				}
				ding = false;
			}
		}
		ding = true;
	}

	gr14 = new TGraph(appliedField_layer3.size(),&(appliedField_layer3[0]),&(internalField_layer3[0]));
		gr14->Draw("P SAME");
                gr14->GetXaxis()->SetTitle("Applied Field (mT)");
                gr14->GetXaxis()->SetRangeUser(0,125);
                gr14->GetYaxis()->SetTitle("Magnetic Field within Shield (mT)");
                gr14->GetYaxis()->SetRangeUser(0,125);
                gr14->SetTitle(title.c_str());
                if(n == 5) n++; //I dont like yellow on graphs
                gr14->SetMarkerColor(n);
			n++;
	leg->AddEntry(gr14,"3 Layer Extrapolation","p");

	c00->SetGridx();
	c00->SetGridy();

//Comparison with 2 layer data
	TTree *t1 = new TTree();
	t1->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_155007.txt","time:voltage:field");
	t1->SetMarkerColor(n);
	t1->Draw("-1*field:voltage*25.4","","SAME");
	leg->AddEntry(t1,"2 layer measured","p");
	c00->Update();


//	leg->Draw();


	cout << "All done with: " << title << endl;
	return 0;
}
