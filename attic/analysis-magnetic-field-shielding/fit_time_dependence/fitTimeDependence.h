#ifndef FIT_TIME_DEPENDENCE
#define FIT_TIME_DEPENDENCE

#include "plotContour.h"
#include "plotChi2.h"
#include "plotResiduals.h"
#include "findSigmaBand.h"

#include "TF1EvalWithError.h"

#include "TimeDependenceResult.h"

TimeDependenceResult* fitTimeDependence( TGraphErrors* gsmooth, bool b_lin2log_x, double fitrange_min, double fitrange_max )
{
    cout << "Start fitting time dependence:" << endl;

    /* Choose fit function */
    /* NOTE: log(m/n) = log(m) - log(n) */
    /* NOTE: using 'x+1' instead of 'x' can help avoid divergence */
    //TF1* ffit = new TF1("ffit","[0]*log( (x+1+[2]) / [1])"); // -> trouble with contour plots
    //TF1* ffit = new TF1("ffit","[0]*log( (x+[2]) ) + [1]"); // -> nicer contour plots!
    //TF1* ffit = new TF1("ffit","log( [2]*x*x + [1]*x + [0] )");
    TF1* ffit = new TF1("ffit","log(x+[0]) + log(x+[1]) + [2]");
    //TF1* ffit = new TF1("ffit","[0]*log(x) + [1]*log(x) + [2]");
    //TF1* ffit = new TF1("ffit","[0] * ( 1 - exp( - x * [1] ) ) + [2]");


    if ( b_lin2log_x == true  )
    {
      ffit = new TF1("ffit","[0]*x*x+[1]*x+[2]");
    }

    /* Choose fit range */
    ffit->SetRange(fitrange_min,fitrange_max); // only this range used for fitting

   /* Fit function to data */
   //  ffit->SetParameters(5, 0.01, 10);
   //ffit->SetParameters(10, 10, 10);

    // R - use only function range
    // S - return TFitResultPtr
    // M -
    // E - use Minos for error erstimate
    // V - Verbose mode
    // N - Do not store graphics funtion- do not draw
    //gsmooth->Fit(ffit,"RSMEN");
    TFitResultPtr fitres = gsmooth->Fit(ffit,"RSMEN");

    TBackCompFitter *fitter = (TBackCompFitter *) TVirtualFitter::GetFitter();
    plotContour( fitter );

    if ( !fitres )
    {
      cout << "ERROR: No fit result!" << endl;
      return NULL;
    }

    /* Plot chisquares */
    double sigmas[6] = {
      fitres->LowerError(0), fitres->UpperError(0),
      fitres->LowerError(1), fitres->UpperError(1),
      fitres->LowerError(2), fitres->UpperError(2),
    };
    plotChi2( gsmooth, ffit, 3, sigmas );

    /* Print chisquare / NDF */
    cout << endl;
    cout << "*** Using MinimizerType " << fitres->MinimizerType() << endl;
    cout << "*** Degrees of Freedom 1: " << fitres->Ndf() << endl;
    cout << "*** Degrees of Freedom 2: " << ffit->GetNDF() << endl;
    cout << "*** Chisquare:          " << ffit->GetChisquare() << endl;
    cout << "*** Chisquare / DOF     " << ffit->GetChisquare() / ffit->GetNDF() << endl;
    cout << "*** Probability:        " << ffit->GetProb() << endl;
    cout << endl;

    /* Print covariance matrix */
    fitres->PrintCovMatrix(cout);
    cout << endl;

    /* Find confidence intervals = error bands */
    TGraphErrors* gconfidence = findSigmaBand( fitter, gsmooth );

    /* plot residuals data w.r.t. fit */
    plotResiduals( gsmooth, ffit, gconfidence );

  /* Extrapolate fit result with uncertainty */
  double x0 = 3600;
  double Bex0 = 0;
  double BexLow = 0;
  double BexUp = 0;

  TF1EvalWithError( fitter,  x0, Bex0, BexLow, BexUp );

  cout << endl;
  cout << "*** Extrapolated value at x0 = " << x0 << ": " << Bex0 << " minus " << BexLow << " plus " << BexUp << endl;
  cout << endl;


  /* Prepare TimeDependenceResult object to return */
  TimeDependenceResult *tdr = new TimeDependenceResult();
  tdr->chi2 = ffit->GetChisquare();
  tdr->ndf  = ffit->GetNDF();
  tdr->prob = ffit->GetProb();

  tdr->par0 = ffit->GetParameter(0);
  tdr->par1 = ffit->GetParameter(1);
  tdr->par2 = ffit->GetParameter(2);

  tdr->epar0 = ffit->GetParError(0);
  tdr->epar1 = ffit->GetParError(1);
  tdr->epar2 = ffit->GetParError(2);

  tdr->extrap_t0 = x0;
  tdr->extrap_field = Bex0;
  tdr->extrap_field_err_pos = BexUp;
  tdr->extrap_field_err_neg = BexLow;


  /* Done */
  cout << "Fitting time dependence was successful." << endl;
  return tdr;
}

#endif
