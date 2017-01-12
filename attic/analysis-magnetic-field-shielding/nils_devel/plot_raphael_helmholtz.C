int plot_raphael_helmholtz()
{
  gStyle->SetOptStat(0);

  TFile *fl1 = new TFile("output/BintVsBext_raphael_helmholtz_1layer.root","OPEN");
  TFile *fl2 = new TFile("output/BintVsBext_raphael_helmholtz_2layer.root","OPEN");
  TFile *fl3 = new TFile("output/BintVsBext_raphael_helmholtz_3layer.root","OPEN");
  TFile *fl4 = new TFile("output/BintVsBext_raphael_helmholtz_4layer.root","OPEN");

  TGraphErrors *gl1 = (TGraphErrors*)fl1->Get("Bint_Vs_Bext");
  TGraphErrors *gl2 = (TGraphErrors*)fl2->Get("Bint_Vs_Bext");
  TGraphErrors *gl3 = (TGraphErrors*)fl3->Get("Bint_Vs_Bext");
  TGraphErrors *gl4 = (TGraphErrors*)fl4->Get("Bint_Vs_Bext");

  gl1->SetMarkerColor(8);
  gl2->SetMarkerColor(46);
  gl3->SetMarkerColor(9);
  gl4->SetMarkerColor(6);

  TCanvas *cout = new TCanvas();
  gl1->Draw("ALP");
  gl2->Draw("LPsame");
  gl3->Draw("LPsame");
  gl4->Draw("LPsame");

  return 0;
}
