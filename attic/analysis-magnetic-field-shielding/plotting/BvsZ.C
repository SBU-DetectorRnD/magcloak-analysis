#include "./headers/exist_test.h"
#include "./headers/BvsZ.h"


int BvsZ()
{

	//Title of plot
	std::string title = "Field Map";

	//Canvas dimensions
	double ymin, ymax;
	double xmin, xmax;
	ymin = -1.0;
	ymax = 10.0;
	xmin = -650;
	xmax = 650;

	//Create canvas and suppress any information drawn on it
	TCanvas *c = new TCanvas();
	gStyle->SetOptStat(0);

	//Use blank histogram to set titles and dimensions
	TH1F *blank = new TH1F("blank",title.c_str(),10,xmin,xmax);
		blank->GetYaxis()->SetRangeUser(ymin,ymax);
		blank->GetXaxis()->SetTitle("position (mm)");
		blank->GetYaxis()->SetTitle("B (mT)");
		blank->GetXaxis()->SetNdivisions(505);
		blank->GetYaxis()->SetNdivisions(505);
		blank->SetLineColor(0);
	blank->Draw();

	//Creat blank legend to be drawn upon by functions
	leg = new TLegend(0.2,0.9,0.7,0.75);
		leg->SetBorderSize(1);


	//Call function in header file to draw plots
	double z_offset = 1060.000;
	double z_scale = 10.0000;
	double field_offset = 0.000;
	double field_scale = -1.000;
	std::string file_path = "/home/josh/Desktop/";
	std::string file_name = "BvZ_MagOn_Cryo.txt";
	std::string file_name_2 = "BvZ_MagOn_Room.txt";
	BvsZ_plot("Cryo", file_path, file_name, *c, *leg, z_offset, z_scale, field_offset, field_scale);
	BvsZ_plot("Room Temp", file_path, file_name_2, *c, *leg, z_offset, z_scale, field_offset, field_scale);

	//Draw legend after all functions
	leg->Draw();

	return 0;
}
