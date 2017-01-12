#include "binData.h"
#include "smoothData.h"

void testSmoothData()
{
  gStyle->SetOptStat(0);

  /* Read data*/
  TTree *t1 = new TTree();
  //t1->ReadFile("data/DataFile_160811_163122.txt","time/F:I:B"); // 10L 20A
  //  t1->ReadFile("data/DataFile_160808_132712.txt","time/F:I:B"); // 10L 8A
//  t1->ReadFile("data/DataFile_160816_163111.txt","time/F:I:B"); // 45L 20A
  t1->ReadFile("data/DataFile_160816_183339.txt","time/F:I:B"); // 45L 23A

  /* Store data in TGraphErrors */
  t1->Draw("B:time:0.03:0","","");

  TGraphErrors *g1raw = new TGraphErrors( t1->GetEntries(),
  &(t1->GetV2()[0]),
  &(t1->GetV1()[0]),
  &(t1->GetV4()[0]),
  &(t1->GetV3()[0]) );

  /* bin data in x */
//  TGraphErrors *g1 = binData( g1raw, 36, 0, 3600 );
//  TGraphErrors *g1 = g1raw;
  TGraphErrors *g1 = smoothData( g1raw );


  /* Graph frames for plotting */
  TH1F* h_frmame = new TH1F("h_frmame", "", 100,-100,3800);
  //h_frmame->GetYaxis()->SetRangeUser(95,145);
  h_frmame->GetYaxis()->SetRangeUser(0,1000);
  h_frmame->SetLineColor(kWhite);
  h_frmame->GetXaxis()->SetTitle("time (s)");
  h_frmame->GetYaxis()->SetTitle("field (mT)");

  TH1F* h_pol = (TH1F*)h_frmame->Clone("h_pol");

  /* Draw results */
  TCanvas *c_log = new TCanvas();
  c_log->SetTitle("Logarithmic fit");
  h_frmame->Draw();

  g1raw->SetMarkerColor(kGray);
  g1raw->SetLineColor(kGray);

  g1raw->Draw("sameP");

  g1->Draw("sameP");


}
