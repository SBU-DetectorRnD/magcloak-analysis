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

TString fname_v1_l1_2mT_1("data/DataFile_141118_135457.txt"); // vertical wrapping 'new = no overlap', 1 layer, 2 mT
TString fname_v1_l2_2mT_1("data/DataFile_141216_152517.txt"); // vertical wrapping 'new = no overlap', 2 layer, 2 mT

TString fname_h1_l1_2mT_1("data/DataFile_141118_173948.txt"); // helix wrapping, version 1 (cyl A), 1 layer, 2 mT
TString fname_h1_l2_2mT_1("data/DataFile_141120_175939.txt"); // helix wrapping, version 1 (cyl A), 1 layer, 2 mT

TString fname_h2_l1_2mT_1("data/DataFile_141120_154144.txt"); // helix wrapping, version 2 (cyl B = 'adjacent'), 1 layer, 2 mT
TString fname_h2_l2_2mT_1("data/DataFile_141120_195415.txt"); // helix wrapping, version 2 (cyl B = 'adjacent'), 1 layer, 2 mT

TString fname_h2_l4_2mT_1("data/DataFile_141216_162327.txt"); // helix wrapping, adjacent, 4 layer, 2 mT

TString fname_h2_l4_7mT_1("data/DataFile_141216_183116.txt"); // helix wrapping, adjacent, 4 layer, 7 mT


// c1: v1 h1a v2 h2a
//
// c2: h2a h2b
// 
// c3: h1b h2b h4b
//
// c4: h4_2mT h4_7mT (measure 5 layer instead?)

/* Helper strings */
TString readString("z/F:B:dB:I:dI");
TString drawStringB("B:(z-151):0:0");
TString drawStringI("I:(z-151):0:0");

