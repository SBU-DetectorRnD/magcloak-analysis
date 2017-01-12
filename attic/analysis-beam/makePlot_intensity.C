makePlot_intensity()
{
  gStyle->SetOptStat(0);

  TFile *f1 = new TFile("intensity1.root","OPEN");
  TFile *f2 = new TFile("intensity2.root","OPEN");
  TFile *f3 = new TFile("intensity3.root","OPEN");

  TH1D* h1 = (TH1D*)f1->Get("h_I");
  TH1D* h2 = (TH1D*)f2->Get("h_I");
  TH1D* h3 = (TH1D*)f3->Get("h_I");
  
  h1->SetLineColor(kGreen);
  h2->SetLineColor(kBlue);
  h3->SetLineColor(kRed);

  TCanvas *c1 = new TCanvas();
  c1->SetLogy();
  h1->Draw();
  h2->Draw("same");
  h3->Draw("same");
}
