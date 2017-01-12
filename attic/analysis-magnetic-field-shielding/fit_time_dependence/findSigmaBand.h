#ifndef FIND_SIGMA_BAND
#define FIND_SIGMA_BAND

#include "TF1EvalWithError.h"

//TGraphErrors* findSigmaBand( TF1* f, TMatrixDSym* cov=NULL )
TGraphErrors* findSigmaBand( TBackCompFitter *fitter, TGraphErrors *gsmooth )
{
  //Create a TGraphErrors to hold the confidence intervals
  int npoints = gsmooth->GetN();

  TGraphErrors *gconfidence = new TGraphErrors(npoints);
  gconfidence->SetTitle("Fitted line with .95 conf. band");
  for (i=0; i<npoints; i++)
    gconfidence->SetPoint(i, gsmooth->GetX()[i], 0);
  //Compute the confidence intervals at the x points of the created graph
  fitter->GetConfidenceIntervals(gconfidence, 0.68);
  //Now the "gconfidence" graph contains function values as its y-coordinates
  //and confidence intervals as the errors on these coordinates
  //Draw the graph, the function and the confidence intervals

  TCanvas *cconf = new TCanvas("Confidence Interval");
  gsmooth->Draw("ap");
  gconfidence->SetFillColor(kRed);
  gconfidence->Draw("3alsame");
  gsmooth->Draw("psame");

return gconfidence;

}


//  TGraphErrors *gband = new TGraphErrors(1);
// double t_ref = 1;
// double B_max = fbest->Eval( t_ref );
// double B_min = fbest->Eval( t_ref );
//
// double par1_max = fbest->GetParameter(1);
// double par1_min = fbest->GetParameter(1);
//
// TF1* fit_temp = (TF1*)fbest->Clone("fit_temp");
//
// for ( int i = 0; i < gcontour3D->GetN(); i++ )
// {
//   fit_temp->SetParameter(0, gcontour3D->GetY()[i]);
//   fit_temp->SetParameter(1, gcontour3D->GetX()[i]);
//   fit_temp->SetParameter(2, gcontour3D->GetZ()[i]);
//
// //  cout << "f(" << gcontour3D->GetY()[i] << " , " << gcontour3D->GetX()[i] << " , " << gcontour3D->GetZ()[i] << ") = " << fit_temp->Eval(t_ref) << endl;
//
// if ( fit_temp->Eval( t_ref ) > B_max )
// //if ( gcontour3D->GetX()[i] > par1_max )
//   {
//     fup->SetParameter(0, gcontour3D->GetY()[i]);
//     fup->SetParameter(1, gcontour3D->GetX()[i]);
//     fup->SetParameter(2, gcontour3D->GetZ()[i]);
//
//     par1_max = gcontour3D->GetX()[i];
//     B_max = fit_temp->Eval( t_ref );
//   }
//
// if ( fit_temp->Eval( t_ref ) < B_min )
// //if ( gcontour3D->GetX()[i] < par1_min )
//   {
//     flow->SetParameter(0, gcontour3D->GetY()[i]);
//     flow->SetParameter(1, gcontour3D->GetX()[i]);
//     flow->SetParameter(2, gcontour3D->GetZ()[i]);
//
//     par1_min = gcontour3D->GetX()[i];
//     B_min = fit_temp->Eval( t_ref );
//   }
//
// }
// cout << "Par1_min: " << par1_min << " , Par1_max: " << par1_max << endl;
// cout << "Bmin: " << B_min << " , Bmax: " << B_max << endl;
//
// fit_temp->Delete();

#endif
