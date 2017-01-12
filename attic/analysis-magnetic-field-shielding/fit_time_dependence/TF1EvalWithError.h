#ifndef TF1_EVAL_WITH_ERROR
#define TF1_EVAL_WITH_ERROR

//int TF1EvalWithError( TF1* f,  double x0, double& val,double &val_err_low, double &val_err_up, TMatrixDSym* cov=NULL )
int TF1EvalWithError( TBackCompFitter *fitter, double x0, double& val,double &val_err_low, double &val_err_up )
{
  TGraphErrors *gconfidence = new TGraphErrors(1);
  gconfidence->SetPoint(0, x0, 0);

  fitter->GetConfidenceIntervals(gconfidence, 0.68);

  val = gconfidence->GetY()[0];
  val_err_low = gconfidence->GetEY()[0];
  val_err_up = gconfidence->GetEY()[0];

  return 0;
}

#endif
