#include <fstream>
#include <string>
#include <math.h>
#include <algorithm>
#include "TGraph.h"

int new_plotter()
{

	TTree *t1 = new TTree();
	t1->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_150113.txt","time:field:voltage");

	TCanvas *c1 = new TCanvas();
	int n = t1->Draw("voltage:field","time>0","goff");
	TGraph *g = new TGraph(n,t1->GetV1(),t1->GetV2());
		g->SetMarkerColor(kRed);
	g->Draw("ap");
	
	TTree *t2 = new TTree();
	t2->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160608_151527.txt","time2:field2:voltage2");

	int n = t2->Draw("voltage2:field2","time2>0","goff");
	TGraph *g2 = new TGraph(n,t2->GetV1(),t2->GetV2());
		g2->SetMarkerColor(kGreen);
	g2->Draw("SAME p");

	return 0;
}
