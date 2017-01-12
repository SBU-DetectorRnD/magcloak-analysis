#ifndef TIME_DEPENDENCE_RESULT
#define TIME_DEPENDENCE_RESULT

struct TimeDependenceResult {
  // TFitResultPtr ?? Clone?
  TString datafile;

  double current;

  double chi2;
  double ndf;
  double prob;

  double par0;
  double par1;
  double par2;

  double epar0;
  double epar1;
  double epar2;

  double extrap_t0;
  double extrap_field;
  double extrap_field_err_pos;
  double extrap_field_err_neg;
};

#endif
