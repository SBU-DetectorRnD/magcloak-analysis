#ifndef GET_CALIBRATION
#define GET_CALIBRATION

#include "exist_test.h"

double get_calibration(std::string file)
{

	TTree *t = new TTree();
	TCanvas *c = new TCanvas();
	gStyle->SetOptFit();
	if(exist_test(file))
	{
		t->ReadFile(file.c_str(),"time:voltage:field");
	}
	else
	{
		cerr << "ERROR: File not found" << endl;
		c->Close();
		return 0;
	}
	t->Draw("field:voltage");
	TGraph* g1 = new TGraph( t->GetEntries(), t->GetV2(), t->GetV1() );
	TF1 *fit1 = new TF1("fit1","pol1");
	g1->Fit("fit1","");
	c->Update();

	double slope = fit1->GetParameter(1);

	return slope;
}


double get_calibration_quiet(std::string file)
{

	TTree *t = new TTree();
	TCanvas *c = new TCanvas();
	gStyle->SetOptFit();
	if(exist_test(file))
	{
		t->ReadFile(file.c_str(),"time:voltage:field");
	}
	else
	{
		cerr << "ERROR: File not found" << endl;
		c->Close();
		return 0;
	}
	t->Draw("field:voltage");
	TGraph* g1 = new TGraph( t->GetEntries(), t->GetV2(), t->GetV1() );
	TF1 *fit1 = new TF1("fit1","pol1");
	g1->Fit("fit1","Q");
	c->Update();

	double slope = fit1->GetParameter(1);
	c->Close();

	return slope;
}

#endif
