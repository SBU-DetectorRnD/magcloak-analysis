#include "readData.h"
#include "binData.h"
#include "lin2log.h"
#include "fitTimeDependence.h"
#include "TimeDependenceResult.h"

TimeDependenceResult* analyzeTimeDependence(
  TString sdatafile = "data/DataFile_160816_183339.txt", //45L 23A
//  TString sdatafile = "data/DataFile_160811_163122.txt",
//  TString sdatafile = "data/DataFile_150422_102753.txt", // 4 layer long measurement
//  TString sdatafile = "data/DataFile_150511_124957.txt",
  Double_t fittmin = 0,
  Double_t fittmax = 36000,
  bool b_lin2log_x = false
)
{
  /* Basic ROOT style settings */
  gStyle->SetOptStat(0);

  /* Create temporary scratch canvas */
  TCanvas *cscratch = new TCanvas("cscratch");
  cscratch->SetTitle("Scratch Canvas");

  /* Read data*/
  TTree *tdata = readData( sdatafile );

  /* Get maximum and minimum values for B and time */
  double tdata_Bmax = tdata->GetMaximum("B");
  double tdata_Bmin = tdata->GetMinimum("B");

  double tdata_tmax = tdata->GetMaximum("time");
  double tdata_tmin = 0;

  /* Get mean current */
  tdata->Draw("I>>hcurrent");
  TH1F *hcurrent = (TH1F*)gDirectory->Get("hcurrent");
  double tdata_current_mean = hcurrent->GetMean();
  double tdata_current_rms = hcurrent->GetRMS();

  /* Re-binning data: settings */
  float rebin_twidth = 60; // width of time bin in seconds
  float rebin_tmin = 0.0; // start at t = 0
  int rebin_tnbins = (int) (tdata_tmax / rebin_twidth);
  float rebin_tmax = rebin_tnbins * rebin_twidth;

  /* Create 2d histogram of data */
  double h2data_Bmax = tdata_Bmax * 1.02;
  double h2data_Bmin = tdata_Bmin * 0.98;
  int h2data_Bnbin = 50;//(int)( (h2data_Bmax - h2data_Bmin)*10 );

  double h2data_tmax = rebin_tmax;
  double h2data_tmin = rebin_tmin;
  int h2data_tnbin = rebin_tnbins;

  cout << "TH2F limits for B: " << h2data_Bnbin  << " bins from " << h2data_Bmin << " to " << h2data_Bmax << endl;
  cout << "TH2F limits for time: " << h2data_tnbin  << " bins from " << h2data_tmin << " to " << h2data_tmax << endl;

  TH2F* h2data = new TH2F("h2data","",h2data_tnbin,h2data_tmin,h2data_tmax,h2data_Bnbin,h2data_Bmin,h2data_Bmax);
  h2data->GetXaxis()->SetTitle("time (s)");
  h2data->GetYaxis()->SetTitle("B (mT)");
  tdata->Draw("B:time >> h2data","","");

  /* Store data in TGraphErrors */
  tdata->Draw("B:time:0.03:0.0","","");
  TGraphErrors *gdata = new TGraphErrors( tdata->GetEntries(),
  &(tdata->GetV2()[0]),
  &(tdata->GetV1()[0]),
  &(tdata->GetV4()[0]),
  &(tdata->GetV3()[0]) );
  gdata->GetXaxis()->SetTitle("time (s)");
  gdata->GetYaxis()->SetTitle("B (mT)");

  /* smooth data by re-binning data in x */
  //TGraphErrors *gsmooth = gdata;
  TGraphErrors *gsmooth = binData( gdata, rebin_tnbins, rebin_tmin, rebin_tmax );
  gsmooth->GetXaxis()->SetTitle("time (s)");
  gsmooth->GetYaxis()->SetTitle("B (mT)");

  /* adjust uncertainties */
  for ( int i = 0; i < gsmooth->GetN(); i++ )
  {
    double err_stat = gsmooth->GetErrorY( i );
    double err_binw = 0.0;

    gsmooth->SetPointError( i, 0, sqrt( err_stat * err_stat + err_binw * err_binw ) );
  }

  /* set x axis to logarithm */
  TGraphErrors* gsmooth_log = lin2log( gsmooth );
  gsmooth_log->SetTitle("");
  gsmooth_log->GetXaxis()->SetTitle("log( time / s )");
  gsmooth_log->GetYaxis()->SetTitle("B (mT)");

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

  /* Plot re-binned data with x-axis logarithm */
  TCanvas *crebin2 = new TCanvas();
  gsmooth_log->Draw("AP");
  crebin2->Print("new_SmoothDataLog.png");

  /* Fit time dependence */
  TimeDependenceResult* tdr = new TimeDependenceResult();

  cscratch->cd();
  if ( b_lin2log_x == false  )
    tdr = fitTimeDependence( gsmooth, b_lin2log_x, fittmin, fittmax );
  else
    {
      if (fittmin < 1)
        fittmin = 1;
      tdr = fitTimeDependence( gsmooth_log, b_lin2log_x, log(fittmin), log(fittmax) );
    }

  /* Update parameters of result */
  tdr->datafile = sdatafile;
  tdr->current = tdata_current_mean;

  /* Print fit results summary */
  cout << "TimeDependenceResult: " << tdr->datafile
       << " " << tdr->current
       << " " << tdr->chi2
       << " " << tdr->ndf
       << " " << tdr->prob
       << " " << tdr->par0
       << " " << tdr->par1
       << " " << tdr->par2
       << " " << tdr->epar0
       << " " << tdr->epar1
       << " " << tdr->epar2
       << endl;

  /* Done */
  cout << "All done." << endl;
  return tdr;
}
