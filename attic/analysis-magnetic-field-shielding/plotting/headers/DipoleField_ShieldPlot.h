#include "exist_test.h"
#include "shade.h"

double mm_to_inches(double mm)
{
	return (mm*0.03937); //1 mm = 0.03937 inches
}


double GetFieldMapFactor(double diameter_mm)
{

	cout << "       Finding Field Map Factor" << endl;
	double FieldMapFactor;
	
        double z_offset = 1.000;
        double z_scale = 1.000;
        double field_offset = -300.3000;
        double field_scale = -1.000;

        TTree *t = new TTree();
        std::string file = "/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/DataFile_160720_162008.txt";

        //Check to see if file exists. If so read in file into TTree.
        if(exists_test(file))
        {
                t->ReadFile(file.c_str(),"z:field:non:important:numbers");
        }
        else
        {
                cerr << "ERROR: File not found" << endl;
                return -1;
        }

        //Create new branches for offsets and multiplicative factors
        TBranch *b_field_offset = t->Branch("field_offset",&field_offset,"pt/D");
        TBranch *b_field_scale = t->Branch("field_scale",&field_scale,"pt/D");
        TBranch *b_z_offset = t->Branch("z_offset",&z_offset,"pt/D");
        TBranch *b_z_scale = t->Branch("z_scale",&z_scale,"pt/D");

        //Plot the data with appropriate offsets
	TCanvas *ctemp = new TCanvas();
        t->Draw("(field_scale*field)-field_offset:(z_scale*z)-z_offset");

        TF1 *fit1 = new TF1("fit1","gaus");
        Graph->Fit("fit1","Q");

	FieldMapFactor = fit1->Eval(fit1->GetParameter(1) - diameter_mm/2.0) / fit1->Eval(fit1->GetParameter(1));

	//Draw lines in the center and the diameter 
	TLine *center_line = new TLine(fit1->GetParameter(1), ctemp->GetUymin(), fit1->GetParameter(1), ctemp->GetUymax());
	TLine *diameter_line = new TLine(fit1->GetParameter(1) - diameter_mm/2.0 , ctemp->GetUymin(), fit1->GetParameter(1) - diameter_mm/2.0, ctemp->GetUymax());
		diameter_line->SetLineStyle(7);
	TLine *diameter_line_2 = new TLine(fit1->GetParameter(1) + diameter_mm/2.0 , ctemp->GetUymin(), fit1->GetParameter(1) + diameter_mm/2.0, ctemp->GetUymax());
		diameter_line_2->SetLineStyle(7);
	center_line->Draw("l SAME");
	diameter_line->Draw("l SAME");
	diameter_line_2->Draw("l SAME");

        cout << "              The Field Map Factor for a " << mm_to_inches(diameter_mm) <<"\" Cylinder is: " << FieldMapFactor << endl;

	ctemp->Close();
	return FieldMapFactor;

}

