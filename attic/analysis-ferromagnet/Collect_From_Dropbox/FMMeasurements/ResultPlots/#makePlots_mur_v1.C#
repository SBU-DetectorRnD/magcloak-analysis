makePlots_mur_v1(){

  gStyle->SetOptStat(0);

  /*  302 stainless steel sheet */
  TTree *t0_err = new TTree();
  TTree *t0_sys = new TTree();
  TTree *t0_sys_p1 = new TTree();
  TTree *t0_sys_p2 = new TTree();

  t0_err->ReadFile("steel302_point_err_20140716.txt","Bext:mu:mu_err");
  t0_sys->ReadFile("steel302_syst_err_20140716.txt","Bext:mu:mu_err");
  t0_sys_p1->ReadFile("steel302_syst_err_20140716_p1.txt","Bext:mu:mu_err");
  t0_sys_p2->ReadFile("steel302_syst_err_20140716_p2.txt","Bext:mu:mu_err");

  t0_err->Draw("mu:Bext:mu_err");
  TGraphErrors* g0_err = new TGraphErrors( t0_err->GetEntries(), t0_err->GetV2(), t0_err->GetV1(), 0, t0_err->GetV3() );

  t0_sys->Draw("mu:Bext:mu_err");
  TGraphErrors* g0_sys = new TGraphErrors( t0_sys->GetEntries(), t0_sys->GetV2(), t0_sys->GetV1(), 0, t0_sys->GetV3() );
  g0_sys->SetFillColor(kGray);

  t0_sys_p1->Draw("mu:Bext:mu_err");
  TGraphErrors* g0_sys_p1 = new TGraphErrors( t0_sys_p1->GetEntries(), t0_sys_p1->GetV2(), t0_sys_p1->GetV1(), 0, t0_sys_p1->GetV3() );
  g0_sys_p1->SetFillColor(kGray);

  t0_sys_p2->Draw("mu:Bext:mu_err");
  TGraphErrors* g0_sys_p2 = new TGraphErrors( t0_sys_p2->GetEntries(), t0_sys_p2->GetV2(), t0_sys_p2->GetV1(), 0, t0_sys_p2->GetV3() );
  g0_sys_p2->SetFillColor(kGray);


  /* 5 layers 430 Stainless Steel Powder */
  TTree *t1_err = new TTree();
  TTree *t1_sys = new TTree();
  TTree *t1_sys_p1 = new TTree();
  TTree *t1_sys_p2 = new TTree();

  t1_err->ReadFile("steel_kapton_point_err_20140716.txt","Bext:mu:mu_err");
  t1_sys->ReadFile("steel_kapton_syst_err_20140716.txt","Bext:mu:mu_err");
  t1_sys_p1->ReadFile("steel_kapton_syst_err_20140716_p1.txt","Bext:mu:mu_err");
  t1_sys_p2->ReadFile("steel_kapton_syst_err_20140716_p2.txt","Bext:mu:mu_err");

  t1_err->Draw("mu:Bext:mu_err");
  TGraphErrors* g1_err = new TGraphErrors( t1_err->GetEntries(), t1_err->GetV2(), t1_err->GetV1(), 0, t1_err->GetV3() );
  g1_err->SetMarkerColor(kBlue);

  t1_sys->Draw("mu:Bext:mu_err");
  TGraphErrors* g1_sys = new TGraphErrors( t1_sys->GetEntries(), t1_sys->GetV2(), t1_sys->GetV1(), 0, t1_sys->GetV3() );
  g1_sys->SetFillColor(kGray);

  t1_sys_p1->Draw("mu:Bext:mu_err");
  TGraphErrors* g1_sys_p1 = new TGraphErrors( t1_sys_p1->GetEntries(), t1_sys_p1->GetV2(), t1_sys_p1->GetV1(), 0, t1_sys_p1->GetV3() );
  g1_sys_p1->SetFillColor(kGray);

  t1_sys_p2->Draw("mu:Bext:mu_err");
  TGraphErrors* g1_sys_p2 = new TGraphErrors( t1_sys_p2->GetEntries(), t1_sys_p2->GetV2(), t1_sys_p2->GetV1(), 0, t1_sys_p2->GetV3() );
  g1_sys_p2->SetFillColor(kGray);



  /* Do graphing */
  TH1F* hframe = new TH1F("hframe","",20,0.5,20.5);
  hframe->GetYaxis()->SetRangeUser(1,11);
  hframe->GetXaxis()->SetTitle("B_{0} [mT]");
  hframe->GetYaxis()->SetTitle("#mu_{r}");

  TCanvas *c1 = new TCanvas();
  hframe->Draw("rad");

  g0_sys_p1->Draw("LE3same");
  g0_sys_p2->Draw("LE3same");

  g1_sys_p1->Draw("LE3same");
  g1_sys_p2->Draw("LE3same");

  g0_err->Draw("LPsame");
  g1_err->Draw("LPsame");

  gPad->RedrawAxis();

  c1->Print("FMMeasurements_results_mur_v1.eps");

  TCanvas *c2 = new TCanvas();
  hframe->Draw("rad");

  g0_sys_p1->Draw("LE3same");
  g0_sys_p2->Draw("LE3same");

  g0_err->Draw("LPsame");

  gPad->RedrawAxis();

  c2->Print("FMMeasurements_results_mur_302only_v1.eps");


  TCanvas *ctest = new TCanvas();
  t1_err->Draw("mu*Bext:Bext","","");
  t0_err->Draw("mu*Bext:Bext","","same");


}
