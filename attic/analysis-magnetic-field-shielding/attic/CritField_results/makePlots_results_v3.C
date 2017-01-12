/**
 *
 */

makePlots_results_v3()
{

  gStyle->SetOptStat(0);

  // Results file format:
  //   bext = M[:,0]
  //   bext_err = M[:,1]
  //   bint = M[:,2]
  //   bint_err = M[:,3]
  //   bshield = M[:,4]
  //   bshield_err = M[:,5]
  //   shielding_factor = M[:,6]
  //   shielding_factor_err = M[:,7]
  //   chi2red = M[:,8]


  TTree *t_1l = new TTree();
  t_1l->ReadFile("critical_field_1layer_bestSample_combined_results.txt","bext/F:bext_err:bin:bin_err:bshield:bshield_err:shielding_factor:shielding_factor_err:chi2red");

  TTree *t_1l_hs = new TTree();
  t_1l_hs->ReadFile("critical_field_1layer_combined_horizontal_sheath.txt","bext/F:bext_err:bin:bin_err:bshield:bshield_err:shielding_factor:shielding_factor_err:chi2red");

  // -----

  t_1l_hs->Draw("bin:bext:bin_err","","");
  TGraphErrors* g_bin_1l_hs = new TGraphErrors( t_1l_hs->GetEntries(""), t_1l_hs->GetV2(), t_1l_hs->GetV1(), 0, t_1l_hs->GetV3() );
  g_bin_1l_hs->SetLineColor(kBlue);
  g_bin_1l_hs->SetMarkerColor(kBlue);
  g_bin_1l_hs->SetMarkerStyle(20);
  g_bin_1l_hs->SetMarkerSize(1.1);

  t_1l->Draw("bin:bext:bin_err","","");
  TGraphErrors* g_bin_1l = new TGraphErrors( t_1l->GetEntries(""), t_1l->GetV2(), t_1l->GetV1(), 0, t_1l->GetV3() );
  g_bin_1l->SetLineColor(kBlack);
  g_bin_1l->SetMarkerColor(kBlack);
  g_bin_1l->SetMarkerStyle(20);
  g_bin_1l->SetMarkerSize(1.1);

  // -----

  t_1l_hs->Draw("bshield:bext:bshield_err","","");
  TGraphErrors* g_bshield_1l_hs = new TGraphErrors( t_1l_hs->GetEntries(""), t_1l_hs->GetV2(), t_1l_hs->GetV1(), 0, t_1l_hs->GetV3() );
  g_bshield_1l_hs->SetLineColor(kBlue);
  g_bshield_1l_hs->SetMarkerColor(kBlue);
  g_bshield_1l_hs->SetMarkerStyle(20);
  g_bshield_1l_hs->SetMarkerSize(1.1);

  t_1l->Draw("bshield:bext:bshield_err","","");
  TGraphErrors* g_bshield_1l = new TGraphErrors( t_1l->GetEntries(""), t_1l->GetV2(), t_1l->GetV1(), 0, t_1l->GetV3() );
  g_bshield_1l->SetLineColor(kBlack);
  g_bshield_1l->SetMarkerColor(kBlack);
  g_bshield_1l->SetMarkerStyle(20);
  g_bshield_1l->SetMarkerSize(1.1);

  // -----

  t_1l_hs->Draw("shielding_factor:bext:shielding_factor_err","","");
  TGraphErrors* g_shielding_factor_1l_hs = new TGraphErrors( t_1l_hs->GetEntries(""), t_1l_hs->GetV2(), t_1l_hs->GetV1(), 0, t_1l_hs->GetV3() );
  g_shielding_factor_1l_hs->SetLineColor(kBlue);
  g_shielding_factor_1l_hs->SetMarkerColor(kBlue);
  g_shielding_factor_1l_hs->SetMarkerStyle(20);
  g_shielding_factor_1l_hs->SetMarkerSize(1.1);

  t_1l->Draw("shielding_factor:bext:shielding_factor_err","","");
  TGraphErrors* g_shielding_factor_1l = new TGraphErrors( t_1l->GetEntries(""), t_1l->GetV2(), t_1l->GetV1(), 0, t_1l->GetV3() );
  g_shielding_factor_1l->SetLineColor(kBlack);
  g_shielding_factor_1l->SetMarkerColor(kBlack);
  g_shielding_factor_1l->SetMarkerStyle(20);
  g_shielding_factor_1l->SetMarkerSize(1.1);

  // -----

  TGraphErrors *g_shielding_factor_95 = new TGraphErrors(4);
  g_shielding_factor_95->SetMarkerSize(1.2);
  g_shielding_factor_95->SetMarkerColor(kBlue);
  TH1F* htemp_xl = new TH1F("htemp_xl","",501,-0.5,500.5);

  t_1l->Draw("bext >> htemp_xl","shielding_factor>0.95");
  g_shielding_factor_95->SetPoint(0, 1, htemp_xl->GetBinCenter(htemp_xl->FindLastBinAbove()) );
  htemp_xl->Reset("ICES");

  t_1l_hs->Draw("bext >> htemp_xl","shielding_factor>0.95");
  g_shielding_factor_95->SetPoint(3, 4, htemp_xl->GetBinCenter(htemp_xl->FindLastBinAbove()) );
  htemp_xl->Reset("ICES");

  for ( int p = 0; p < g_shielding_factor_95->GetN(); p++ )
    g_shielding_factor_95->SetPointError(p, 0, 2);

  // -----

  TGraphErrors *g_shielding_factor_90 = new TGraphErrors(4);

  t_1l->Draw("bext >> htemp_xl","shielding_factor>0.90");
  g_shielding_factor_90->SetPoint(0, 1, htemp_xl->GetBinCenter(htemp_xl->FindLastBinAbove()) );
  htemp_xl->Reset("ICES");

  t_1l_hs->Draw("bext >> htemp_xl","shielding_factor>0.90");
  g_shielding_factor_90->SetPoint(3, 4, htemp_xl->GetBinCenter(htemp_xl->FindLastBinAbove()) );
  htemp_xl->Reset("ICES");

  for ( int p = 0; p < g_shielding_factor_90->GetN(); p++ )
    g_shielding_factor_90->SetPointError(p, 0, 2);

  // -----

  TH1F* hframe_1 = new TH1F("hframe_1","",501,-0.5,500.5);
  hframe_1->GetXaxis()->SetTitle("B_{0} [mT]");
  hframe_1->GetYaxis()->SetTitle("B_{in} [mT]");
  hframe_1->GetYaxis()->SetRangeUser(0,500);

  //  TH1F* hframe_2 = new TH1F("hframe_2","",501,-0.5,500.5);
  TH1F* hframe_2 = new TH1F("hframe_2","",600,0.5,600.5);
  hframe_2->GetXaxis()->SetTitle("B_{0} [mT]");
  hframe_2->GetYaxis()->SetTitle("B_{shield} [mT]");
  //hframe_2->GetYaxis()->SetRangeUser(0,75);
  hframe_2->GetYaxis()->SetRangeUser(0.5,100);

  TH1F* hframe_2b = new TH1F("hframe_2b","",600,0.5,600.5);
  hframe_2b->GetXaxis()->SetTitle("B_{0} [mT]");
  hframe_2b->GetYaxis()->SetTitle("B_{shield} [mT]");
  hframe_2b->GetYaxis()->SetRangeUser(0.5,40);

  TH1F* hframe_3 = new TH1F("hframe_3","",501,-0.5,500.5);
  hframe_3->GetXaxis()->SetTitle("B_{0} [mT]");
  hframe_3->GetYaxis()->SetTitle("B_{shield} / B_{0}");
  hframe_3->GetYaxis()->SetRangeUser(0,1);

  TH1F* hframe_4 = new TH1F("hframe_4","",4,0.5,4.5);
  hframe_4->GetXaxis()->SetTitle("# SC layers");
  hframe_4->GetYaxis()->SetTitle("B_{95}");
  hframe_4->GetYaxis()->SetRangeUser(0,80);

  // -----

  TLegend *leg1 = new TLegend(0.2,0.6,0.6,0.8);
  leg1->SetNColumns(1);
  leg1->AddEntry(g_bin_1l_hs,"2 layer vertical","p");
  leg1->AddEntry(g_bin_1l,"1 layer","p");

  TLegend *leg2 = new TLegend(0.2,0.65,0.6,0.85);
  leg2->SetNColumns(1);
  leg2->AddEntry(g_bshield_1l_hs,"2 layer vertical","p");
  leg2->AddEntry(g_bshield_1l,"1 layer","p");

  TLegend *leg3 = new TLegend(0.5,0.6,0.9,0.8);
  leg3->SetNColumns(1);
  leg3->AddEntry(g_bshield_1l_hs,"1 layer HS","p");
  leg3->AddEntry(g_bshield_1l,"1 layer","p");

  // -----

  TCanvas *c1 = new TCanvas();
  hframe_1->Draw("rad");
  leg1->Draw();
  g_bin_1l_hs->Draw("Psame");
  g_bin_1l->Draw("Psame");

  gPad->RedrawAxis();

  //  c1->Print("Plots/CritField_results_v1_bin.png");
  //  c1->Print("Plots/CritField_results_v1_bin.eps");

  TCanvas *c2 = new TCanvas();
  hframe_2->Draw("rad");
  leg2->Draw();
  g_bshield_1l->Draw("Psame");
  g_bshield_1l_hs->Draw("Psame");

  gPad->RedrawAxis();

  c2->SetLogx();
  c2->SetLogy();

  TLine *l_2 = new TLine(0.5,0.5,100,100);
  l_2->SetLineColor(4);
  l_2->SetLineStyle(2);
  l_2->Draw();

  //  c2->Print("Plots/CritField_results_v2_bshield.png");
  //  c2->Print("Plots/CritField_results_v2_bshield.eps");

  TCanvas *c2b = new TCanvas();
  hframe_2b->Draw("");
  g_bshield_1l->Draw("Psame");

  gPad->RedrawAxis();

  c2b->SetLogx();
  c2b->SetLogy();

  TLine *l_2b = new TLine(0.5,0.5,40,40);
  l_2b->SetLineColor(4);
  l_2b->SetLineStyle(2);
  l_2b->Draw();

  //  c2b->Print("Plots/CritField_results_v1_bshield_1layer.png");
  //  c2b->Print("Plots/CritField_results_v1_bshield_1layer.eps");

//  hframe_2b->GetXaxis()->SetRangeUser(0,100);
//  c2b->Print("Plots/CritField_results_v1_bshield_1layer_zoom.png");
//  c2b->Print("Plots/CritField_results_v1_bshield_1layer_zoom.eps");

  TCanvas *c3 = new TCanvas();
  hframe_3->Draw("rad");
  leg3->Draw();
  g_shielding_factor_1l_hs->Draw("Psame");
  g_shielding_factor_1l->Draw("Psame");

  gPad->RedrawAxis();

  //  c3->Print("Plots/CritField_results_v1_shielding_factor.png");
  //  c3->Print("Plots/CritField_results_v1_shielding_factor.eps");

//  TCanvas *c4 = new TCanvas();
//  hframe_4->Draw("rad");
//  //  leg2->Draw();
//  //  g_shielding_factor_90->Draw("Psame");
//  g_shielding_factor_95->Draw("Psame");
//
//  gPad->RedrawAxis();
//
//  c4->Print("Plots/CritField_results_v1_shielding_factor_95.png");
//  c4->Print("Plots/CritField_results_v1_shielding_factor_95.eps");

}
