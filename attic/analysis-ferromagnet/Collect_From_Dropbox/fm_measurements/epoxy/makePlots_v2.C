/** Script for B vs Z scan
 *
 *
 */

TGraphErrors* makeGraph( TTree* tdata, TString drawString )
{
  vector<float> v_x_0, v_y_0, v_ex_0, v_ey_0;
  tdata->Draw(drawString,"");
  for ( int i = 0; i < tdata->GetEntries(""); i++ )
    {
      v_x_0.push_back(tdata->GetV2()[i]);
      v_y_0.push_back(tdata->GetV1()[i]);
      v_ex_0.push_back(tdata->GetV4()[i]);
      v_ey_0.push_back(tdata->GetV3()[i]);//0.03 / 100. * t0->GetV1()[i]);
    }
  TGraphErrors* g0 = new TGraphErrors( tdata->GetEntries(""), &(v_x_0[0]), &(v_y_0[0]), &(v_ex_0[0]), &(v_ey_0[0]) );

  return g0;
};

TString fname_1("room1_results.txt");
TString fname_2("room2_results.txt");
TString fname_3("ln2_1_results.txt");
TString fname_4("ln2_2_results.txt");


/* Helper strings */
TString readString("");
TString drawStringB("ur:Bext:sig_ur:0");

makePlots_v2()
{
  gStyle->SetOptStat(0);

  /* c1 */
  TTree *t1_c1 = new TTree();
  TTree *t2_c1 = new TTree();
  TTree *t3_c1 = new TTree();
  TTree *t4_c1 = new TTree();

  t1_c1->ReadFile( fname_1, readString );
  t2_c1->ReadFile( fname_2, readString );
  t3_c1->ReadFile( fname_3, readString );
  t4_c1->ReadFile( fname_4, readString );

  TGraphErrors* g1_c1 = makeGraph( t1_c1, drawStringB );
  TGraphErrors* g2_c1 = makeGraph( t2_c1, drawStringB );
  TGraphErrors* g3_c1 = makeGraph( t3_c1, drawStringB );
  TGraphErrors* g4_c1 = makeGraph( t4_c1, drawStringB );

  TLegend *leg_c1 = new TLegend(0.65,0.75,0.9,0.85);
  leg_c1->SetNColumns(1);
  leg_c1->AddEntry(g1_c1,"T = room","p");
  leg_c1->AddEntry(g3_c1,"T = lN2","p");

  g1_c1->SetLineColor(kRed);
  g1_c1->SetMarkerColor(kRed);
  g1_c1->SetMarkerStyle(20);
  g1_c1->SetMarkerSize(1.1);

  g2_c1->SetLineColor(kRed);
  g2_c1->SetMarkerColor(kRed);
  g2_c1->SetMarkerStyle(20);
  g2_c1->SetMarkerSize(1.1);

  g3_c1->SetLineColor(kBlue);
  g3_c1->SetMarkerColor(kBlue);
  g3_c1->SetMarkerStyle(20);
  g3_c1->SetMarkerSize(1.1);

  g4_c1->SetLineColor(kBlue);
  g4_c1->SetMarkerColor(kBlue);
  g4_c1->SetMarkerStyle(20);
  g4_c1->SetMarkerSize(1.1);

  TH1F* h_frame = new TH1F("h_frame","",100,0,36);
  h_frame->GetYaxis()->SetTitle("#mu_{r}");
  h_frame->GetXaxis()->SetTitle("B_{ext} [mT]");
  h_frame->GetYaxis()->SetRangeUser(2,6);
  h_frame->GetXaxis()->SetTitleOffset(1.5);
  h_frame->GetYaxis()->SetNdivisions(505);

  TCanvas *c1 = new TCanvas();
  h_frame->Draw("");
  leg_c1->Draw();
  g1_c1->Draw("LPsame");
  //  g2_c1->Draw("LPsame");
  g3_c1->Draw("LPsame");
  //  g4_c1->Draw("LPsame");
  gPad->RedrawAxis();
  c1->Print("FmEpoxy_MuRvsBext_v1.eps");
  c1->Print("FmEpoxy_MuRvsBext_v1.png");

}
