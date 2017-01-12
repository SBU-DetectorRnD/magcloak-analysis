makePlots()
{
  TTree *t1 = new TTree();
  t1->ReadFile("data_2013_12_02_BvsZWarm.txt","");
  t1->Print();

  TTree *t2 = new TTree();
  t2->ReadFile("data_2013_12_02_BvsZCryo.txt","");

  vector<float> v_x_1, v_y_1, v_ex_1, v_ey_1;
  t1->Draw("B-B0:z","I>0.09");
  for ( int i = 0; i < t1->GetEntries("I>0.09"); i++ )
    {
      v_x_1.push_back(t1->GetV2()[i]);
      v_y_1.push_back(t1->GetV1()[i]);
      v_ex_1.push_back(0);
      v_ey_1.push_back(0.03);
    }
  TGraphErrors* g1 = new TGraphErrors( t1->GetEntries("I>0.09"), &(v_x_1[0]), &(v_y_1[0]), &(v_ex_1[0]), &(v_ey_1[0]) );
  g1->SetMarkerColor(kRed);

  vector<float> v_x_2, v_y_2, v_ex_2, v_ey_2;
  t2->Draw("B-B0:z","I>0.09");
  for ( int i = 0; i < t2->GetEntries("I>0.09"); i++ )
    {
      v_x_2.push_back(t2->GetV2()[i]);
      v_y_2.push_back(t2->GetV1()[i]);
      v_ex_2.push_back(0);
      v_ey_2.push_back(0.03);
    }
  TGraphErrors* g2 = new TGraphErrors( t2->GetEntries("I>0.09"), &(v_x_2[0]), &(v_y_2[0]), &(v_ex_2[0]), &(v_ey_2[0]) );
  g2->SetMarkerColor(kBlue);

  //  t1->Draw("B/4.061*2.059:z","I>0.09","same");

  TLine* lZero = new TLine(-65.5,0.0,65.5,0.0);
  lZero->SetLineColor(kGreen+1);
  lZero->SetLineStyle(2);

  TLegend *leg1 = new TLegend(0.2,0.2,0.8,0.35);
  leg1->AddEntry(g1,"I = 0.10 A, T = warm","p");
  leg1->AddEntry(g2,"I = 0.10 A, T = lN_{2}","p");
  //leg1->AddEntry(lZero,"I = 0.00 A, T = lN_{2}, z - z_{0} = 0 mm","l");

  TH1F* h_frame = new TH1F("h_frame","",131,-65.5,65.5);
  h_frame->GetYaxis()->SetTitle("B_{y} [mT]");
  h_frame->GetXaxis()->SetTitle("z - z_{0} [mm]");
  h_frame->GetYaxis()->SetRangeUser(0,2.1);
  h_frame->GetXaxis()->SetTitleOffset(1.5);

  gStyle->SetOptStat(0);

  TCanvas *c0 = new TCanvas();
  t1->Draw("B","I>0.09&&z==0");

  TCanvas *c1 = new TCanvas();
  h_frame->Draw();
  leg1->Draw();
  //lZero->Draw("same");
  g1->Draw("Psame");
  g2->Draw("Psame");
  c1->Print("B_vs_z_warm_cryo.png");

}