int DipoleFieldShieldPlot(std::string file_path, std::string summary_file_path, std::string ten_file1_txt, std::string title, TCanvas &c00, TLegend &leg, int color, std::string helm_or_sol, std::string negate, int file_end, double inner_diameter_mm, double outer_diameter_mm)
{
	cout << "*******************************************************" << endl << "Beginning: " << title  << " WITH correction for dipole field" << endl;
	cout << "       Cylinder Outer/Inner Diameter (mm): " << outer_diameter_mm << "/" << inner_diameter_mm << endl;
//Measurement Files

	std::string file_end_str = Form("%d",file_end);

	std::string python_input = "python plot_output.py \""+file_path+"\" \""+ten_file1_txt+"\" "+negate+" "+helm_or_sol+" "+file_end_str;

	cout << "       Python Command Executed for: " << ten_file1_txt << endl;
	cout << "       [If this plot looks odd, you may have to adjust the parameters in 'plot_output.py' directly]" << endl;
	gSystem->Exec(python_input.c_str());

	std::string ten_file1_root = (ten_file1_txt.erase(ten_file1_txt.size()-3,3)).append("root");	

	if(exist_test((summary_file_path+"SummaryOf_"+ten_file1_root).c_str()))
	{
		cout << "       Summary File Found. Proceeding with Plot" << endl;
	} 
	else 
	{
		cerr << "ERROR: Summary File for " << ten_file1_txt << " not found. Aborting." << endl;
		return -1;
	}   
	
	//Read the measurement summary file into vectors

	TFile f((summary_file_path+"SummaryOf_"+ten_file1_root).c_str());
	TCanvas *ctemp = new TCanvas();
	t->Draw("Bo:Bi:BoErr:BiErr");
	ctemp->Close();

        vector<double> AppliedField, AppliedField_err, AppliedField_Noerr, InternalField, InternalField_err;
        vector<double> AppliedField_DipoleCorrection_Outer, AppliedField_DipoleCorrection_Inner;

	double fieldAtEdge_vs_fieldAtCenter = GetFieldMapFactor(outer_diameter_mm);
	double fieldAtInner_vs_fieldAtCenter = GetFieldMapFactor(inner_diameter_mm);

	for(int i = 0; i < t->GetEntries(); i++)
        {
                        AppliedField.push_back((t->GetV1())[i]); 			//Calculated Applied Field
                        AppliedField_DipoleCorrection_Outer.push_back((t->GetV1())[i] / fieldAtEdge_vs_fieldAtCenter); 			//Calculated Applied Field
                        AppliedField_DipoleCorrection_Inner.push_back((t->GetV1())[i] / fieldAtInner_vs_fieldAtCenter); 			//Calculated Applied Field
			if((t->GetV3())[i] > 0.01)
			{
                        	AppliedField_err.push_back((t->GetV3())[i]); 		//Error in applied field
			}
			else
			{
				AppliedField_err.push_back(0.01);
			}
                        AppliedField_Noerr.push_back(0.00); 				//If error in applied field is to be neglected, use this instead of true error
                        InternalField.push_back((t->GetV2())[i]); 			//Measured Internal Field
                        InternalField_err.push_back((t->GetV4())[i]); 		//Error in measurement of internal field
        }

	//Plot the applied field vs. internal field on the canvas which was passed in by reference
	c00.cd();
	//This is the minimum shielding, using the field at the center of the superconductor
	gr12 = new TGraphErrors(AppliedField_DipoleCorrection_Inner.size(),&(AppliedField_DipoleCorrection_Inner[0]),&(InternalField[0]),&(AppliedField_Noerr[0]),&(InternalField_err[0]));
		gr12->Draw("PL SAME");
                gr12->SetMarkerColor(color);
                gr12->SetLineColor(color);

	//This is the maximum shielding, using the field at the edge of the superconductor (extrapolated from field map)
	gr13 = new TGraphErrors(AppliedField_DipoleCorrection_Outer.size(),&(AppliedField_DipoleCorrection_Outer[0]),&(InternalField[0]),&(AppliedField_Noerr[0]),&(InternalField_err[0]));
		gr13->Draw("PL SAME");
                gr13->SetMarkerColor(color);
                gr13->SetLineColor(color);

	//Draw a crosshatching between the two curves
	int n = AppliedField_DipoleCorrection_Outer.size();
	TGraph *grshade = new TGraph(2*AppliedField_DipoleCorrection_Outer.size());
	for (int i=0;i<n;i++) 
	{
		grshade->SetPoint(i,AppliedField_DipoleCorrection_Inner[i],InternalField[i]);
		grshade->SetPoint(n+i,AppliedField_DipoleCorrection_Outer[n-i-1],InternalField[n-i-1]);
	}
	grshade->SetFillStyle(3013);
	grshade->SetFillColor(color);
	grshade->Draw("f SAME");

	c00.Update();
	
	//Add the above plot(s) to the legend passed in by reference
	leg.AddEntry(gr12,title.c_str(),"pl");

	cout << "All done with: " << title << endl;
	return 0;
}


