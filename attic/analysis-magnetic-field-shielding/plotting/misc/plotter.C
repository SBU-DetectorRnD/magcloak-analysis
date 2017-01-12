#include <fstream>
#include <string>
#include <math.h>
#include <algorithm>
#include "TGraph.h"

int plotter()
{

//	Additional files at the end

	//measurement of 10 Min sample before heating: 6/8/16
//	std::ifstream file("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_124028.txt"); //first run no cooling
//	std::ifstream file1("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_135913.txt"); //second run with cooling
//	std::ifstream file2("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160610_105850.txt"); //second run with cooling

	//measurement of 15 Min sample before heating: 6/8/16
//	std::ifstream file("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_142717.txt"); //first run no cooling
//	std::ifstream file1("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_144249.txt"); //second run with cooling
//	std::ifstream file2("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160610_130301.txt"); //second run with cooling

	//measurement of 20 Min sample before heating: 6/8/16
//	std::ifstream file("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_150113.txt"); //first run no cooling
//	std::ifstream file1("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_151527.txt"); //second run with cooling
//	std::ifstream file2("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160610_133314.txt"); //second run with cooling

	//measurement of 20 Min sample before heating: 6/8/16
	std::ifstream file("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_150113.txt"); //first run no cooling
	std::ifstream file1("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_151527.txt"); //second run with cooling
	std::ifstream file2("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_160510.txt"); //second run with cooling

	std::string str;
//	bool third_file = false;
	bool third_file = true;

        vector<double> v_t, v_x, v_y;
        double ti, xi, yi;
        while (std::getline(file, str))
        {
                if(file >> ti >> xi >> yi)
                {
                        v_t.push_back(ti); //Time
                        v_x.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
                        v_y.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
                }

        }

       TCanvas *c1 = new TCanvas();
        gr1 = new TGraph(v_x.size(),&(v_x[0]),&(v_y[0]));
                gr1->Draw("AP");
                gr1->GetXaxis()->SetTitle("Voltage (mV)");
                gr1->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr1->SetTitle("Voltage from Power Supply vs. Magnetic Field");
                gr1->SetMarkerColor(kRed);
//		gr1->GetXaxis()->SetRange(0.01,50);
	TF1 *fit1 = new TF1("fit1","pol1",13,300);
	gr1->Fit("fit1","R");
	TF1 *f1 = gr1->GetFunction("fit1");
                f1->SetName("f1");
                f1->SetLineColor(kRed-3);
                f1->SetRange(0,300);
        f1->Draw("SAME");
        c1->Update();

	TCanvas *c2 = new TCanvas();
        gr2 = new TGraph(v_t.size(),&(v_t[0]),&(v_y[0]));
                gr2->Draw("AP");
                gr2->GetXaxis()->SetTitle("Time (s)");
                gr2->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr2->SetTitle("Time vs. Magnetic Field");
                gr2->SetMarkerColor(kRed);
//		gr2->GetXaxis()->SetRange(0.01,50);
        c2->Update();

        vector<double> v_t1, v_x1, v_y1;
        double ti1, xi1, yi1;
        while (std::getline(file1, str))
        {
                if(file1 >> ti1 >> xi1 >> yi1)
                {
                        v_t1.push_back(ti1); //Time
                        v_x1.push_back(TMath::Abs(xi1)); //Current in mA -> Voltage in mV
                        v_y1.push_back(TMath::Abs(yi1)); //Magnetic Field (mT)
                }

        }

        TCanvas *c4 = new TCanvas();
	gr3 = new TGraph(v_x1.size(),&(v_x1[0]),&(v_y1[0]));
//	gr3 = new TGraph(50,&(v_x1[0]),&(v_y1[0]));
                gr3->Draw("AP");
                gr3->GetXaxis()->SetTitle("Voltage (mV)");
                gr3->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr3->SetTitle("Voltage from Power Supply vs. Magnetic Field");
                gr3->SetMarkerColor(kGreen);
                gr3->SetLineColor(kGreen);
	TF1 *fit3 = new TF1("fit3","pol1",50,120);
	gr3->Fit("fit3","R");
	TF1 *f3 = gr3->GetFunction("fit3");
                f3->SetName("f3");
                f3->SetLineColor(kGreen-3);
                f3->SetRange(0,300);
        f3->Draw("SAME");
        c4->Update();

        TCanvas *c5 = new TCanvas();
	gr4 = new TGraph(v_t1.size(),&(v_t1[0]),&(v_y1[0]));
//	gr4 = new TGraph(50,&(v_x1[0]),&(v_y1[0]));
                gr4->Draw("AP");
                gr4->GetXaxis()->SetTitle("Time");
                gr4->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
                gr4->SetTitle("Time vs. Magnetic Field");
                gr4->SetMarkerColor(kGreen);
                gr4->SetLineColor(kGreen);
        c4->Update();

	if(third_file)
	{

		vector<double> v_t2, v_x2, v_y2;
		double ti2, xi2, yi2;
		while (std::getline(file2, str))
		{
			if(file2 >> ti2 >> xi2 >> yi2)
			{
				v_t2.push_back(ti2); //Time
				v_x2.push_back(TMath::Abs(xi2)); //Current in mA -> Voltage in mV
				v_y2.push_back(TMath::Abs(yi2)); //Magnetic Field (mT)
			}

		}

		TCanvas *c7 = new TCanvas();
		gr7 = new TGraph(v_x2.size(),&(v_x2[0]),&(v_y2[0]));
	//	gr7 = new TGraph(50,&(v_x2[0]),&(v_y2[0]));
			gr7->Draw("AP");
			gr7->GetXaxis()->SetTitle("Voltage (mV)");
			gr7->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
			gr7->SetTitle("Voltage from Power Supply vs. Magnetic Field");
			gr7->SetMarkerColor(kBlue);
			gr7->SetLineColor(kBlue);
		TF1 *fit7 = new TF1("fit7","pol1",13,300);
		gr7->Fit("fit7", "R");
		TF1 *f7 = gr7->GetFunction("fit7");
			f7->SetName("f7");
			f7->SetLineColor(kBlue-3);
			f7->SetRange(0,300);
		f7->Draw("SAME");
	        c7->Update();

		TCanvas *c8 = new TCanvas();
		gr8 = new TGraph(v_t2.size(),&(v_t2[0]),&(v_y2[0]));
	//	gr8 = new TGraph(50,&(v_x2[0]),&(v_y2[0]));
			gr8->Draw("AP");
			gr8->GetXaxis()->SetTitle("Time");
			gr8->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
			gr8->SetTitle("Time vs. Magnetic Field");
			gr8->SetMarkerColor(kBlue);
			gr8->SetLineColor(kBlue);
		c8->Update();
	}

	TCanvas *c6 = new TCanvas();
	gr1->Draw("AP");
	gr3->Draw("P SAME");
	if(third_file)
		gr7->Draw("P SAME");
        leg3 = new TLegend(0.2,0.8,0.48,0.9);
                leg3->AddEntry(gr1,"Room Temperature","p");
                leg3->AddEntry(gr3,"Cryo","p");
        	if(third_file)
		        leg3->AddEntry(gr7,"Cryo After Baking","p");
	leg3->Draw();
	c6->Update();

	double shielding = TMath::Abs((fit1->GetParameter(0)) - fit3->GetParameter(0));
	cout << fit1->Eval(80) << endl;
	cout << fit3->Eval(80) << endl;
	cout << fit7->Eval(80) << endl;
	cout << fit7->Eval(80) - fit3->Eval(80) << endl;

	cout << endl << "Approximate Shielding (y-intercept): " << shielding << "mT" << endl;

	return 0;
}




	//Measurement 1: 5/27/16
