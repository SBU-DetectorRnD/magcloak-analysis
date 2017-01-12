/* Call MINUIT minimizer object explicitly */

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TRandom2.h"
#include "TError.h"
#include <iostream>

#include "plotChi2.C"
#include "plotContour.C"
#include "plotResiduals.C"
#include "findErrorBounds.C"

/* global pointer to data for fit */
TGraphErrors* g_data_global = NULL;
TF1* f_fit_global = NULL;

/* Function to minimize for Chi2-Fit */
double Chi2_log(const double *xx )
{
  /* Get const parameters */
  const Double_t par0 = xx[0];
  const Double_t par1 = xx[1];
  const Double_t par2 = xx[2];

  /* Set parameters for fit function */
  f_fit_global->SetParameter(0,par0);
  f_fit_global->SetParameter(1,par1);
  f_fit_global->SetParameter(2,par2);

  return g_data_global->Chisquare(f_fit_global, "R"); // Option R -> use only function range
}

/* Main part of macro */
void fitData( TGraphErrors *gdata , TF1* ffit , double *startval )
{
  cout << "\n***** fitData.C: Starting fitting procedure... *****" << endl;

  g_data_global = gdata;
  f_fit_global = ffit;

  if ( g_data_global == NULL )
  {
    cout << "fitData.C: Did no find any data to fit." << endl;
    return NULL;
  }

  const char * minName = "Minuit2";
  const char *algoName = "Migrad";

  // create minimizer giving a name and a name (optionally) for the specific
  // algorithm
  // possible choices are:
  //     minName                  algoName
  // Minuit /Minuit2             Migrad, Simplex,Combined,Scan  (default is Migrad)
  //  Minuit2                     Fumili2
  //  Fumili
  //  GSLMultiMin                ConjugateFR, ConjugatePR, BFGS,
  //                              BFGS2, SteepestDescent
  //  GSLMultiFit
  //   GSLSimAn
  //   Genetic
  ROOT::Math::Minimizer* min =
  ROOT::Math::Factory::CreateMinimizer(minName, algoName);

  /* Alternatice constructor for Minuit */
  //ROOT::Minuit2::Minuit2Minimizer *min = new ROOT::Minuit2::Minuit2Minimizer();

  //TMinuit *min = new TMinuit();

  // set tolerance , etc...
  min->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2
  min->SetMaxIterations(10000);  // for GSL
  min->SetTolerance(0.001);
  min->SetPrintLevel(1);

  // create funciton wrapper for minmizer
  // a IMultiGenFunction type
  ROOT::Math::Functor f(&Chi2_log,3);
  double step[3] = {0.1,0.1,10};

  // starting point
  double variable[3] = { startval[0],startval[1],startval[2]};

  // set function to minimize (this is the Chi2 function)
  min->SetFunction(f);

  // Set the free variables to be minimized!
  min->SetVariable(0,"x",variable[0], step[0]);
  min->SetVariable(1,"y",variable[1], step[1]);
  min->SetVariable(2,"z",variable[2], step[2]);

  // do the minimization
  min->Minimize();

  /* set function parameters to best fit results */
  const double *xs = min->X();

  f_fit_global->SetParameter(0, xs[0]);
  f_fit_global->SetParameter(1, xs[1]);
  f_fit_global->SetParameter(2, xs[2]);

  /* Print summaries of minimization process */
  std::cout << "Minimum: f(" << xs[0] << ", " << xs[1] << ", " << xs[2] << "): "
  << min->MinValue()  << std::endl;

  // expected minimum is 0
  if ( min->MinValue()  < 1.E-4  && f(xs) < 1.E-4)
  std::cout << "Minimizer " << minName << " - " << algoName
  << "   converged to the right minimum" << std::endl;
  else {
    std::cout << "Minimizer " << minName << " - " << algoName
    << "   failed to converge !!!" << std::endl;
    Error("NumericalMinimization","fail to converge");
  }

  /* Get degrees of freedom NDF */
  unsigned ndf = 0;

  /* get range of function used for fit */
  double fit_min, fit_max;
  f_fit_global->GetRange(fit_min,fit_max);

  /* count data points in range of function */
  for ( int p = 0; p < g_data_global->GetN(); p++ )
  {
    if ( g_data_global->GetX()[p] >= fit_min && g_data_global->GetX()[p] <= fit_max )
    {
      ndf++;
    }
  }
  ndf -= 3; // 3 parameters in fit

  cout << "Degrees of freedom: " << ndf << " --> chi2 / NDF = " << min->MinValue() / ndf << endl;

  /* Get error covariance matrix from minimizer */
  TMatrixD matrix0(3,3);
  for ( unsigned i = 0; i < 3; i++ )
  {
    for ( unsigned j = 0; j < 3; j++ )
    {
      matrix0[i][j] = min->CovMatrix(i,j);
    }
  }
  matrix0.Print();

  /* Get minos error */
  double par0_minos_error_up = 0;
  double par0_minos_error_low = 0;
  double par1_minos_error_up = 0;
  double par1_minos_error_low = 0;
  double par2_minos_error_up = 0;
  double par2_minos_error_low = 0;

  min->GetMinosError( 0, par0_minos_error_low, par0_minos_error_up );
  min->GetMinosError( 1, par1_minos_error_low, par1_minos_error_up );
  min->GetMinosError( 2, par2_minos_error_low, par2_minos_error_up );

  cout << "Minos uncertainty parameter 0: up = " << par0_minos_error_up << ", low = " << par0_minos_error_low << endl;
  cout << "Minos uncertainty parameter 1: up = " << par1_minos_error_up << ", low = " << par1_minos_error_low << endl;
  cout << "Minos uncertainty parameter 2: up = " << par2_minos_error_up << ", low = " << par2_minos_error_low << endl;

  /* Plot chi2 */
  /* sigmas symmetric from covariance matrix */
  //double sigmas[6] = {sqrt(matrix0[0][0]), sqrt(matrix0[0][0]), sqrt(matrix0[1][1]), sqrt(matrix0[1][1]), sqrt(matrix0[2][2]), sqrt(matrix0[2][2])};
  /* sigmas asymmetric from minos */
  double sigmas[6] = {par0_minos_error_low, par0_minos_error_up, par1_minos_error_low, par1_minos_error_up, par2_minos_error_low, par2_minos_error_up};
  plotChi2( g_data_global, f_fit_global, 3, sigmas );

  /* Draw contour plots */
  TGraph2D* gcontour3D = plotContour( min , 3 , ndf );

  /* Find error boundaries */
  TF1* flow = (TF1*)f_fit_global->Clone("flow");
  TF1* fup = (TF1*)f_fit_global->Clone("fup");
  flow->SetRange(0,100000);
  flow->SetLineColor(kBlue);
  flow->SetLineStyle(2);
  fup->SetRange(0,100000);
  fup->SetLineColor(kBlue);
  fup->SetLineStyle(2);
  findErrorBounds( f_fit_global, flow, fup, gcontour3D );

  /* plot residuals data w.r.t. fit */
  plotResiduals( g_data_global, f_fit_global, flow, fup );

  /* Draw data, best fit, and 1-sigma band */
  TCanvas *cfit = new TCanvas();
  g_data_global->Draw("AP");
  //f_fit_global->SetRange(0,4000);
  f_fit_global->Draw("same");
  flow->Draw("same");
  fup->Draw("same");

  cfit->Print("new_FitResult.png");

  /* Fit Extrapolation */
  double t_extrapolate_months = 6;
  double t_extrapolate_s = t_extrapolate_months * 30 * 24 * 60 * 60;
  cout << "Extrapolation to " << t_extrapolate_months
  << " months: " << ffit->Eval( t_extrapolate_s )
  << " , up: " << fup->Eval( t_extrapolate_s ) - ffit->Eval( t_extrapolate_s )
  << " , low: " << flow->Eval( t_extrapolate_s ) - ffit->Eval( t_extrapolate_s )
  << endl;


  return f_fit_global;
}