makePlots()
{

  /* c1 */
  

  TTree *t0 = new TTree();
  TTree *t1 = new TTree();
  TTree *t2 = new TTree();
  TTree *t3 = new TTree();
  TTree *t4 = new TTree();
  TTree *t5 = new TTree();
  TTree *t6 = new TTree();

  t0->ReadFile( fname_roomT_2mT_1, readString );
  t1->ReadFile( fname_v1_l1_2mT_1, readString );
  t2->ReadFile( fname_v1_l2_2mT_1, readString );
  t3->ReadFile( fname_h1_l1_2mT_1, readString );
  t4->ReadFile( fname_h1_l2_2mT_1, readString );
  t5->ReadFile( fname_h2_l1_2mT_1, readString );
  t6->ReadFile( fname_h2_l2_2mT_1, readString );

  TGraphErrors* g0 = makeGraph( t0, drawStringB );
  TGraphErrors* g1 = makeGraph( t1, drawStringB );
  TGraphErrors* g2 = makeGraph( t2, drawStringB );
  TGraphErrors* g3 = makeGraph( t3, drawStringB );
  TGraphErrors* g4 = makeGraph( t4, drawStringB );
  TGraphErrors* g5 = makeGraph( t5, drawStringB );
  TGraphErrors* g6 = makeGraph( t6, drawStringB );

  g0->SetLineColor(kRed);
  g0->SetMarkerColor(kRed);
  g0->SetMarkerStyle(20);
  g0->SetMarkerSize(1.1);

  g1->SetLineColor(kMagenta+2);
  g1->SetMarkerColor(kMagenta+2);
  g1->SetMarkerStyle(21);
  g1->SetMarkerSize(1.1);

  g2->SetLineColor(kBlue);
  g2->SetMarkerColor(kBlue);
  g2->SetMarkerStyle(21);
  g2->SetMarkerSize(1.2);

  g3->SetLineColor(kGreen+1);
  g3->SetMarkerColor(kGreen+1);
  g3->SetMarkerStyle(21);
  g3->SetMarkerSize(1.2);

  g4->SetLineColor(kMagenta+2);
  g4->SetMarkerColor(kMagenta+2);
  g4->SetMarkerStyle(22);
  g4->SetMarkerSize(1.2);

  g5->SetLineColor(kBlue);
  g5->SetMarkerColor(kBlue);
  g5->SetMarkerStyle(22);
  g5->SetMarkerSize(1.2);

  g6->SetLineColor(kGreen+1);
  g6->SetMarkerColor(kGreen+1);
  g6->SetMarkerStyle(22);
  g6->SetMarkerSize(1.2);


  TGraphErrors* g0I = makeGraph( t0, drawStringI );
  TGraphErrors* g1I = makeGraph( t1, drawStringI );
  TGraphErrors* g2I = makeGraph( t2, drawStringI );
  TGraphErrors* g3I = makeGraph( t3, drawStringI );
  TGraphErrors* g4I = makeGraph( t4, drawStringI );
  TGraphErrors* g5I = makeGraph( t5, drawStringI );
  TGraphErrors* g6I = makeGraph( t6, drawStringI );

  g0I->SetLineColor(  g0->GetLineColor());
  g0I->SetMarkerColor(g0->GetMarkerColor());
  g0I->SetMarkerStyle(g0->GetMarkerStyle());
  g0I->SetMarkerSize( g0->GetMarkerSize());

  g1I->SetLineColor(  g1->GetLineColor());
  g1I->SetMarkerColor(g1->GetMarkerColor());
  g1I->SetMarkerStyle(g1->GetMarkerStyle());
  g1I->SetMarkerSize( g1->GetMarkerSize());

  g2I->SetLineColor(  g2->GetLineColor());
  g2I->SetMarkerColor(g2->GetMarkerColor());
  g2I->SetMarkerStyle(g2->GetMarkerStyle());
  g2I->SetMarkerSize( g2->GetMarkerSize());

  g3I->SetLineColor(  g3->GetLineColor());
  g3I->SetMarkerColor(g3->GetMarkerColor());
  g3I->SetMarkerStyle(g3->GetMarkerStyle());
  g3I->SetMarkerSize( g3->GetMarkerSize());

  g4I->SetLineColor(  g4->GetLineColor());
  g4I->SetMarkerColor(g4->GetMarkerColor());
  g4I->SetMarkerStyle(g4->GetMarkerStyle());
  g4I->SetMarkerSize( g4->GetMarkerSize());

  g5I->SetLineColor(  g5->GetLineColor());
  g5I->SetMarkerColor(g5->GetMarkerColor());
  g5I->SetMarkerStyle(g5->GetMarkerStyle());
  g5I->SetMarkerSize( g5->GetMarkerSize());

  g6I->SetLineColor(  g6->GetLineColor());
  g6I->SetMarkerColor(g6->GetMarkerColor());
  g6I->SetMarkerStyle(g6->GetMarkerStyle());
  g6I->SetMarkerSize( g6->GetMarkerSize());


//  TLine* lLeft = new TLine(-37,0,-37,2.2);
//  //lLeft->SetLineColor(kGreen+1);
//  lLeft->SetLineStyle(2);
//
//  TLine* lRight = new TLine(37,0,37,2.2);
//  //lRight->SetLineColor(kGreen+1);
//  lRight->SetLineStyle(2);
//

  //  TLegend *leg1 = new TLegend(0.7,0.7,0.9,0.9);
  TLegend *leg1 = new TLegend(0.4,0.6,0.8,0.8);
  leg1->SetNColumns(1);
  leg1->AddEntry(g0,"No SC","p");
  leg1->AddEntry(g1,"Vertical 1 layer","p");
  leg1->AddEntry(g2,"Helix A 1 layers","p");
  leg1->AddEntry(g3,"Helix B 1 layer","p");
  leg1->AddEntry(g4,"Vertical 2 layers","p");
  leg1->AddEntry(g5,"Helix A 2 layers","p");
  leg1->AddEntry(g6,"Helix B 2 layer","p");

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

  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas();
  h_frame->Draw("");
//  lLeft->Draw("same");
//  lRight->Draw("same");
  leg1->Draw();
  g0->Draw("LPsame");
  g1->Draw("LPsame");
  g2->Draw("LPsame");
  g3->Draw("LPsame");
  g4->Draw("LPsame");
  g5->Draw("LPsame");
  g6->Draw("LPsame");
  gPad->RedrawAxis();
//  c1->Print("../Plots/Public/BvsZ_v2_warm_cryo.eps");
//  c1->Print("../Plots/Public/BvsZ_v2_warm_cryo.png");


  TCanvas *c2 = new TCanvas();
  h_frameI->Draw("");
  leg1->Draw();
  g0I->Draw("LPsame");
  g1I->Draw("LPsame");
  g2I->Draw("LPsame");
  g3I->Draw("LPsame");
  g4I->Draw("LPsame");
  g5I->Draw("LPsame");
  g6I->Draw("LPsame");
  gPad->RedrawAxis();
//  c2->Print("../Plots/Public/BvsZ_v2_warm_cryo.eps");
//  c2->Print("../Plots/Public/BvsZ_v2_warm_cryo.png");

}
