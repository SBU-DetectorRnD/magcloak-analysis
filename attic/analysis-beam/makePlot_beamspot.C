int makePlot_beamspot(
		      const TString infile = "BeamSpotData/beamspot_data.txt"
		      )
{
  gStyle->SetOptStat(0);

  gStyle->SetCanvasDefH(600);
  gStyle->SetCanvasDefW(600);
  gStyle->SetPadTopMargin(0.10);
  gStyle->SetErrorX(0.0);
  gStyle->SetMarkerStyle(7);
  gStyle->SetMarkerColor(kRed);
  gStyle->SetMarkerSize(0.7);
  gStyle->SetEndErrorSize(6);

  TTree *BeamTree = new TTree();
  BeamTree->ReadFile(infile, "ID/F:I_x:I_y:meanx:stdx:meany:stdy");
  BeamTree->Print();

//  TCanvas *c1 = new TCanvas();
//  TH1 *h1 = c1->DrawFrame(-1.0, -7.0, 1.0, 8.0);
//  h1->SetTitle("");
//  h1->GetXaxis()->SetTitle("#Delta B (mT)");
//  h1->GetXaxis()->SetTitleOffset(1.5);
//  h1->GetYaxis()->SetTitle("#Delta x (Pixels)");
//  h1->GetYaxis()->SetTitleOffset(1.5);
//  BeamTree->Draw("meanx:I_x", "", "profs,same,e1");
  BeamTree->Draw("meanx:I_x", "", "profs,e1");
  //  c1->Print("BNL_VDG_BeamPlot.png");
  //  c1->Print("BNL_VDG_BeamPlot.eps");

  htemp->Fit("pol1", "", "", -1.25, 1.0);



  //
  TCanvas *c2 = new TCanvas();
  
  BeamTree->Draw("meany:I_x", "", "profs,e1");
  htemp->Fit("pol1", "", "", -1.25, 1.0);


  return 0;
}
