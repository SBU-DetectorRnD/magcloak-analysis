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
//TString fname_1("DataFile_141111_145840_helmholtz_map.txt");
//TString fname_2("DataFile_141111_144527_fm_bz_scan.txt");

TString fname_1("DataFile_141111_211629_bvz_helmholtz.txt");
TString fname_2("DataFile_141111_204657_room_bvz.txt");

/* Helper strings */
TString readString("z/F:B:dB:I:dI");
TString drawStringB("B:(z-153.5):0:0");

makePlots_v1()
{
  gStyle->SetOptStat(0);

  float fmlength = 127;

  TLine* lLeft = new TLine(-fmlength/2.,16.5,-fmlength/2.,22);
  //lLeft->SetLineColor(kGreen+1);
  lLeft->SetLineStyle(2);

  TLine* lRight = new TLine(fmlength/2.,16.5,fmlength/2.,22);
  //lRight->SetLineColor(kGreen+1);
  lRight->SetLineStyle(2);

  /* c1 */
  TTree *t1_c1 = new TTree();
  TTree *t2_c1 = new TTree();

  t1_c1->ReadFile( fname_1, readString );
  t2_c1->ReadFile( fname_2, readString );

  TGraphErrors* g1_c1 = makeGraph( t1_c1, drawStringB );
  TGraphErrors* g2_c1 = makeGraph( t2_c1, drawStringB );

  TLegend *leg_c1 = new TLegend(0.32,0.55,0.8,0.7);
  leg_c1->SetNColumns(1);
  leg_c1->AddEntry(g1_c1,"empty Helmholtz coil","p");
  leg_c1->AddEntry(g2_c1,"steel / epoxy cylinder","p");

  g1_c1->SetLineColor(kRed);
  g1_c1->SetMarkerColor(kRed);
  g1_c1->SetMarkerStyle(20);
  g1_c1->SetMarkerSize(1.1);

  g2_c1->SetLineColor(kBlue);
  g2_c1->SetMarkerColor(kBlue);
  g2_c1->SetMarkerStyle(21);
  g2_c1->SetMarkerSize(1.1);

  TH1F* h_frame = new TH1F("h_frame","",131,-100,100);
  h_frame->GetYaxis()->SetTitle("B_{y} [mT]");
  h_frame->GetXaxis()->SetTitle("z - z_{c} [mm]");
  h_frame->GetYaxis()->SetRangeUser(16.5,22);
  h_frame->GetXaxis()->SetTitleOffset(1.5);
  h_frame->GetYaxis()->SetNdivisions(505);

  TCanvas *c1 = new TCanvas();
  h_frame->Draw("");
  leg_c1->Draw();
  lLeft->Draw("same");
  lRight->Draw("same");
  g1_c1->Draw("LPsame");
  g2_c1->Draw("LPsame");
  gPad->RedrawAxis();
  c1->Print("FmEpoxy_BvsZ_v1.eps");
  c1->Print("FmEpoxy_BvsZ_v1.png");

}
