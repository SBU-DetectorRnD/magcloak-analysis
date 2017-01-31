#include "./headers/exist_test.h"
#include "./headers/BvsZ.h"


int BvsZ()
{

	//Title of plot
	std::string title = "Field Map (33.1 mm from Cloak Center)";

	//Canvas dimensions
	double ymin, ymax;
	double xmin, xmax;
	ymin = 30.0;
	ymax = 60.0;
	xmin = -160;
	xmax = 160;

	//Create canvas and suppress any information drawn on it
	TCanvas *c = new TCanvas();
	//TCanvas *c = new TCanvas("c", title.c_str(), 750, 750); 
	gStyle->SetOptStat(0);
	c->SetGridx(1);
	c->SetGridy(1); 
//	c->SetFixedAspectRatio(); 

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
	double z_offset = -136.000;
	double z_scale = -1.0000;
	double field_offset = 0.000;
	double field_scale = -1.000;
	std::string file_path = "/mnt/c/Users/jlabo/Dropbox/Stony Brook Research Team Folder/LabVIEW/DATA_Gaussmeter/";

	//1-26-2017
	std::string file_name = "DataFile_170126_180157.txt";	//FM only
	std::string file_name_2 = "DataFile_170126_172846.txt";	//Reference
	std::string file_name_3 = "DataFile_170126_181436.txt";	//FM+SC
	std::string file_name_4 = "DataFile_170126_182733.txt";	//FM+SC round 2
	std::string file_name_5 = "DataFile_170126_183703.txt";	//FM only round 2
	std::string file_name_6 = "DataFile_170126_193411.txt";	//Full Cloak at Cryo (side 2)
	std::string file_name_7 = "DataFile_170126_195703.txt";	//SC Only at Cryo
	std::string file_name_8 = "DataFile_170126_200358.txt";	//SC Only at Cryo 2

	//1-27-2017
	std::string file_name_9 = "DataFile_170127_125124.txt";	//0.618 RT
	std::string file_name_10 = "DataFile_170127_130008.txt";	//Cloak (0.618) at RT
	std::string file_name_11 = "DataFile_170127_132409.txt";	//FM (0.554) Cryo
	std::string file_name_12 = "DataFile_170127_133338.txt";	//FM (0.554) Cryo 2
	std::string file_name_13 = "DataFile_170127_134912.txt";	//FM (0.618) Cryo 
	std::string file_name_14 = "DataFile_170127_140919.txt";	//0.554 fm + sc (side 1) 
	std::string file_name_15 = "DataFile_170127_142831.txt";	//0.618 fm + sc (side 2) 

	BvsZ_plot_Inorm("RT: Reference Field", file_path, file_name_2, *c, *leg, z_offset, z_scale, field_offset, field_scale);
//	BvsZ_plot_Inorm("RT: Ferromagnet (0.554) Only", file_path, file_name_5, *c, *leg, z_offset, z_scale, field_offset, field_scale);
	BvsZ_plot_Inorm("Cryo: Ferromagnet (0.554) Only", file_path, file_name_12, *c, *leg, z_offset, z_scale, field_offset, field_scale);
//	BvsZ_plot_Inorm("RT: Ferromagnet (0.554) + 45-layer SC Side 2", file_path, file_name_4, *c, *leg, z_offset, z_scale, field_offset, field_scale);
//	BvsZ_plot_Inorm("Cryo: 45-layer SC", file_path, file_name_8, *c, *leg, z_offset, z_scale, field_offset, field_scale);
	BvsZ_plot_Inorm("Cryo: Ferromagnet (0.554) + 45-layer SC Side 2", file_path, file_name_6, *c, *leg, z_offset, z_scale, field_offset, field_scale);

//	BvsZ_plot_Inorm("RT: Ferromagnet (0.618) Only", file_path, file_name_9, *c, *leg, z_offset, z_scale, field_offset, field_scale);
//	BvsZ_plot_Inorm("Cryo: Ferromagnet (0.618) Only", file_path, file_name_13, *c, *leg, z_offset, z_scale, field_offset, field_scale);
//	BvsZ_plot_Inorm("RT: Ferromagnet (0.618) + 45-layer SC", file_path, file_name_10, *c, *leg, z_offset, z_scale, field_offset, field_scale);
	BvsZ_plot_Inorm("Cryo: Ferromagnet (0.554) + 45-layer SC Side 1", file_path, file_name_14, *c, *leg, z_offset, z_scale, field_offset, field_scale);
//	BvsZ_plot_Inorm("Cryo: Ferromagnet (0.618) + 45-layer SC Side 2", file_path, file_name_15, *c, *leg, z_offset, z_scale, field_offset, field_scale);

	//Draw legend after all functions
	leg->Draw();

	return 0;
}