/* Fragments that still need to be worked into the code above */
// if ( grcont01 )
// {
//   grcont01->GetXaxis()->SetTitle("Parameter 1");
//   grcont01->GetYaxis()->SetTitle("Parameter 0");
//   TCanvas *c_cont1 = new TCanvas();
//   grcont01->SetLineColor(kRed);
//   grcont01->Draw("alp");
// }
//
// if ( grcont02 )
// {
//   grcont02->GetXaxis()->SetTitle("Parameter 2");
//   grcont02->GetYaxis()->SetTitle("Parameter 0");
//   TCanvas *c_cont2 = new TCanvas();
//   grcont02->SetLineColor(kRed);
//   grcont02->Draw("alp");
// }
//
// if ( grcont12 )
// {
//   grcont12->GetXaxis()->SetTitle("Parameter 2");
//   grcont12->GetYaxis()->SetTitle("Parameter 1");
//   TCanvas *c_cont3 = new TCanvas();
//   grcont12->Draw("alp");
//   grcont12->SetLineColor(kRed);
// }
//
// g_log->Fit(fit_log,"ME","",fitrange[0],fitrange[1]);
//
// cout << "Log fit Chi2 = " << fit_log->GetChisquare() <<
// " , NDF = " << fit_log->GetNDF() <<
// " , Prob = " << fit_log->GetProb() << endl;
//