int DipoleFieldShieldPlot_existing(std::string file_path, std::string summary_file_path, std::string ten_file1_txt, std::string title, TCanvas &c00, TLegend &leg, int color, std::string helm_or_sol, std::string negate, int file_end, double inner_diameter_mm, double outer_diameter_mm)
{
	cout << "*******************************************************" << endl << "Beginning: " << title  << " WITH correction for dipole field" << endl;
	cout << "       Cylinder Outer/Inner Diameter (mm): " << outer_diameter_mm << "/" << inner_diameter_mm << endl;
//Measurement Files

	std::string ten_file1_root = (ten_file1_txt.erase(ten_file1_txt.size()-3,3)).append("root");	

	if(exist_test((summary_file_path+"SummaryOf_"+ten_file1_root).c_str()))
	{
		cout << "       Summary File Found. Proceeding with Plot" << endl;
	} 
	else 
	{
		cerr << "ERROR: Summary File for " << ten_file1_txt << " not found. Aborting." << endl;
		return -1;
	}   
	
	//Read the measurement summary file into vectors

	TFile f((summary_file_path+"SummaryOf_"+ten_file1_root).c_str());
	TCanvas *ctemp = new TCanvas();
	t->Draw("Bo:Bi:BoErr:BiErr");
	ctemp->Close();

        vector<double> AppliedField, AppliedField_err, AppliedField_Noerr, InternalField, InternalField_err;
        vector<double> AppliedField_DipoleCorrection_Outer, AppliedField_DipoleCorrection_Inner;

	double fieldAtEdge_vs_fieldAtCenter = GetFieldMapFactor(outer_diameter_mm);
	double fieldAtInner_vs_fieldAtCenter = GetFieldMapFactor(inner_diameter_mm);

	for(int i = 0; i < t->GetEntries(); i++)
        {
                        AppliedField.push_back((t->GetV1())[i]); 			//Calculated Applied Field
                        AppliedField_DipoleCorrection_Outer.push_back((t->GetV1())[i] / fieldAtEdge_vs_fieldAtCenter); 			//Calculated Applied Field
                        AppliedField_DipoleCorrection_Inner.push_back((t->GetV1())[i] / fieldAtInner_vs_fieldAtCenter); 			//Calculated Applied Field
			if((t->GetV3())[i] > 0.01)
			{
                        	AppliedField_err.push_back((t->GetV3())[i]); 		//Error in applied field
			}
			else
			{
				AppliedField_err.push_back(0.01);
			}
                        AppliedField_Noerr.push_back(0.00); 				//If error in applied field is to be neglected, use this instead of true error
                        InternalField.push_back((t->GetV2())[i]); 			//Measured Internal Field
                        InternalField_err.push_back((t->GetV4())[i]); 		//Error in measurement of internal field
        }

	//Plot the applied field vs. internal field on the canvas which was passed in by reference
	c00.cd();
	//This is the minimum shielding, using the field at the center of the superconductor
	gr12 = new TGraphErrors(AppliedField_DipoleCorrection_Inner.size(),&(AppliedField_DipoleCorrection_Inner[0]),&(InternalField[0]),&(AppliedField_Noerr[0]),&(InternalField_err[0]));
		gr12->Draw("PL SAME");
                gr12->SetMarkerColor(color);
                gr12->SetLineColor(color);

	//This is the maximum shielding, using the field at the edge of the superconductor (extrapolated from field map)
	gr13 = new TGraphErrors(AppliedField_DipoleCorrection_Outer.size(),&(AppliedField_DipoleCorrection_Outer[0]),&(InternalField[0]),&(AppliedField_Noerr[0]),&(InternalField_err[0]));
		gr13->Draw("PL SAME");
                gr13->SetMarkerColor(color);
                gr13->SetLineColor(color);

	//Draw a crosshatching between the two curves
	int n = AppliedField_DipoleCorrection_Outer.size();
	TGraph *grshade = new TGraph(2*AppliedField_DipoleCorrection_Outer.size());
	for (int i=0;i<n;i++) 
	{
		grshade->SetPoint(i,AppliedField_DipoleCorrection_Inner[i],InternalField[i]);
		grshade->SetPoint(n+i,AppliedField_DipoleCorrection_Outer[n-i-1],InternalField[n-i-1]);
	}
	grshade->SetFillStyle(3013);
	grshade->SetFillColor(color);
	grshade->Draw("f SAME");

	c00.Update();
	
	//Add the above plot(s) to the legend passed in by reference
	leg.AddEntry(gr12,title.c_str(),"pl");

	cout << "All done with: " << title << endl;
	return 0;
}


