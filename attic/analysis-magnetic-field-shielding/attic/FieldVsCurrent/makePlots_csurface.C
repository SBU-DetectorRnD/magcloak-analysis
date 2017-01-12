makePlots_csurface()
{
  TTree *t1 = new TTree();
  t1->ReadFile("data_2013_12_06_FieldAtCylinderSurfce.dat","");
  t1->Print();

  vector<float> v_x_1, v_y_1, v_ex_1, v_ey_1;
  t1->Draw("Bplusr*(-1):Bcenter","Bcenter>-999");
  t1->Draw("Bminusr:Bcenter","Bcenter>-999","same");

  //

//  for ( int i = 0; i < t1->GetEntries(""); i++ )
//    {
//      v_x_1.push_back(t1->GetV2()[i]);
//      v_y_1.push_back(t1->GetV1()[i]);
//      v_ex_1.push_back(0);
//      v_ey_1.push_back(0.03);
//    }
//  TGraphErrors* g_center = new TGraphErrors( t1->GetEntries(""), &(v_x_1[0]), &(v_y_1[0]), &(v_ex_1[0]), &(v_ey_1[0]) );
//  g_center->SetMarkerColor(kBlue);
//
//  TF1* f_center = new TF1("f_center","[0]*x+[1]");
//  g_center->Fit("f_center");
//  cout << "Chisquare: " << f_center->GetChisquare() << endl;
//
//  TLegend *leg1 = new TLegend(0.2,0.2,0.8,0.35);
//  leg1->AddEntry(g_center,"center","p");
//  //leg1->AddEntry(lZero,"I = 0.00 A, T = lN_{2}, z - z_{0} = 0 mm","l");
//
//  TH1F* h_frame = new TH1F("h_frame","",30,0,3);
//  h_frame->GetYaxis()->SetTitle("B_{y} [mT]");
//  h_frame->GetXaxis()->SetTitle("I [A]");
//  h_frame->GetYaxis()->SetRangeUser(0,60);
//  h_frame->GetXaxis()->SetTitleOffset(1.5);
//
//  gStyle->SetOptStat(0);
//
////  TCanvas *c0 = new TCanvas();
////  t1->Draw("B","I>0.09&&z==0");
//
//  TCanvas *c1 = new TCanvas();
//  h_frame->Draw();
//  //leg1->Draw();
//  //lZero->Draw("same");
//  g_center->Draw("Psame");
//  c1->Print("B_vs_I_lowPower.png");
//
}
