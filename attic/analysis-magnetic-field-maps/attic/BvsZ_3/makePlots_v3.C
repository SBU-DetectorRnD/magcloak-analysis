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

/* List of all good B vs Z scans: */
TString fname_roomT_2mT_1("data/DataFile_141118_122119.txt"); // room temperature, 2 mT

TString fname_h2_l1_2mT_1("data/DataFile_141120_154144.txt"); // helix wrapping, version 2 (cyl B = 'adjacent'), 1 layer, 2 mT

TString fname_h2_l2_2mT_1("data/DataFile_141120_195415.txt"); // helix wrapping, version 2 (cyl B = 'adjacent'), 2 layer, 2 mT
TString fname_h2_l2_2mT_2("data/DataFile_141217_135845.txt"); // helix wrapping, version 2 (cyl B = 'adjacent'), 2 layer, 2 mT

TString fname_h2_l4_2mT_1("data/DataFile_141216_162327.txt"); // helix wrapping, adjacent, 4 layer, 2 mT

TString fname_h2_l5_2mT_1("data/DataFile_141217_172728.txt"); // helix wrapping, adjacent, 5 layer, 2 mT

/* Helper strings */
TString readString("z/F:B:dB:I:dI");
TString drawStringB("B:(z-151):0:0");
TString drawStringI("I:(z-151):0:0");