int DipoleFieldShieldPlot_single(std::string file_path, std::string summary_file_path, std::string ten_file1_txt, std::string title, TCanvas &c00, TLegend &leg, int color, std::string helm_or_sol, std::string negate, int file_end, double outer_diameter_mm)
{
	cout << "*******************************************************" << endl << "Beginning: " << title  << " WITH correction for dipole field" << endl;
	cout << "       Single Plot with Diameter (mm): " << outer_diameter_mm << endl;
//Measurement Files

	std::string file_end_str = Form("%d",file_end);

	std::string python_input = "python plot_output.py \""+file_path+"\" \""+ten_file1_txt+"\" "+negate+" "+helm_or_sol+" "+file_end_str;

	cout << "       Python Command Executed for: " << ten_file1_txt << endl;
	cout << "       [If this plot looks odd, you may have to adjust the parameters in 'plot_output.py' directly]" << endl;
	gSystem->Exec(python_input.c_str());


	std::string ten_file1_root = (ten_file1_txt.erase(ten_file1_txt.size()-3,3)).append("root");	

	if(exist_test((summary_file_path+"SummaryOf_"+ten_file1_root).c_str()))
	{
		cout << "       Summary File Found. Proceeding with Plot" << endl;
	} 
	else 
	{
		cerr << "ERROR: Summary File for " << ten_file1_txt << " not found. Aborting." << endl;
		return -1;
	}   
	
	//Read the measurement summary file into vectors
	TFile f((summary_file_path+"SummaryOf_"+ten_file1_root).c_str());
	TCanvas *ctemp = new TCanvas();
	t->Draw("Bo:Bi:BoErr:BiErr");
	ctemp->Close();

        vector<double> AppliedField, AppliedField_err, AppliedField_Noerr, InternalField, InternalField_err;
        vector<double> AppliedField_DipoleCorrection_Outer;

	double fieldAtEdge_vs_fieldAtCenter = GetFieldMapFactor(outer_diameter_mm);

	for(int i = 0; i < t->GetEntries(); i++)
        {
                        AppliedField.push_back((t->GetV1())[i]); 			//Calculated Applied Field
                        AppliedField_DipoleCorrection_Outer.push_back((t->GetV1())[i] / fieldAtEdge_vs_fieldAtCenter); 			//Calculated Applied Field
			if((t->GetV3())[i] > 0.01)
			{
                        	AppliedField_err.push_back((t->GetV3())[i]); 		//Error in applied field
			}
			else
			{
				AppliedField_err.push_back(0.01);
			}
                        AppliedField_Noerr.push_back(0.00); 				//If error in applied field is to be neglected, use this instead of true error
                        InternalField.push_back((t->GetV2())[i]); 			//Measured Internal Field
                        InternalField_err.push_back((t->GetV4())[i]); 		//Error in measurement of internal field
        }

	//Plot the applied field vs. internal field on the canvas which was passed in by reference
	c00.cd();
	//This is the maximum shielding, using the field at the edge of the superconductor (extrapolated from field map)
	gr13 = new TGraphErrors(AppliedField_DipoleCorrection_Outer.size(),&(AppliedField_DipoleCorrection_Outer[0]),&(InternalField[0]),&(AppliedField_Noerr[0]),&(InternalField_err[0]));
		gr13->Draw("PL SAME");
                gr13->SetMarkerColor(color);
                gr13->SetLineColor(color);

	c00.Update();
	
	//Add the above plot(s) to the legend passed in by reference
	leg.AddEntry(gr13,title.c_str(),"pl");


	cout << "All done with: " << title << endl;
	return 0;
}

int onetoone_DipoleField(TCanvas &c, TLegend &leg, double inner_mm, double outer_mm)
{
	c.cd();

	cout << "*******************************************************" << endl << "Beginning: 1:1 Reference WITH correction for dipole field" << endl;

	TF1 *onetoone = new TF1("onetoone","pol1",0,1500);
		onetoone->SetParameter(0,0);
		onetoone->SetParameter(1,GetFieldMapFactor(inner_mm));
		onetoone->SetLineColor(kBlue);
		onetoone->SetLineStyle(7);
	onetoone->Draw("SAME");

	TF1 *onetoone_corrected = new TF1("onetoone","pol1",0,1500);
		onetoone_corrected->SetParameter(0,0);
		onetoone_corrected->SetParameter(1,GetFieldMapFactor(outer_mm));
		onetoone_corrected->SetLineColor(kBlue);
		onetoone_corrected->SetLineStyle(7);
	onetoone_corrected->Draw("SAME");

	shade( c, *onetoone, *onetoone_corrected);

	leg.AddEntry(onetoone,"1:1 Reference (Dipole Corrected)","l");	

	cout << "All done with:  1:1 Reference WITH correction for dipole field" << endl;

	return 0;
}


int onetoone_DipoleField(TCanvas &c, TLegend &leg, double outer_mm)
{
	c.cd();
	cout << "*******************************************************" << endl << "Beginning: 1:1 Reference WITH correction for dipole field" << endl;

	TF1 *onetoone_corrected = new TF1("onetoone","pol1",0,1500);
		onetoone_corrected->SetParameter(0,0);
		onetoone_corrected->SetParameter(1,GetFieldMapFactor(outer_mm));
		onetoone_corrected->SetLineColor(kBlack);
		onetoone_corrected->SetLineStyle(7);
	onetoone_corrected->Draw("SAME");

	leg.AddEntry(onetoone_corrected,"1:1 Reference (Dipole Corrected)","l");	
	cout << "All done with:  1:1 Reference WITH correction for dipole field" << endl;

	return 0;
}
