int plot_xcheck_helmholtz()
{
  gStyle->SetOptStat(0);

  TFile *fl1 = new TFile("output/BintVsBext_raphael_helmholtz_1layer.root","OPEN");
  TFile *fl2 = new TFile("output/BintVsBext_raphael_helmholtz_2layer.root","OPEN");
  TFile *fl3 = new TFile("output/BintVsBext_raphael_helmholtz_3layer.root","OPEN");
  TFile *fl4 = new TFile("output/BintVsBext_raphael_helmholtz_4layer.root","OPEN");

  TFile *fl5 = new TFile("output/BintVsBext_sep2016_helmholtz_sample1_1layer.root","OPEN");
  TFile *fl6 = new TFile("output/BintVsBext_sep2016_helmholtz_sample2_2layer.root","OPEN");

  TGraphErrors *gl1 = (TGraphErrors*)fl1->Get("Bint_Vs_Bext");
  TGraphErrors *gl2 = (TGraphErrors*)fl2->Get("Bint_Vs_Bext");
  TGraphErrors *gl3 = (TGraphErrors*)fl3->Get("Bint_Vs_Bext");
  TGraphErrors *gl4 = (TGraphErrors*)fl4->Get("Bint_Vs_Bext");

  TGraphErrors *gl5 = (TGraphErrors*)fl5->Get("Bint_Vs_Bext");
  TGraphErrors *gl6 = (TGraphErrors*)fl6->Get("Bint_Vs_Bext");

  gl1->SetLineColor(8);
  gl2->SetLineColor(46);
  gl3->SetLineColor(9);
  gl4->SetLineColor(6);

  gl5->SetMarkerColor(4);
  gl6->SetMarkerColor(2);

  TCanvas *cout = new TCanvas();
  gl1->Draw("AL");
  gl2->Draw("Lsame");
  gl3->Draw("Lsame");
  gl4->Draw("Lsame");
  gl5->Draw("LPsame");
  gl6->Draw("LPsame");

  return 0;
}
