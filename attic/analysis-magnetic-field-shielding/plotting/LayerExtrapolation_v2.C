#include "headers/DataCut.h"
#include "headers/exist_test.h"
#include "headers/get_calibration"

int n = 1;
bool onetoone_leg = false;

int LayerExtrapolation_v2()
{

	std::string file_txt = "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_102426.txt";	//Room temperature calibration 
	std::string file1_txt = "/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_222417.txt";	//Before baking
	std::string title = "1 Layer YBCO (lN2)";
	TCanvas *c00 = new TCanvas();
	leg = new TLegend(0.2,0.9,0.7,0.75);
		leg->SetBorderSize(1);

	//Measurement Files

        vector<double> time_1, field_1, voltage_1;

	unpack_file(file1_txt, time_1, voltage_1, field_1);
	for(int i = 0; i < voltage_1.size(); i++)
	{
		voltage_1[i] = TMath::Abs(voltage_1[i]);
		field_1[i] = TMath::Abs(field_1[i]);
	}

        vector<double> voltageTOfield_1;
	double calibration_factor = TMath::Abs(get_calibration_quiet(file_txt));

        for(int i = 0; i < voltage_1.size(); i++)
        {
                      voltageTOfield_1.push_back(voltage_1[i]*calibration_factor);
	}


	c00->cd();
        gr12 = new TGraph(time_1.size(),&(voltageTOfield_1[0]),&(field_1[0]));
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

	//Second layer

	double internalField_2_i = 0;
	bool ding = true;
	bool dong = true;
	vector<double> appliedField_layer2, internalField_layer2;
	for (int i = 0; i < 1020; i++)
	{
		appliedField_layer2.push_back(i/10.0);
		for(int j = 0; j<voltageTOfield_1.size();j++)
		{
			if(voltageTOfield_1[j] > i/10.0)
			{
				if (ding == true) 
				{
					//cout << field_1[j] << endl;
					for(int k = 0; k < voltageTOfield_1.size();k++)
					{
						if(dong = true)
						{
							if(field_1[j] > voltageTOfield_1[k]) internalField_2_i = field_1[k];	
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
					//cout << field_1[j] << endl;
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

	/*
	//Comparison with 2 layer data
	TTree *t1 = new TTree();
	t1->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160701_155007.txt","time:voltage:field");
	t1->SetMarkerColor(n);
	t1->Draw("-1*field:voltage*25.4","","SAME");
	leg->AddEntry(t1,"2 layer measured","p");
	*/
	c00->Update();
	leg->Draw();

	cout << "All done with: " << title << endl;
	return 0;
}
