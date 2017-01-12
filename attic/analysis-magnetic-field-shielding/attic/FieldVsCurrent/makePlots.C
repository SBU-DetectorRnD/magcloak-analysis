makePlots()
{
  TTree *t0 = new TTree();
  t0->ReadFile("data_2013_11_30_IvsB.dat","");
  t0->Print();

  TTree *t1 = new TTree();
  t1->ReadFile("data_2013_11_26_FieldVsCurrent_1LayerSC_cooled.dat","");
  t1->Print();

  TTree *t2 = new TTree();
  t2->ReadFile("data_2013_12_11_FieldVsCurrent_2LayerSC_cooled.dat","");
  t2->Print();

//  TTree *t3 = new TTree();
//  t3->ReadFile("","");
//  t3->Print();

//  TTree *t4 = new TTree();
//  t4->ReadFile("","");
//  t4->Print();

  vector<float> v_x_0, v_y_0, v_ex_0, v_ey_0;
  t0->Draw("BCenter-BOffset:I","");
  for ( int i = 0; i < t0->GetEntries(""); i++ )
    {
      v_x_0.push_back(t0->GetV2()[i]);
      v_y_0.push_back(t0->GetV1()[i]);
      v_ex_0.push_back(0);
      v_ey_0.push_back(0.00);
    }
  TGraphErrors* g0 = new TGraphErrors( t0->GetEntries(""), &(v_x_0[0]), &(v_y_0[0]), &(v_ex_0[0]), &(v_ey_0[0]) );
  g0->SetMarkerColor(kRed);
  g0->SetMarkerStyle(20);

  vector<float> v_x_1, v_y_1, v_ex_1, v_ey_1;
  t1->Draw("BCenter-BOffset:I","");
  for ( int i = 0; i < t1->GetEntries(""); i++ )
    {
      v_x_1.push_back(t1->GetV2()[i]);
      v_y_1.push_back(t1->GetV1()[i]);
      v_ex_1.push_back(0);
      v_ey_1.push_back(0.00);
    }
  TGraphErrors* g1 = new TGraphErrors( t1->GetEntries(""), &(v_x_1[0]), &(v_y_1[0]), &(v_ex_1[0]), &(v_ey_1[0]) );
  g1->SetMarkerColor(kBlue);
  g1->SetMarkerStyle(21);

  vector<float> v_x_2, v_y_2, v_ex_2, v_ey_2;
  t2->Draw("BCenter-BOffset:I","");
  for ( int i = 0; i < t2->GetEntries(""); i++ )
    {
      v_x_2.push_back(t2->GetV2()[i]);
      v_y_2.push_back(t2->GetV1()[i]);
      v_ex_2.push_back(0);
      v_ey_2.push_back(0.00);
    }
  TGraphErrors* g2 = new TGraphErrors( t2->GetEntries(""), &(v_x_2[0]), &(v_y_2[0]), &(v_ex_2[0]), &(v_ey_2[0]) );
  g2->SetMarkerColor(kBlue);
  g2->SetMarkerStyle(22);

//  vector<float> v_x_3, v_y_3, v_ex_3, v_ey_3;
//  t3->Draw("BCenter-BOffset:I","");
//  for ( int i = 0; i < t3->GetEntries(""); i++ )
//    {
//      v_x_3.push_back(t3->GetV2()[i]);
//      v_y_3.push_back(t3->GetV1()[i]);
//      v_ex_3.push_back(0);
//      v_ey_3.push_back(0.00);
//    }
//  TGraphErrors* g3 = new TGraphErrors( t3->GetEntries(""), &(v_x_3[0]), &(v_y_3[0]), &(v_ex_3[0]), &(v_ey_3[0]) );
//  g3->SetMarkerColor(kGreen+1);
//  g3->SetMarkerStyle(22);
//  g3->SetMarkerStyle(23);
//
//  vector<float> v_x_4, v_y_4, v_ex_4, v_ey_4;
//  t4->Draw("BCenter-BOffset:I","");
//  for ( int i = 0; i < t4->GetEntries(""); i++ )
//    {
//      v_x_4.push_back(t4->GetV2()[i]);
//      v_y_4.push_back(t4->GetV1()[i]);
//      v_ex_4.push_back(0);
//      v_ey_4.push_back(0.00);
//    }
//  TGraphErrors* g4 = new TGraphErrors( t4->GetEntries(""), &(v_x_4[0]), &(v_y_4[0]), &(v_ex_4[0]), &(v_ey_4[0]) );
//  g4->SetMarkerColor(kBlack);
//  g4->SetMarkerStyle(22);
//  g4->SetMarkerStyle(29);

  TLegend *leg1 = new TLegend(0.3,0.2,0.8,0.27);
  leg1->SetNColumns(2);
  leg1->AddEntry(g0,"0 SC layers","p");
  leg1->AddEntry(g1,"1 SC layers","p");
  leg1->AddEntry(g2,"2 SC layers","p");
  //leg1->AddEntry(g3,"3 SC layers","p");
  //leg1->AddEntry(g4,"4 SC layers","p");

  TH1F* h_frame = new TH1F("h_frame","",30,0,3);
  h_frame->GetYaxis()->SetTitle("B_{y} [mT]");
  h_frame->GetXaxis()->SetTitle("I [A]");
  h_frame->GetYaxis()->SetRangeUser(0,100);
  h_frame->GetXaxis()->SetTitleOffset(1.5);

  gStyle->SetOptStat(0);

//  TCanvas *c0 = new TCanvas();
//  t1->Draw("B","I>0.09&&z==0");

  TCanvas *c1 = new TCanvas();
  h_frame->Draw();
  leg1->Draw();
  //lZero->Draw("same");
  g0->Draw("Psame");
  g1->Draw("Psame");
  g2->Draw("Psame");
  c1->Print("B_vs_I_lowPower.png");

}
