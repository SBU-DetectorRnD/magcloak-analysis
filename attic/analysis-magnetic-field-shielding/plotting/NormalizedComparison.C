int NormalizedComparison()
{

	//B_int (2-layer overlap) / B_int (1-layer overlap) vs B_ext + B_int (2-layer SOLDERED) / B_int (1-layer overlap) vs B_ext

	std::string two_layer_overlap = "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_155007.txt";
	std::string one_layer_overlap = "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_222417.txt";
	std::string two_layer_solder = "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_183353.txt";

	double solenoid_calibration = 24.4; //24.4 mT/A

	//Read in one layer overlap

        TTree *t1 = new TTree();
        t1->ReadFile(one_layer_overlap.c_str(),"seconds:voltage:field");

        //The vectors have to be drawn before they can be read in. Temporary canvas created for this purpose and then closed immedietly after.
        TCanvas *ctemp = new TCanvas();
        t1->Draw("seconds:voltage:field");
	ctemp->Close();

        //Number of seconds created from size of TTree (1 measurement per second fixed rate)
        vector<double> seconds_1layer;

        for(int i = 0; i < t1->GetEntries(); i++)
        {
                seconds_1layer.push_back(t1->GetV1()[i]);
        }
        vector<double> fields_1layer;
        for ( int i = 0; i < t1->GetEntries(); i++ )
        {
                fields_1layer.push_back( -1*t1->GetV3()[i] );  //Hall probe was oriented in the opposite direction to the fiels, this simply flips the field from negative to positive.
        }
        vector<double> applied_fields_1layer;
        for ( int i = 0; i < t1->GetEntries(); i++ )
        {
                applied_fields_1layer.push_back( t1->GetV2()[i]*solenoid_calibration );
        }



	//Read in 2 layer overlap

        TTree *t2 = new TTree();
        t2->ReadFile(two_layer_overlap.c_str(),"seconds:voltage:field");

        //The vectors have to be drawn before they can be read in. Temporary canvas created for this purpose and then closed immedietly after.
        TCanvas *ctemp2 = new TCanvas();
        t2->Draw("seconds:voltage:field");
	ctemp2->Close();

        //Number of seconds created from size of TTree (1 measurement per second fixed rate)
        vector<double> seconds_2layer;

        for(int i = 0; i < t2->GetEntries(); i++)
        {
                seconds_2layer.push_back(t2->GetV1()[i]);
        }
        vector<double> fields_2layer;
        for ( int i = 0; i < t2->GetEntries(); i++ )
        {
                fields_2layer.push_back( -1*t2->GetV3()[i] );  //Hall probe was oriented in the opposite direction to the fiels, this simply flips the field from negative to positive.
        }
        vector<double> applied_fields_2layer;
        for ( int i = 0; i < t2->GetEntries(); i++ )
        {
                applied_fields_2layer.push_back( t2->GetV2()[i]*solenoid_calibration );
        }



	//Read in 2 layer solder

        TTree *t3 = new TTree();
        t3->ReadFile(two_layer_solder.c_str(),"seconds:voltage:field");

        //The vectors have to be drawn before they can be read in. Temporary canvas created for this purpose and then closed immedietly after.
        TCanvas *ctemp3 = new TCanvas();
        t3->Draw("seconds:voltage:field");
	ctemp3->Close();

        //Number of seconds created from size of TTree (1 measurement per second fixed rate)
        vector<double> seconds_solder;

        for(int i = 0; i < 4674; i++)
        {
                seconds_solder.push_back(t3->GetV1()[i]);
        }
        vector<double> fields_solder;
        for ( int i = 0; i < 4674; i++ )
        {
                fields_solder.push_back( -1*t3->GetV3()[i] );  //Hall probe was oriented in the opposite direction to the fiels, this simply flips the field from negative to positive.
        }
        vector<double> applied_fields_solder;
        for ( int i = 0; i < 4674; i++ )
        {
                applied_fields_solder.push_back( t3->GetV2()[i]*solenoid_calibration );
        }


	//Construct plot
	//B_int (2-layer overlap) / B_int (1-layer overlap) vs B_ext + B_int (2-layer SOLDERED) / B_int (1-layer overlap) vs B_ext

	vector<double> Bint2LayerOVERBint1layer, applied_new_2layer;
	int temp_i, temp_j;
	for(int i = 0; i < fields_1layer.size(); i++)
	{
		for(int j = 0; j < fields_2layer.size(); j++)
		{
			temp_i = TMath::Nint( applied_fields_1layer[i]*10.0);
			temp_j = TMath::Nint( applied_fields_2layer[j]*10.0);	
			if(temp_i ==  temp_j)
			{
				if(TMath::Abs(fields_1layer[i]) > 0.00)
				{
					Bint2LayerOVERBint1layer.push_back(fields_2layer[j] / fields_1layer[i]);
					applied_new_2layer.push_back(applied_fields_2layer[j]);
				}
			}
		}
	}

	vector<double> BintSolderOVERBint1layer, applied_new_solder;
	for(int i = 0; i < fields_1layer.size(); i++)
	{
		for(int j = 0; j < fields_solder.size(); j++)
		{
			temp_i = TMath::Nint( applied_fields_1layer[i]*10.0);
			temp_j = TMath::Nint( applied_fields_solder[j]*10.0);	
			if(temp_i ==  temp_j)
			{
				if(TMath::Abs(fields_1layer[i]) > 0.00)
				{
					BintSolderOVERBint1layer.push_back(fields_solder[j] / fields_1layer[i]);
					applied_new_solder.push_back(applied_fields_solder[j]);
				}
			}
		}
	}

	TCanvas *c1 = new TCanvas();
        gr2 = new TGraph(BintSolderOVERBint1layer.size(),&(applied_new_solder[0]),&(BintSolderOVERBint1layer[0]));
                gr2->Draw("AP");
                gr2->GetXaxis()->SetTitle("External Field (mT)");
                gr2->GetYaxis()->SetTitle("B_{i} / B_{1-layer}");
                gr2->SetTitle("");
                gr2->SetMarkerColor(kBlue);
                gr2->SetFillColor(kBlue);
        gr1 = new TGraph(Bint2LayerOVERBint1layer.size(),&(applied_new_2layer[0]),&(Bint2LayerOVERBint1layer[0]));
                gr1->Draw("P SAME");
                gr1->GetXaxis()->SetTitle("External Field (mT)");
                gr1->GetYaxis()->SetTitle("Internal Field (mT)");
                gr1->SetTitle("Internal vs. External Magnetic Field");
                gr1->SetMarkerColor(kGreen);
                gr1->SetFillColor(kGreen);

	TF1 *onetoone = new TF1("onetoone","pol1",0,1500);
		onetoone->SetParameter(0,1);
		onetoone->SetParameter(1,0);
		onetoone->SetLineColor(kBlack);
		onetoone->SetLineStyle(7);
	onetoone->Draw("SAME");

        leg = new TLegend(0.2,0.9,0.7,0.75);
                leg->SetBorderSize(1);
		leg->AddEntry(onetoone,"B_{1} / B_{2} = 1.0","l");
                leg->AddEntry(gr1,"B_{int-solder} / B_{int-1layer}","fp");
                leg->AddEntry(gr2,"B_{int-2layer} / B_{int-1layer}","fp");
//	leg->Draw();

	c1->SetGridx();
	c1->SetGridy();
	c1->SetFixedAspectRatio();







	return 0;
}
