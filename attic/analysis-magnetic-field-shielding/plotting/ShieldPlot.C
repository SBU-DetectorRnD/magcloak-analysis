#include "headers/ShieldPlot.h"
#include "headers/DipoleField_ShieldPlot.h"
#include "headers/exist_test.h"

int ShieldPlot()
{
	int color = 2;				//Integer corresponding to the color of the plot which is created.
	gStyle->SetOptStat(0);			//Prevents fit information from bring printed on the canvas.
	std::string title = "YBCO: 45 Layer 4.5\" Solder Test Comparison";			//Title of the canvas which will host all of the plots
	bool plot_reference_line = true;	//If true will plot a 1:1 reference line on top of the plots.
	bool plot_rapheal = false; 		//If true will plot rapheals measurement on top of new measurement
	bool draw_legend = true;		//If true will draw legend on plot
	bool print_jpg = false;			//Prints canvas automatically to Plot_[title].jpg
	bool fixed_aspect_ratio = false;		//If true fixes output canvas aspect ratio to 1:1

	double ymin, ymax, xmax, xmin;
	ymin = -0.75;	//Defines the range of the canvas which will be printed in x and y
	ymax = 50;
	xmin = 0;
	xmax = 75;

	//Create canvas to draw all of the plots on. This will be passed by reference into each function below.
	TCanvas *c00 = new TCanvas("c00",title.c_str(),750,750);	//Makes canvas large enough for png printing.
	c00->cd();
	//Use blank histogram to set the parameters of the canvas
        TH1F *blank = new TH1F("blank",title.c_str(),10,xmin,xmax);
                blank->GetYaxis()->SetRangeUser(ymin,ymax);
                blank->GetXaxis()->SetTitle("B_{0} (mT)");
                //blank->GetYaxis()->SetTitle("B_{0} - B_{i} (mT)");	//Shielding Factor y-axis
                blank->GetYaxis()->SetTitle("B_{i} (mT)");		//Internal vs. External Field y-axis
                blank->GetXaxis()->SetNdivisions(505);
                blank->GetYaxis()->SetNdivisions(505);
                blank->SetLineColor(0);
        blank->Draw();

	//Further canvas settings (axis grid, aspect ratio, etc.)
	c00->SetGridx(1);
	c00->SetGridy(1);
	if(fixed_aspect_ratio) c00->SetFixedAspectRatio();

	//Legend for the plots. This is passed by reference into each function called below which will add its own entry to it.
        leg = new TLegend(0.2,0.9,0.7,0.75);
                leg->SetBorderSize(1);

	//If bool above is set to true this will plot a 1:1 shielding reference line on top of the measurements
	if(plot_reference_line)
	{
		TF1 *onetoone = new TF1("onetoone","pol1",0,1500);
			onetoone->SetParameter(0,0);
			onetoone->SetParameter(1,1);
			onetoone->SetLineColor(kBlack);
			onetoone->SetLineStyle(7);
		onetoone->Draw("SAME");
		leg->AddEntry(onetoone,"1:1 Reference","l");
	}


	//If the bool above is true this will plot rapheals measurements on top of the current plot
	if(plot_rapheal)
	{
		std::string title_rapheal = "2015 1-Layer (Helmholtz)";
		cout << "*******************************************************" << endl << "Beginning: " << title_rapheal << endl;
		//Plots rapheals 1-layer measurement from his thesis
		TTree *t = new TTree();
		t->ReadFile("./DataFiles/1layer_wide_sc_fit_results.txt","Bo:w:Bi:Bi_err:t:y:u:i:o:p");
		TCanvas *ctemp = new TCanvas();
//		t->Draw("Bo-Bi:Bo:Bi_err","","pl");
		t->Draw("Bi:Bo:Bi_err","","pl");
		c00->cd();
		TGraphErrors *r = new TGraphErrors(t->GetEntries(),t->GetV2(),t->GetV1());
		for ( int i = 0; i < t->GetEntries(); i++ )
			r->SetPointError(i,0, t->GetV3()[i]);
		r->Draw("Pl same");
		leg->AddEntry(r,title_rapheal.c_str(),"pl");
		ctemp->Close();
		cout << "All done with: " << title_rapheal.c_str() << endl;
	}

/*
	There are a number of functions located in the header file for this macro. Each one takes the following parameters as input:
		Calibration File 	---	Room temperature calibration measuremnt used to convert voltage -> applied field. This is only required for old measurements before we implemented the datacut algorithm.
		Measurement File	---	A file containing the data for the measuremnt. For the Helium measurements this should be in the form "current field" and for all other (non-summary) files in the form "time voltage field". For the summary file the format is "Applied_Field Error_Applied_Field Internal_Field Error_Internal_Field"
		Title			---	A string which will be the entries label in the legend
		TCanvas			---	The TCanvas (defailt *c00) which is passed by reference for the function to draw on
		TLegend			---	The TLegend (defailt *leg) which is passed by reference for the function to draw on
		color			---	An integer which specifies the color of the markers for that measurement

	For the python calibration, the path of the directory where the python macros are stored is also necessary, as well as which setup the measurement was taken in (for the calibration factor) and whether or not to invert the measurement.

*/

/*
	onetoone_DipoleField(*c00, *leg, 0.00, 24.50);

	shield_plot_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160720_172022.txt",								//File name for measurement file
		"2016 10-Layer Solder (Dipole Center)",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Dipole",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		1962);											//Line of file to stop python. 0 if full file.

	color++;

	DipoleFieldShieldPlot(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160720_172022.txt",								//File name for measurement file
		"2016 10-Layer Solder (Dipole Corrected)",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Dipole",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		1962,											//Line of file to stop python. 0 if full file.
		(24.50 - 1.04),										//Inner Diameter of Cylinder in mm
		24.50);											//Outer Diameter of Cylinder in mm

	color++;

	shield_plot_python(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160722_184220.txt",								//File name for measurement file
		"2016 45-Layer Solder (Dipole Center)",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"DipoleNew",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		0);											//Line of file to stop python. 0 if full file.

	color++;
	color++;

	DipoleFieldShieldPlot(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160722_184220.txt",								//File name for measurement file
		"2016 45-Layer Solder (Dipole Corrected)",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"DipoleNew",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		0,											//Line of file to stop python. 0 if full file.
		13.02,											//Inner Diameter of Cylinder in mm
		24.50);											//Outer Diameter of Cylinder in mm

color++;
*/
	shield_plot_python_existing(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160915_132943.txt",								//File name for measurement file
		"2016 1-layer overlap (new) --- Dipole",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"DipoleNew",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		0);											//Line of file to stop python. 0 if full file.
	color++;

	shield_plot_python_existing(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160915_151651.txt",								//File name for measurement file
		"2016 2-layer overlap --- Dipole",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"DipoleNew",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		0);											//Line of file to stop python. 0 if full file.
	color++;

	shield_plot_python_existing(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160915_113128.txt",								//File name for measurement file
		"2016 1-layer overlap (old)",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"DipoleNew",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		0);											//Line of file to stop python. 0 if full file.
	color++;


/*
	DipoleFieldShieldPlot_existing(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160915_132943.txt",								//File name for measurement file
		"2016 1-layer overlap [Dipole Corrected]",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Dipole",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		0,											//Line of file to stop python. 0 if full file.
		(24.50 - 0.0508*2),										//Inner Diameter of Cylinder in mm
		24.50);											//Outer Diameter of Cylinder in mm
	color++;
	color++;

	DipoleFieldShieldPlot_existing(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160915_151651.txt",								//File name for measurement file
		"2016 2-layer overlap [Dipole Corrected]",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"Dipole",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		0,											//Line of file to stop python. 0 if full file.
		(24.50 - 0.0508*2),										//Inner Diameter of Cylinder in mm
		24.50);											//Outer Diameter of Cylinder in mm
	color++;


	shield_plot_python_existing(
		"/home/josh/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/",		//File path of the datafile
		"./SummaryFiles/",									//File path of the python output, should be a folder SummaryFiles 
													//	in the same directory as the python script.
		"DataFile_160915_173531.txt",								//File name for measurement file
		"2016 2-layer overlap (new) --- Helmholtz",								//File name
		*c00,											//canvas to draw on
		*leg,											//legend to draw on
		color,											//color of plot marker / lines
		"DipoleNew",										//1 of solenoid calibration, 2 if helmholtz calibration
		"True",											//Negate measurements of the field?
		0);											//Line of file to stop python. 0 if full file.
	color++;
*/

	if(draw_legend) leg->Draw();
	if(print_jpg) 
	{
		c00->Print(("./plots/Plot_"+title+".png").c_str());
		c00->Print(("./plots/Plot_"+title+".eps").c_str());
		cout << "Image Created with title: Plot_" << title << ".png" << endl;
	}

	return 0;
}
