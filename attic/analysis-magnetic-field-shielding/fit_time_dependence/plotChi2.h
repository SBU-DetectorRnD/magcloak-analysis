#ifndef PLOT_CHISQUARE
#define PLOT_CHISQUARE

/* Plot chisquares */
TCanvas * plotChi2( TGraphErrors* gref, TF1* fref, int npar, double* sigmas )
{
  cout << "fitData.C: Plot chisquare functions" << endl;
  unsigned nsteps = 1000;

  Double_t w = 450 * npar;
  Double_t h = 450;
  TCanvas * c_chi2 = new TCanvas("chi2", "chi2", w, h);
  c_chi2->SetWindowSize(w + (w - c_chi2->GetWw()), h + (h - c_chi2->GetWh()));
  c_chi2->Divide(npar,1);

  TF1* flocal;

  for ( int par = 0; par < npar; par++ )
  {
    c_chi2->cd(par+1);

    flocal = (TF1*)fref->Clone("flocal");
    flocal->SetParameter(0,fref->GetParameter(0));
    flocal->SetParameter(1,fref->GetParameter(1));
    flocal->SetParameter(2,fref->GetParameter(2));

    double step = 10. * sigmas[par*2] / nsteps;
    double x0 = flocal->GetParameter(par) - (nsteps / 2.) * step;

    TLine *l_fit = new TLine( flocal->GetParameter(par), 0, flocal->GetParameter(par), 1000000000);
    TLine *l_fit_p1s = new TLine( flocal->GetParameter(par) + sigmas[par*2+1], 0, flocal->GetParameter(par) + sigmas[par*2+1], 1000000000);
    TLine *l_fit_m1s = new TLine( flocal->GetParameter(par) + sigmas[par*2], 0, flocal->GetParameter(par) + sigmas[par*2], 1000000000);

    l_fit->SetLineColor(kGreen);
    l_fit_p1s->SetLineColor(kBlue);
    l_fit_m1s->SetLineColor(kBlue);

    vector<double> vx;
    vector<double> vy;

    for ( int i = 0; i < nsteps; i++ )
    {
      double xtest = x0 + i * step;
      flocal->SetParameter(par,xtest);
      vx.push_back( xtest );
      vy.push_back( gref->Chisquare(flocal) );
    }

    TGraph* chi2 = new TGraph(vx.size(), &(vx[0]), &(vy[0]));
    TString xaxisname("parameter ");
    xaxisname+=par;
    chi2->GetXaxis()->SetTitle(xaxisname);
    chi2->GetYaxis()->SetTitle("#chi^{2}");
    chi2->SetTitle("");

    chi2->Draw("AL");
    l_fit->Draw();
    l_fit_p1s->Draw();
    l_fit_m1s->Draw();
  }

  c_chi2->Print("new_Chisquare.png");

  return c_chi2;

}

#endif