makePlots_v3()
{
  gStyle->SetOptStat(0);

  TLine* lLeft = new TLine(-37,0,-37,2.2);
  //lLeft->SetLineColor(kGreen+1);
  lLeft->SetLineStyle(2);

  TLine* lRight = new TLine(37,0,37,2.2);
  //lRight->SetLineColor(kGreen+1);
  lRight->SetLineStyle(2);


  /* c1 */
  TTree *t0_c1 = new TTree();
  TTree *t1_c1 = new TTree();
  TTree *t2_c1 = new TTree();
  TTree *t3_c1 = new TTree();
  TTree *t4_c1 = new TTree();

  t0_c1->ReadFile( fname_roomT_2mT_1, readString );
  t1_c1->ReadFile( fname_h2_l1_2mT_1, readString );
  t2_c1->ReadFile( fname_h2_l2_2mT_1, readString );
  t3_c1->ReadFile( fname_h2_l4_2mT_1, readString );
  t4_c1->ReadFile( fname_h2_l5_2mT_1, readString );

  TGraphErrors* g0_c1 = makeGraph( t0_c1, drawStringB );
  TGraphErrors* g1_c1 = makeGraph( t1_c1, drawStringB );
  TGraphErrors* g2_c1 = makeGraph( t2_c1, drawStringB );
  TGraphErrors* g3_c1 = makeGraph( t3_c1, drawStringB );
  TGraphErrors* g4_c1 = makeGraph( t4_c1, drawStringB );

  TLegend *leg_c1 = new TLegend(0.4,0.6,0.8,0.8);
  leg_c1->SetNColumns(1);
  leg_c1->AddEntry(g0_c1,"No SC","p");
  leg_c1->AddEntry(g1_c1,"r1 1 layer","p");
  leg_c1->AddEntry(g2_c1,"r1 2 layers","p");
  leg_c1->AddEntry(g3_c1,"r1 4 layers","p");
  leg_c1->AddEntry(g4_c1,"r2 5 layers","p");

  g0_c1->SetLineColor(kRed);
  g0_c1->SetMarkerColor(kRed);
  g0_c1->SetMarkerStyle(20);
  g0_c1->SetMarkerSize(1.1);

  g1_c1->SetLineColor(kBlue);
  g1_c1->SetMarkerColor(kBlue);
  g1_c1->SetMarkerStyle(21);
  g1_c1->SetMarkerSize(1.1);

  g2_c1->SetLineColor(kGreen+1);
  g2_c1->SetMarkerColor(kGreen+1);
  g2_c1->SetMarkerStyle(21);
  g2_c1->SetMarkerSize(1.2);

  g3_c1->SetLineColor(kMagenta+2);
  g3_c1->SetMarkerColor(kMagenta+2);
  g3_c1->SetMarkerStyle(21);
  g3_c1->SetMarkerSize(1.2);

  g4_c1->SetLineColor(kBlack);
  g4_c1->SetMarkerColor(kBlack);
  g4_c1->SetMarkerStyle(21);
  g4_c1->SetMarkerSize(1.2);


  TGraphErrors* g0I_c1 = makeGraph( t0_c1, drawStringI );
  TGraphErrors* g1I_c1 = makeGraph( t1_c1, drawStringI );
  TGraphErrors* g2I_c1 = makeGraph( t2_c1, drawStringI );
  TGraphErrors* g3I_c1 = makeGraph( t3_c1, drawStringI );
  TGraphErrors* g4I_c1 = makeGraph( t4_c1, drawStringI );

  g0I_c1->SetLineColor(  g0_c1->GetLineColor());
  g0I_c1->SetMarkerColor(g0_c1->GetMarkerColor());
  g0I_c1->SetMarkerStyle(g0_c1->GetMarkerStyle());
  g0I_c1->SetMarkerSize( g0_c1->GetMarkerSize());

  g1I_c1->SetLineColor(  g1_c1->GetLineColor());
  g1I_c1->SetMarkerColor(g1_c1->GetMarkerColor());
  g1I_c1->SetMarkerStyle(g1_c1->GetMarkerStyle());
  g1I_c1->SetMarkerSize( g1_c1->GetMarkerSize());

  g2I_c1->SetLineColor(  g2_c1->GetLineColor());
  g2I_c1->SetMarkerColor(g2_c1->GetMarkerColor());
  g2I_c1->SetMarkerStyle(g2_c1->GetMarkerStyle());
  g2I_c1->SetMarkerSize( g2_c1->GetMarkerSize());

  g3I_c1->SetLineColor(  g3_c1->GetLineColor());
  g3I_c1->SetMarkerColor(g3_c1->GetMarkerColor());
  g3I_c1->SetMarkerStyle(g3_c1->GetMarkerStyle());
  g3I_c1->SetMarkerSize( g3_c1->GetMarkerSize());

  g4I_c1->SetLineColor(  g4_c1->GetLineColor());
  g4I_c1->SetMarkerColor(g4_c1->GetMarkerColor());
  g4I_c1->SetMarkerStyle(g4_c1->GetMarkerStyle());
  g4I_c1->SetMarkerSize( g4_c1->GetMarkerSize());


  TH1F* h_frame = new TH1F("h_frame","",131,-85.5,85.5);
  h_frame->GetYaxis()->SetTitle("B_{y} [mT]");
  h_frame->GetXaxis()->SetTitle("z - z_{c} [mm]");
  h_frame->GetYaxis()->SetRangeUser(0,2.2);
  h_frame->GetXaxis()->SetTitleOffset(1.5);

  TH1F* h_frameI = new TH1F("h_frameI","",131,-85.5,85.5);
  h_frameI->GetYaxis()->SetTitle("I [A]");
  h_frameI->GetXaxis()->SetTitle("z - z_{c} [mm]");
  h_frameI->GetYaxis()->SetRangeUser(2.565,2.585);
  h_frameI->GetXaxis()->SetTitleOffset(1.5);

  TCanvas *c1 = new TCanvas();
  h_frame->Draw("");
//  lLeft->Draw("same");
//  lRight->Draw("same");
  leg_c1->Draw();
  g0_c1->Draw("LPsame");
  g1_c1->Draw("LPsame");
  g2_c1->Draw("LPsame");
  g3_c1->Draw("LPsame");
  g4_c1->Draw("LPsame");
  gPad->RedrawAxis();
  c1->Print("Plots/BvsZ_v2_warm_cryo_v3.eps");
  c1->Print("Plots/BvsZ_v2_warm_cryo_v3.png");


  TCanvas *c1I = new TCanvas();
  h_frameI->Draw("");
  leg_c1->Draw();
  g0I_c1->Draw("LPsame");
  g1I_c1->Draw("LPsame");
  g2I_c1->Draw("LPsame");
  g3I_c1->Draw("LPsame");
  g4I_c1->Draw("LPsame");
  gPad->RedrawAxis();
//  c1I->Print("../Plots/Public/BvsZ_v2_warm_cryo.eps");
//  c1I->Print("../Plots/Public/BvsZ_v2_warm_cryo.png");


}
