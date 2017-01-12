
makePlots_compareSamples_v1()
{

  TString datafolder("../analysis/root/output/");

  /* Get files */
  TFile* f03 = new TFile( TString(datafolder).Append("fitResults_run003_1layer_sheath.root"), "OPEN" );
  TFile* f12 = new TFile( TString(datafolder).Append("fitResults_run012_1layer_sheath.root"), "OPEN" );
  TFile* f13 = new TFile( TString(datafolder).Append("fitResults_run013_1layer_sheath.root"), "OPEN" );
  TFile* f14 = new TFile( TString(datafolder).Append("fitResults_run014_1layer_sheath.root"), "OPEN" );
  TFile* f15 = new TFile( TString(datafolder).Append("fitResults_run015_1layer_sheath.root"), "OPEN" );
  //  TFile* f16 = new TFile( TString(datafolder).Append("fitResults_run016_1layer_sheath.root"), "OPEN" ); // BAD run

  /* Get trees */
  TTree* t03 = (TTree*)f03->Get("fitResults");
  TTree* t12 = (TTree*)f12->Get("fitResults");
  TTree* t13 = (TTree*)f13->Get("fitResults");
  TTree* t14 = (TTree*)f14->Get("fitResults");
  TTree* t15 = (TTree*)f15->Get("fitResults");
  //  TTree* t16 = (TTree*)f16->Get("fitResults");

  /* Create graphs */
  t12->Draw("bshield:bext:bshield_err:bext_err");
  TGraphErrors* g12 = new TGraphErrors( t12->GetEntries(), t12->GetV2(), t12->GetV1(), t12->GetV4(), t12->GetV3() );
  g12->SetMarkerColor(kRed);
  g12->SetLineColor(kRed);

  t13->Draw("bshield:bext:bshield_err:bext_err");
  TGraphErrors* g13 = new TGraphErrors( t13->GetEntries(), t13->GetV2(), t13->GetV1(), t13->GetV4(), t13->GetV3() );
  g13->SetMarkerColor(kBlue);
  g13->SetLineColor(kBlue);

  t14->Draw("bshield:bext:bshield_err:bext_err");
  TGraphErrors* g14 = new TGraphErrors( t14->GetEntries(), t14->GetV2(), t14->GetV1(), t14->GetV4(), t14->GetV3() );
  g14->SetMarkerColor(kGreen+1);
  g14->SetLineColor(kGreen+1);

  t15->Draw("bshield:bext:bshield_err:bext_err");
  TGraphErrors* g15 = new TGraphErrors( t15->GetEntries(), t15->GetV2(), t15->GetV1(), t15->GetV4(), t15->GetV3() );
  g15->SetMarkerColor(kYellow+1);
  g15->SetLineColor(kYellow+2);

  /* Draw graphs */
  TCanvas *c1 = new TCanvas();
  g12->Draw("ALP");
  g13->Draw("LPsame");
  g14->Draw("LPsame");
  g15->Draw("LPsame");

}
