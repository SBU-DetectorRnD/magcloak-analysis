
makePlots_mur_vs_r2r1()
{
  gStyle->SetOptStat(0);

  TF1* f_mur = new TF1("f_mur","(x*x+1)/(x*x-1)");
  f_mur->SetRange(1.0,3.0);
  f_mur->SetLineColor(kBlue);
  f_mur->SetLineWidth(4);

  /* Do graphing */
  TH1F* hframe = new TH1F("hframe","",20,1.0,3.0);
  hframe->GetYaxis()->SetRangeUser(1,100);
  hframe->GetXaxis()->SetTitle("R_{2} / R_{1}");
  hframe->GetYaxis()->SetTitle("#mu_{2}");

  TCanvas *c1 = new TCanvas();
  hframe->Draw("rad");

  f_mur->Draw("same");

  gPad->RedrawAxis();

  c1->SetLogy(1);
  c1->Print("FMMeasurements_predict_mur_r2r1.eps");

  // ----

  TLine *l1_5 = new TLine(1.0,1.5,3.0,1.5);
  l1_5->SetLineStyle(2);
  l1_5->SetLineColor(kGray+2);

  TLine *l4_5 = new TLine(1.0,4.5,3.0,4.5);
  l4_5->SetLineStyle(2);
  l4_5->SetLineColor(kGray+2);

  // ----

  TCanvas *c1_1_5 = new TCanvas();
  hframe->Draw("rad");
  f_mur->Draw("same");
  l1_5->Draw("same");
  gPad->RedrawAxis();
  c1_1_5->SetLogy(1);
  c1_1_5->Print("FMMeasurements_predict_mur_r2r1_l1_5.eps");

  TCanvas *c1_4_5 = new TCanvas();
  hframe->Draw("rad");
  f_mur->Draw("same");
  l4_5->Draw("same");
  gPad->RedrawAxis();
  c1_4_5->SetLogy(1);
  c1_4_5->Print("FMMeasurements_predict_mur_r2r1_l4_5.eps");




}