//	std::ifstream file("./DataFile_160527_104031.txt"); //First run no cooling
//	std::ifstream file1("./DataFile_160527_105543.txt"); //Second run with cooling

	//Measurement 2: 5/27/16
//	std::ifstream file("./5_27_16/DataFile_160527_115536.txt"); //First run no cooling
//	std::ifstream file1("./5_27_16/DataFile_160527_121004.txt"); //Second run with cooling
//	std::ifstream file1("./DataFile_160527_125222.txt"); //Second run with cooling again

//	Measurement 3: 5/31/16
//	std::ifstream file("./5-31-16/LabVIEWDataFile_160531_104004.txt"); //First run no cooling
//	std::ifstream file1("./5-31-16/LabVIEWDataFile_160531_105613.txt"); //Second run with cooling

	//Measurement 1 of SC tube for VDG prototype: 6/1/16
//	std::ifstream file("./6-1-16/DataFile_160601_111440.txt"); //First run no cooling
//	std::ifstream file1("./6-1-16/DataFile_160601_112021.txt"); //Second run with cooling

	//Measurement 5: 6/2/16
//	std::ifstream file("./6-2-16/DataFile_160602_132724.txt"); //First run no cooling
//	std::ifstream file1("./6-2-16/DataFile_160602_134458.txt"); //Second run with cooling

	//Measurement 6: 6/2/16 --- After 10 minutes of heating at 500 F
//	std::ifstream file("./6-2-16/DataFile_160602_150406.txt"); //First run no cooling
//	std::ifstream file1("./6-2-16/DataFile_160602_152511.txt"); //Second run with cooling

	//Measurement 7: 6/2/16 --- After 30 minutes of additional heating at 500 F
//	std::ifstream file("./6-2-16/DataFile_160602_170539.txt"); //First run no cooling
//	std::ifstream file1("./6-2-16/DataFile_160602_172125.txt"); //Second run with cooling


	//measurement of 10 Min sample before heating: 6/3/16
//	std::ifstream file("./6-3-16/DataFile_160603_105304.txt"); //first run no cooling
//	std::ifstream file1("./6-3-16/DataFile_160603_111607.txt"); //second run with cooling
//	std::ifstream file2("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160607_121726.txt"); //second run with cooling

	//measurement of 20 Min sample before heating: 6/3/16
//	std::ifstream file("./6-3-16/DataFile_160603_114313.txt"); //first run no cooling
//	std::ifstream file1("./6-3-16/DataFile_160603_120143.txt"); //second run with cooling
//	std::ifstream file1("./6-3-16/DataFile_160603_123432.txt"); //second run with cooling
//	std::ifstream file1("./6-3-16/DataFile_160603_145645.txt"); //second run with cooling
//	std::ifstream file("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160607_123841.txt"); //first run no cooling
//	std::ifstream file1("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160607_131134.txt"); //second run with cooling

	//measurement of 30 Min sample before heating: 6/3/16
//	std::ifstream file("./6-3-16/DataFile_160603_141347.txt"); //first run no cooling
//	std::ifstream file1("./6-3-16/DataFile_160603_143027.txt"); //second run with cooling
//	std::ifstream file2("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160607_150843.txt"); //second run with cooling



