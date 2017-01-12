int report_plots()
{
//Measurement Files
	//measurement of 10 Min sample before heating: 6/8/16
	std::ifstream ten_file("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_124028.txt"); //first run no cooling
	std::ifstream ten_file1("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_135913.txt"); //second run with cooling
	std::ifstream ten_file2("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160610_105850.txt"); //second run with cooling

	//measurement of 15 Min sample before heating: 6/8/16
	std::ifstream fifteen_file("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_142717.txt"); //first run no cooling
	std::ifstream fifteen_file1("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_144249.txt"); //second run with cooling
	std::ifstream fifteen_file2("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160610_130301.txt"); //second run with cooling

	//measurement of 20 Min sample before heating: 6/8/16
	std::ifstream twenty_file("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_150113.txt"); //first run no cooling
	std::ifstream twenty_file1("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_151527.txt"); //second run with cooling
	std::ifstream twenty_file2("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160610_133314.txt"); //second run with cooling

	//Measurement of 30 min Sample before heating
	std::ifstream thirty_file("./6-3-16/DataFile_160603_141347.txt"); //first run no cooling
	std::ifstream thirty_file1("./6-3-16/DataFile_160603_143027.txt"); //second run with cooling
	std::ifstream thirty_file2("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160607_150843.txt"); //second run with cooling

//Read in the measurement files

	std::string str;
	double ti, xi, yi;
	vector<double> ten_time, ten_field, ten_voltage;
	vector<double> ten_time_1, ten_field_1, ten_voltage_1;
	vector<double> ten_time_2, ten_field_2, ten_voltage_2;
	vector<double> fifteen_time, fifteen_field, fifteen_voltage;
	vector<double> fifteen_time_1, fifteen_field_1, fifteen_voltage_1;
	vector<double> fifteen_time_2, fifteen_field_2, fifteen_voltage_2;
	vector<double> twenty_time, twenty_field, twenty_voltage;
	vector<double> twenty_time_1, twenty_field_1, twenty_voltage_1;
	vector<double> twenty_time_2, twenty_field_2, twenty_voltage_2;
	vector<double> thirty_time, thirty_field, thirty_voltage;
	vector<double> thirty_time_1, thirty_field_1, thirty_voltage_1;
	vector<double> thirty_time_2, thirty_field_2, thirty_voltage_2;

	//10 Minute Sample
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

	while (std::getline(ten_file2, str))
	{
		if(ten_file2 >> ti >> xi >> yi)
		{
			ten_time_2.push_back(ti); //Time
			ten_voltage_2.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
			ten_field_2.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
		}
	}


	//15 Minute Sample
	while (std::getline(fifteen_file, str))
	{
		if(fifteen_file >> ti >> xi >> yi)
		{
			fifteen_time.push_back(ti); //Time
			fifteen_voltage.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
			fifteen_field.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
		}
	}

	while (std::getline(fifteen_file1, str))
	{
		if(fifteen_file1 >> ti >> xi >> yi)
		{
			fifteen_time_1.push_back(ti); //Time
			fifteen_voltage_1.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
			fifteen_field_1.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
		}
	}

	while (std::getline(fifteen_file2, str))
	{
		if(fifteen_file2 >> ti >> xi >> yi)
		{
			fifteen_time_2.push_back(ti); //Time
			fifteen_voltage_2.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
			fifteen_field_2.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
		}
	}


	//20 Minute Sample
	while (std::getline(twenty_file, str))
	{
		if(twenty_file >> ti >> xi >> yi)
		{
			twenty_time.push_back(ti); //Time
			twenty_voltage.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
			twenty_field.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
		}
	}

	while (std::getline(twenty_file1, str))
	{
		if(twenty_file1 >> ti >> xi >> yi)
		{
			twenty_time_1.push_back(ti); //Time
			twenty_voltage_1.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
			twenty_field_1.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
		}
	}

	while (std::getline(twenty_file2, str))
	{
		if(twenty_file2 >> ti >> xi >> yi)
		{
			twenty_time_2.push_back(ti); //Time
			twenty_voltage_2.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
			twenty_field_2.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
		}
	}

	//30 Minute Sample
	while (std::getline(thirty_file, str))
	{
		if(thirty_file >> ti >> xi >> yi)
		{
			thirty_time.push_back(ti); //Time
			thirty_voltage.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
			thirty_field.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
		}
	}

	while (std::getline(thirty_file1, str))
	{
		if(thirty_file1 >> ti >> xi >> yi)
		{
			thirty_time_1.push_back(ti); //Time
			thirty_voltage_1.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
			thirty_field_1.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
		}
	}

	while (std::getline(thirty_file2, str))
	{
		if(thirty_file2 >> ti >> xi >> yi)
		{
			thirty_time_2.push_back(ti); //Time
			thirty_voltage_2.push_back(TMath::Abs(xi)); //Current in mA -> Voltage in mV
			thirty_field_2.push_back(TMath::Abs(yi)); //Magnetic Field (mT)
		}
	}

//Make plots

	//10 Minute Sample
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
		f1->SetLineColor(kRed-3);
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

	leg3 = new TLegend(0.2,0.8,0.48,0.9);
		leg3->AddEntry(gr1,"Room Temperature","p");
		leg3->AddEntry(gr2,"Cryo","p");
		leg3->AddEntry(gr3,"Cryo After Baking","p");
	leg3->Draw();


	//15 Minute sample
	TCanvas *c4 = new TCanvas();
	gr4 = new TGraph(fifteen_time.size(),&(fifteen_voltage[0]),&(fifteen_field[0]));
		gr4->Draw("AP");
		gr4->GetXaxis()->SetTitle("Voltage (mV)");
		gr4->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr4->SetTitle("Voltage from Power Supply vs. Magnetic Field --- 15 Minutes");
		gr4->SetMarkerColor(kRed);
	TF1 *fit4 = new TF1("fit4","pol1",13,300);
	gr4->Fit("fit4","R");
	TF1 *f4 = gr4->GetFunction("fit4");
		f4->SetName("f4");
		f4->SetLineColor(kRed-3);
		f4->SetRange(0,300);
		f4->Draw("SAME");
	c4->Update();

	gr5 = new TGraph(fifteen_time_1.size(),&(fifteen_voltage_1[0]),&(fifteen_field_1[0]));
		gr5->Draw("p SAME");
		gr5->GetXaxis()->SetTitle("Voltage (mV)");
		gr5->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr5->SetTitle("Voltage from Power Supply vs. Magnetic Field");
		gr5->SetMarkerColor(kBlue);
	TF1 *fit5 = new TF1("fit5","pol1",60,300);
	gr5->Fit("fit5","R");
	TF1 *f5 = gr5->GetFunction("fit5");
		f5->SetName("f5");
		f5->SetLineColor(kBlue-3);
		f5->SetRange(0,300);
		f5->Draw("SAME");
	c4->Update();

	gr6 = new TGraph(fifteen_time_2.size(),&(fifteen_voltage_2[0]),&(fifteen_field_2[0]));
		gr6->Draw("p SAME");
		gr6->GetXaxis()->SetTitle("Voltage (mV)");
		gr6->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr6->SetTitle("Voltage from Power Supply vs. Magnetic Field");
		gr6->SetMarkerColor(kGreen);
	TF1 *fit6 = new TF1("fit6","pol1",60,300);
	gr6->Fit("fit6","R");
	TF1 *f6 = gr6->GetFunction("fit6");
		f6->SetName("f2");
		f6->SetLineColor(kGreen-3);
		f6->SetRange(0,300);
		f6->Draw("SAME");
	c4->Update();

	leg4 = new TLegend(0.2,0.8,0.48,0.9);
		leg4->AddEntry(gr4,"Room Temperature","p");
		leg4->AddEntry(gr5,"Cryo","p");
		leg4->AddEntry(gr6,"Cryo After Baking","p");
	leg4->Draw();

	//20 Minute sample
	TCanvas *c7 = new TCanvas();
	gr7 = new TGraph(twenty_time.size(),&(twenty_voltage[0]),&(twenty_field[0]));
		gr7->Draw("AP");
		gr7->GetXaxis()->SetTitle("Voltage (mV)");
		gr7->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr7->SetTitle("Voltage from Power Supply vs. Magnetic Field --- 20 Minutes");
		gr7->SetMarkerColor(kRed);
	TF1 *fit7 = new TF1("fit7","pol1",13,300);
	gr7->Fit("fit7","R");
	TF1 *f7 = gr7->GetFunction("fit7");
		f7->SetName("f7");
		f7->SetLineColor(kRed-3);
		f7->SetRange(0,300);
		f7->Draw("SAME");
	c7->Update();

	gr8 = new TGraph(twenty_time_1.size(),&(twenty_voltage_1[0]),&(twenty_field_1[0]));
		gr8->Draw("p SAME");
		gr8->GetXaxis()->SetTitle("Voltage (mV)");
		gr8->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr8->SetTitle("Voltage from Power Supply vs. Magnetic Field");
		gr8->SetMarkerColor(kBlue);
	TF1 *fit8 = new TF1("fit8","pol1",60,300);
	gr8->Fit("fit8","R");
	TF1 *f8 = gr8->GetFunction("fit8");
		f8->SetName("f8");
		f8->SetLineColor(kBlue-3);
		f8->SetRange(0,300);
		f8->Draw("SAME");
	c7->Update();

	gr9 = new TGraph(twenty_time_2.size(),&(twenty_voltage_2[0]),&(twenty_field_2[0]));
		gr9->Draw("p SAME");
		gr9->GetXaxis()->SetTitle("Voltage (mV)");
		gr9->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr9->SetTitle("Voltage from Power Supply vs. Magnetic Field");
		gr9->SetMarkerColor(kGreen);
	TF1 *fit9 = new TF1("fit9","pol1",60,300);
	gr9->Fit("fit9","R");
	TF1 *f9 = gr9->GetFunction("fit9");
		f9->SetName("f2");
		f9->SetLineColor(kGreen-3);
		f9->SetRange(0,300);
		f9->Draw("SAME");
	c7->Update();

	leg5 = new TLegend(0.2,0.8,0.48,0.9);
		leg5->AddEntry(gr7,"Room Temperature","p");
		leg5->AddEntry(gr8,"Cryo","p");
		leg5->AddEntry(gr9,"Cryo After Baking","p");
	leg5->Draw();


	//30 Minute sample
	TCanvas *c10 = new TCanvas();
	gr10 = new TGraph(thirty_time.size(),&(thirty_voltage[0]),&(thirty_field[0]));
		gr10->Draw("AP");
		gr10->GetXaxis()->SetTitle("Voltage (mV)");
		gr10->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr10->SetTitle("Voltage from Power Supply vs. Magnetic Field --- 30 Minutes");
		gr10->SetMarkerColor(kRed);
	TF1 *fit10 = new TF1("fit10","pol1",13,300);
	gr10->Fit("fit10","R");
	TF1 *f10 = gr10->GetFunction("fit10");
		f10->SetName("f10");
		f10->SetLineColor(kRed-3);
		f10->SetRange(0,300);
		f10->Draw("SAME");
	c10->Update();

	gr11 = new TGraph(thirty_time_1.size(),&(thirty_voltage_1[0]),&(thirty_field_1[0]));
		gr11->Draw("p SAME");
		gr11->GetXaxis()->SetTitle("Voltage (mV)");
		gr11->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr11->SetTitle("Voltage from Power Supply vs. Magnetic Field");
		gr11->SetMarkerColor(kBlue);
	TF1 *fit11 = new TF1("fit11","pol1",60,300);
	gr11->Fit("fit11","R");
	TF1 *f11 = gr11->GetFunction("fit11");
		f11->SetName("f11");
		f11->SetLineColor(kBlue-3);
		f11->SetRange(0,300);
		f11->Draw("SAME");
	c10->Update();

	gr12 = new TGraph(thirty_time_2.size(),&(thirty_voltage_2[0]),&(thirty_field_2[0]));
		gr12->Draw("p SAME");
		gr12->GetXaxis()->SetTitle("Voltage (mV)");
		gr12->GetYaxis()->SetTitle("Magnitude of Magnetic Field (mT)");
		gr12->SetTitle("Voltage from Power Supply vs. Magnetic Field");
		gr12->SetMarkerColor(kGreen);
	TF1 *fit12 = new TF1("fit12","pol1",60,300);
	gr12->Fit("fit12","R");
	TF1 *f12 = gr12->GetFunction("fit12");
		f12->SetName("f2");
		f12->SetLineColor(kGreen-3);
		f12->SetRange(0,300);
		f12->Draw("SAME");
	c7->Update();

	leg6 = new TLegend(0.2,0.8,0.48,0.9);
		leg6->AddEntry(gr10,"Room Temperature","p");
		leg6->AddEntry(gr11,"Cryo","p");
		leg6->AddEntry(gr12,"Cryo After Baking","p");
	leg6->Draw();

	//Get parameters from fits
	vector<double>  shielding;
	vector<double>  baking_time;
	baking_time.push_back(10);
	baking_time.push_back(15);
	baking_time.push_back(20);
	baking_time.push_back(30);
	
	shielding.push_back( (fit1->Eval(100) - fit3->Eval(100)) / (fit1->Eval(100) - fit2->Eval(100)) * 100 );
	shielding.push_back( (fit4->Eval(100) - fit6->Eval(100)) / (fit4->Eval(100) - fit5->Eval(100)) * 100 );
	shielding.push_back( (fit7->Eval(100) - fit9->Eval(100)) / (fit7->Eval(100) - fit8->Eval(100)) * 100 );
//	shielding.push_back( (fit7->Eval(100) - fit8->Eval(100)) / (fit7->Eval(100) - fit9->Eval(100)) * 100 );
	shielding.push_back( (fit10->Eval(100) - fit12->Eval(100)) / (fit10->Eval(100) - fit11->Eval(100)) * 100 );

	vector<double> shielding_error, baking_error;
	baking_error.push_back(0.00);	
	baking_error.push_back(0.00);	
	baking_error.push_back(0.00);	
	baking_error.push_back(0.00);	

	double angled_error = 5.07741*1.5;
	shielding_error.push_back(angled_error);
	shielding_error.push_back(angled_error);
	shielding_error.push_back(angled_error);
	shielding_error.push_back(angled_error);

	//Plot shielding percentages
	TCanvas *c13 = new TCanvas();
	c13->SetFixedAspectRatio();
	gr13 = new TGraphErrors(baking_time.size(),&(baking_time[0]),&(shielding[0]),&baking_error[0],&shielding_error[0]);
		gr13->Draw("AP");
		gr13->GetXaxis()->SetTitle("Time in Oven (Minutes)");
		gr13->GetYaxis()->SetTitle("Percentage of Original Shielding");
		gr13->SetTitle("Baking Time vs. Shielding Retention");
		gr13->SetMarkerColor(kRed);
//	TF1 *expdecay = new TF1("expdecay","-[0] / (1 + [1]*TMath::Exp([2]*x) )",0,35);
//		expdecay->SetParameter(0,100);
//		expdecay->SetParameter(1,2);
//		expdecay->SetParameter(1,2);
//		gr13->Fit("expdecay");
//		TF1 *f13 = gr13->GetFunction("expdecay");
		TF1 *f13 = new TF1("f13","pol1",0,40);
			f13->SetParameter(0,100);
			f13->SetParameter(1,0);
		f13->SetName("f13");
		f13->SetLineColor(kBlue-3);
		f13->SetLineStyle(7);
		f13->SetRange(0,35);
	f13->Draw("SAME");
	leg7 = new TLegend(0.2,0.3,0.65,0.2);
//		leg7->AddEntry(f13,"-a/(1 + b*e^(c*x))","l");
		leg7->AddEntry(f13,"100\% of Shielding Before Baking","l");
	leg7->Draw();
//	gStyle->SetOptFit();
	c13->Update();
//	gStyle->SetOptFit(kFALSE);

	return 0;
}
