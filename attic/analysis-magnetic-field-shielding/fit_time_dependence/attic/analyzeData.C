#include "readData.C"
#include "binData.C"
#include "smoothData.C"
#include "fitData.C"
#include "fitData2.C"
#include "makePlot.C"
#include "lin2log.C"
//#include "plotResiduals.C"

void analyzeData(
//  TString sdatafile = "data/DataFile_160816_183339.txt", //45L 23A
//  TString sdatafile = "data/DataFile_160811_163122.txt",
TString sdatafile = "data/DataFile_150629_160304.txt",
  Double_t tmin = 0,
  Double_t tmax = 36000
  // Double_t Bmin = 0,
  // Double_t Bmax = 200
)
{
  /* Basic ROOT style settings */
  gStyle->SetOptStat(0);

  /* Create temporary scratch canvas */
  TCanvas *cscratch = new TCanvas("cscratch");
  cscratch->SetTitle("Scratch Canvas");

  /* Read data*/
  TTree *tdata = readData( sdatafile );

  double tdata_Bmax = tdata->GetMaximum("B");
  double tdata_Bmin = tdata->GetMinimum("B");

  /* Create 2d histogram of data */
  double h2data_Bmax = ((int) (tdata_Bmax * 1.02)) + 0.05;
  double h2data_Bmin = ((int) (tdata_Bmin * 0.98)) - 0.05;
  int h2data_nbin = (h2data_Bmax - h2data_Bmin)*10+1;

  cout << "TH2F limits: " << h2data_Bmax << " " << h2data_Bmin << " " << h2data_nbin << endl;

  TH2F* h2data = new TH2F("h2data","",60,tmin,tmax/10,h2data_nbin,h2data_Bmin,h2data_Bmax);
  h2data->GetXaxis()->SetTitle("time (s)");
  h2data->GetYaxis()->SetTitle("B (mT)");
  tdata->Draw("B:time >> h2data","","");

  /* Store data in TGraphErrors */
  tdata->Draw("B:time:0.03:0","","");
  TGraphErrors *gdata = new TGraphErrors( tdata->GetEntries(),
  &(tdata->GetV2()[0]),
  &(tdata->GetV1()[0]),
  &(tdata->GetV4()[0]),
  &(tdata->GetV3()[0]) );
  gdata->GetXaxis()->SetTitle("time (s)");
  gdata->GetYaxis()->SetTitle("B (mT)");

  /* smooth data by re-binning data in x */
  TGraphErrors *gsmooth = binData( gdata, 600, tmin, tmax );
  gsmooth->GetXaxis()->SetTitle("time (s)");
  gsmooth->GetYaxis()->SetTitle("B (mT)");

  /* adjust uncertainties */
  for ( int i = 0; i < gsmooth->GetN(); i++ )
  {
    double err_stat = gsmooth->GetErrorY( i );
    double err_binw = 0.0;

    gsmooth->SetPointError( i, 0, sqrt( err_stat * err_stat + err_binw * err_binw ) );
  }

  /* Plot raw data */
  TCanvas *craw = new TCanvas();
  craw->SetTitle("Raw data");
  gdata->Draw("AP");
  craw->Print("new_RawData.png");

  /* Plot raw data as 2d-histogram and overlay re-binned data */
  TCanvas *craw2 = new TCanvas();
  h2data->Draw("colz");
  gsmooth->Draw("Psame");
  craw2->Print("new_SmoothData.png");

  // TGraphErrors* gsmoothlin = lin2log( gsmooth );
  // gsmooth = gsmoothlin;

  cscratch->cd();
  /* Fit function to data */
  /* NOTE: log(m/n) = log(m) - log(n) */
  /* NOTE: using 'x+1' instead of 'x' can help avoid divergence */
  //TF1* ffit = new TF1("ffit","[0]*log( (x+1+[2]) / [1])"); // -> trouble with contour plots
  TF1* ffit = new TF1("ffit","[0]*log( (x+[2]) ) + [1]"); // -> nicer contour plots!
  //TF1* ffit = new TF1("ffit","[0]*x*2+[1]*x+[2]");
//  TF1* ffit = new TF1("ffit","[0]*x+[1]*x+[2]");
//  TF1* ffit = new TF1("ffit","[0]*log(x+[1])+[2]*log(x+[3])+[4]");
  //TF1* ffit = new TF1("ffit","log(x+[0])+log(x+[1])+[2]");


  double fitrange[2] = {0,3600};
  ffit->SetRange(fitrange[0],fitrange[1]); // only this range used for fitting

  /* Fit function to data using minimizer explicitly */
  //  double startvalues[3] = {10., 0.1, 10.};
  //double startvalues[3] = {0.1, 0.1, 0.1};
  //fitData( gsmooth, ffit, startvalues );


  /* Fit function to data using wrapper functions */
  fitData2( gsmooth, ffit );

  //  cout << "Fit chi2: " << ffit->Chisquare() << endl;

  // h2->Draw("colz");
  // g1->Draw("psame");
  //
  // TCanvas *c3 = new TCanvas();
}
