makePlots()
{
  TTree *t1 = new TTree();
  t1->ReadFile("data_2013_12_03_FieldMap.dat","");
  t1->Print();

  vector<float> v_x_1, v_y_1, v_ex_1, v_ey_1;
  t1->Draw("B_dx:d","B_dx>=0");
  for ( int i = 0; i < t1->GetEntries("B_dx>=0"); i++ )
    {
      v_x_1.push_back(t1->GetV2()[i]);
      v_y_1.push_back(t1->GetV1()[i]);
      v_ex_1.push_back(0);
      v_ey_1.push_back(0.03);
    }
  TGraphErrors* g_x = new TGraphErrors( v_x_1.size(), &(v_x_1[0]), &(v_y_1[0]), &(v_ex_1[0]), &(v_ey_1[0]) );
  g_x->SetMarkerColor(kGreen+1);
  g_x->SetMarkerStyle(23);
  g_x->SetMarkerSize(1.2);

  vector<float> v_x_2, v_y_2, v_ex_2, v_ey_2;
  t1->Draw("B_dy:d","B_dy>=0");
  for ( int i = 0; i < t1->GetEntries("B_dy>=0"); i++ )
    {
      v_x_2.push_back(t1->GetV2()[i]);
      v_y_2.push_back(t1->GetV1()[i]);
      v_ex_2.push_back(0);
      v_ey_2.push_back(0.03);
    }
  TGraphErrors* g_y = new TGraphErrors( v_x_2.size(), &(v_x_2[0]), &(v_y_2[0]), &(v_ex_2[0]), &(v_ey_2[0]) );
  g_y->SetMarkerColor(kBlue);
  g_y->SetMarkerStyle(22);
  g_y->SetMarkerSize(1.2);

  vector<float> v_x_3, v_y_3, v_ex_3, v_ey_3;
  t1->Draw("B_dz:d","B_dz>=0");
  for ( int i = 0; i < t1->GetEntries("B_dz>=0"); i++ )
    {
      v_x_3.push_back(t1->GetV2()[i]);
      v_y_3.push_back(t1->GetV1()[i]);
      v_ex_3.push_back(0);
      v_ey_3.push_back(0.03);
    }
  TGraphErrors* g_z = new TGraphErrors( v_x_3.size(), &(v_x_3[0]), &(v_y_3[0]), &(v_ex_3[0]), &(v_ey_3[0]) );
  g_z->SetMarkerColor(kRed);
  g_z->SetMarkerStyle(24);
  g_z->SetMarkerSize(1.2);

  TLegend *leg1 = new TLegend(0.4,0.22,0.8,0.40);
  leg1->AddEntry(g_x,"d = x - x_{0}","p");
  leg1->AddEntry(g_y,"d = y - y_{0}","p");
  leg1->AddEntry(g_z,"d = z - z_{0}","p");
  //leg1->AddEntry(lZero,"I = 0.00 A, T = lN_{2}, z - z_{0} = 0 mm","l");

  TH1F* h_frame = new TH1F("h_frame","",100,-80,80);
  h_frame->GetYaxis()->SetTitle("B_{y} [mT]");
  h_frame->GetXaxis()->SetTitle("d [mm]");
  h_frame->GetYaxis()->SetRangeUser(0,130);
  h_frame->GetXaxis()->SetTitleOffset(1.5);

  gStyle->SetOptStat(0);

//  TCanvas *c0 = new TCanvas();
//  t1->Draw("B","I>0.09&&z==0");

  TCanvas *c1 = new TCanvas();
  h_frame->Draw();
  leg1->Draw();
  //lZero->Draw("same");
  g_x->Draw("Psame");
  g_y->Draw("Psame");
  g_z->Draw("Psame");
  c1->Print("FieldMapDipole.png");

}
