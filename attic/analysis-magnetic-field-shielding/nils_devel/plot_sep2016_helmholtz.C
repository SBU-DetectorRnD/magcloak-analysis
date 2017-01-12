int plot_sep2016_helmholtz()
{
  gStyle->SetOptStat(0);

  TFile *fl1 = new TFile("output/BintVsBext_sep2016_helmholtz_sample1_1layer.root","OPEN");
  TFile *fl2 = new TFile("output/BintVsBext_sep2016_helmholtz_sample2_2layer.root","OPEN");

  TGraphErrors *gl1 = (TGraphErrors*)fl1->Get("Bint_Vs_Bext");
  TGraphErrors *gl2 = (TGraphErrors*)fl2->Get("Bint_Vs_Bext");

  gl1->SetMarkerColor(8);
  gl2->SetMarkerColor(46);

  TCanvas *cout = new TCanvas();
  gl1->Draw("ALP");
  gl2->Draw("LPsame");

  return 0;
}
