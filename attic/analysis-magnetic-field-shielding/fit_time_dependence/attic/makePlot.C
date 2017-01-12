TCanvas * plot_fit( double xmin, double xmax, double ymin, double ymax,
  TGraphErrors* g_data, TF1* f_fit, TF1* f_fit_psig, TF1* f_fit_msig )
  {
    /* Graph frames for plotting */
    TH1F* h_log = new TH1F("h_log", "", 100,xmin,xmax);
    //h_log->GetYaxis()->SetRangeUser(95,145);
    h_log->GetYaxis()->SetRangeUser(ymin,ymax);
    h_log->SetLineColor(kWhite);
    h_log->GetXaxis()->SetTitle("time (s)");
    h_log->GetYaxis()->SetTitle("field (mT)");

    /* Draw results */
    TCanvas *c_log = new TCanvas();
    h_log->Draw();

    g_data->Draw("sameP");

    f_fit->Draw("same");
    f_fit_psig->Draw("same");
    f_fit_msig->Draw("same");

    return c_log;
  }
