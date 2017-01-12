makePlots_v3()
{
  TTree *t0 = new TTree();
  t0->ReadFile("data_2013_12_08_BvsZCryo1Layer.txt","");
  t0->Print();

  TTree *t1 = new TTree();
  t1->ReadFile("data_2013_12_12_BvsZCryo4Layer.txt","");
  t1->Print();

  TTree *t2 = new TTree();
  t2->ReadFile("data_2014_04_10_BvsZCryo4Layer_Bext5.5mT.txt","");
  t2->Print();

  TTree *t3 = new TTree();
  t3->ReadFile("data_2014_04_10_BvsZCryo4Layer_Bext10.4mT.txt","");
  t3->Print();

  t0->Draw("B-BOffset:z","");
  TGraphErrors* g0 = new TGraphErrors( t0->GetEntries(""), t0->GetV2(), t0->GetV1() );
  for ( int i = 0; i < t0->GetEntries(""); i++ )
    g0->SetPointError( i, 0.0, (0.03 / 100. * t0->GetV1()[i]) );
  g0->SetLineColor(kGray+1);
  g0->SetMarkerColor(kGray+1);
  g0->SetMarkerStyle(20);
  g0->SetMarkerSize(1.1);

  t1->Draw("B-BOffset:z","");
  TGraphErrors* g1 = new TGraphErrors( t1->GetEntries(""), t1->GetV2(), t1->GetV1() );
  for ( int i = 0; i < t1->GetEntries(""); i++ )
    g1->SetPointError( i, 0.0, (0.03 / 100. * t1->GetV1()[i]) );
  g1->SetLineColor(kRed+1);
  g1->SetMarkerColor(kRed+1);
  g1->SetMarkerStyle(20);
  g1->SetMarkerSize(1.1);

  t2->Draw("B-BOffset:z","");
  TGraphErrors* g2 = new TGraphErrors( t2->GetEntries(""), t2->GetV2(), t2->GetV1() );
  for ( int i = 0; i < t2->GetEntries(""); i++ )
    g2->SetPointError( i, 0.0, (0.03 / 100. * t2->GetV1()[i]) );
  g2->SetLineColor(kBlue);
  g2->SetMarkerColor(kBlue);
  g2->SetMarkerStyle(20);
  g2->SetMarkerSize(1.1);

  t3->Draw("B-BOffset:z","");
  TGraphErrors* g3 = new TGraphErrors( t3->GetEntries(""), t3->GetV2(), t3->GetV1() );
  for ( int i = 0; i < t3->GetEntries(""); i++ )
    g3->SetPointError( i, 0.0, (0.03 / 100. * t3->GetV1()[i]) );
  g3->SetLineColor(kGreen+1);
  g3->SetMarkerColor(kGreen+1);
  g3->SetMarkerStyle(20);
  g3->SetMarkerSize(1.1);


  TLine* lLeft = new TLine(-37,0,-37,10);
  //lLeft->SetLineColor(kGreen+1);
  lLeft->SetLineStyle(2);

  TLine* lRight = new TLine(37,0,37,10);
  //lRight->SetLineColor(kGreen+1);
  lRight->SetLineStyle(2);

  TLine* lLeft_zoom = new TLine(-37,0,-37,1.0);
  lLeft_zoom->SetLineStyle(2);

  TLine* lRight_zoom = new TLine(37,0,37,1.0);
  lRight_zoom->SetLineStyle(2);

  TLegend *leg1 = new TLegend(0.7,0.7,0.9,0.9);
  leg1->SetNColumns(1);
  leg1->AddEntry(g1,"Bext  2.0 mT","p");
  leg1->AddEntry(g2,"Bext  5.5 mT","p");
  leg1->AddEntry(g3,"Bext 10.4 mT","p");

  TH1F* h_frame = new TH1F("h_frame","",131,-65.5,65.5);
  h_frame->GetYaxis()->SetTitle("B_{y} [mT]");
  h_frame->GetXaxis()->SetTitle("z - z_{0} [mm]");
  h_frame->GetYaxis()->SetRangeUser(0,10);
  h_frame->GetXaxis()->SetTitleOffset(1.5);

  TH1F* h_frame_zoom = (TH1F*)h_frame->Clone("h_frame_zoom");
  h_frame_zoom->GetYaxis()->SetRangeUser(0,1.0);

  gStyle->SetOptStat(0);

  TCanvas *c0 = new TCanvas();
  //  t1->Draw("B","I>0.09&&z==0");

  TCanvas *c1 = new TCanvas();
  h_frame->Draw();
  lLeft->Draw("same");
  lRight->Draw("same");
  leg1->Draw();
  g1->Draw("LPsame");
  g2->Draw("LPsame");
  g3->Draw("LPsame");
  gPad->RedrawAxis();
  c1->Print("B_vs_z_cryo_varBext.png");

  TCanvas *c1b = new TCanvas();
  h_frame_zoom->Draw();
  lLeft_zoom->Draw("same");
  lRight_zoom->Draw("same");
  leg1->Draw();
  g0->Draw("LPsame");
  g1->Draw("LPsame");
  g2->Draw("LPsame");
  g3->Draw("LPsame");
  gPad->RedrawAxis();
  c1b->Print("B_vs_z_cryo_varBext_zoom.png");

}
