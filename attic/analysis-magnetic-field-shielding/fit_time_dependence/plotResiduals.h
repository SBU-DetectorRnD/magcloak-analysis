#ifndef PLOT_RESIDUALS
#define PLOT_RESIDUALS

/* Plot residuals data w.r.t. fit result */
TCanvas * plotResiduals( TGraphErrors* gref, TF1* fref, TGraphErrors* gconfidence )
{
  cout << "plotResiduals.C: Plot residuals" << endl;

  Double_t w = 500;
  Double_t h = 800;
  TCanvas * c_res = new TCanvas("residuals", "residuals", w, h);
  c_res->SetWindowSize(w + (w - c_res->GetWw()), h + (h - c_res->GetWh()));
  c_res->Divide(1,2);

  TGraphErrors *gresidual = new TGraphErrors( gref->GetN() );

  for ( int p = 0; p < gref->GetN(); p++ )
  {
    gresidual->SetPoint(p, gref->GetX()[p], gref->GetY()[p] - fref->Eval( gref->GetX()[p] ));
    gresidual->SetPointError(p, gref->GetErrorX(p), gref->GetErrorY(p) );
  }

  /* difference 'best fit' and sigma bounds */
  int npoints = gconfidence->GetN();

  TGraphErrors *gconfidence_residual = new TGraphErrors(npoints);
  for (i=0; i<npoints; i++)
  {
      gconfidence_residual->SetPoint(i, gconfidence->GetX()[i], 0);
      gconfidence_residual->SetPointError(i, 0, gconfidence->GetEY()[i]);
  }
  gconfidence_residual->SetFillColor(kGray+1);

  /* Plot */
  c_res->cd(1);
  gref->Draw("AP");
  gconfidence->SetFillColor(kGray+1);
  gconfidence->Draw("3alsame");
  gref->Draw("psame");
  fref->Draw("same");

  c_res->cd(2);
  gresidual->GetXaxis()->SetTitle("t (s)");
  gresidual->GetYaxis()->SetTitle("data - fit");
  gresidual->SetTitle("");

  gresidual->Draw("AP");
  gconfidence_residual->Draw("3alsame");
  gresidual->Draw("Psame");

  TLine *lzero = new TLine(0,0,36000,0);
  lzero->SetLineColor(kGreen+2);
  lzero->Draw("same");

  c_res->Print("new_Residual.png");

  return c_res;

}

#endif
