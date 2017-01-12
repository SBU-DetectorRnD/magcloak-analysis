#ifndef PLOT_CONTOUR
#define PLOT_CONTOUR

//plotContour( ROOT::Math::Minimizer* min, unsigned npar, int ndf )
void plotContour( TBackCompFitter *fitter , int npar = 3 )
{

  /* Prepare canvas for drawing */
  Double_t w = 450 * npar;
  Double_t h = 450;
  TCanvas * c_cont = new TCanvas("contour", "contour", w, h);
  c_cont->SetWindowSize(w + (w - c_cont->GetWw()), h + (h - c_cont->GetWh()));
  c_cont->Divide(npar,1);


  /* Select confidence levels (~ sigma intervals) and number of poitns for contour ellipses */
  double cl1 = 0.68;
  double cl2 = 0.95;

  unsigned int npoints = 80;


  /* Select colors for contour ellipses */
  unsigned color_cl1 = 42;
  unsigned color_cl2 = 38;


  /* Collect TGraph's for countours for 1st parameter combination */
  TGraph *gr_xp0_yp1_cl1 = new TGraph( npoints );
  fitter->Contour( 0, 1, gr_xp0_yp1_cl1, cl1);

  TGraph *gr_xp0_yp1_cl2 = new TGraph( npoints );
  fitter->Contour( 0, 1, gr_xp0_yp1_cl2, cl2);


  /* Collect TGraph's for countours for 3rd parameter combination */
  TGraph *gr_xp2_yp1_cl1 = new TGraph( npoints );
  fitter->Contour( 2, 1, gr_xp2_yp1_cl1, cl1);

  TGraph *gr_xp2_yp1_cl2 = new TGraph( npoints );
  fitter->Contour( 2, 1, gr_xp2_yp1_cl2, cl2);


  /* Collect TGraph's for countours for 2nd parameter combination */
  TGraph *gr_xp0_yp2_cl1 = new TGraph( npoints );
  fitter->Contour( 0, 2, gr_xp0_yp2_cl1, cl1);

  TGraph *gr_xp0_yp2_cl2 = new TGraph( npoints );
  fitter->Contour( 0, 2, gr_xp0_yp2_cl2, cl2);


  /* Set graph colors, titles and axis labels */
  gr_xp0_yp1_cl1->SetFillColor(color_cl1);
  gr_xp2_yp1_cl1->SetFillColor(color_cl1);
  gr_xp0_yp2_cl1->SetFillColor(color_cl1);

  gr_xp0_yp1_cl2->SetTitle("");
  gr_xp0_yp1_cl2->GetXaxis()->SetTitle("parameter 0");
  gr_xp0_yp1_cl2->GetYaxis()->SetTitle("parameter 1");
  gr_xp0_yp1_cl2->SetFillColor(color_cl2);

  gr_xp2_yp1_cl2->SetTitle("");
  gr_xp2_yp1_cl2->GetXaxis()->SetTitle("parameter 2");
  gr_xp2_yp1_cl2->GetYaxis()->SetTitle("parameter 1");
  gr_xp2_yp1_cl2->SetFillColor(color_cl2);

  gr_xp0_yp2_cl2->SetTitle("");
  gr_xp0_yp2_cl2->GetXaxis()->SetTitle("parameter 0");
  gr_xp0_yp2_cl2->GetYaxis()->SetTitle("parameter 2");
  gr_xp0_yp2_cl2->SetFillColor(color_cl2);


  /* Plots contours */
  c_cont->cd(1);
  gr_xp0_yp1_cl2->Draw("ALF");
  gr_xp0_yp1_cl1->Draw("LFsame");

  c_cont->cd(2);
  gr_xp2_yp1_cl2->Draw("ALF");
  gr_xp2_yp1_cl1->Draw("LFsame");

  c_cont->cd(3);
  gr_xp0_yp2_cl2->Draw("ALF");
  gr_xp0_yp2_cl1->Draw("LFsame");

  c_cont->Print("new_Contour.png");

return;

}

// /* 3D graph */
// TGraph2D *g012 = new TGraph2D( npoints, &(cont_01_x[0]), &(cont_01_y[0]), &(cont_21_y[0]) );
// TCanvas *c3d = new TCanvas();
// g012->Draw("line");

// /* Draw contour plots */
// gMinuit->SetErrorDef(1); // SetErrorDef(N^2) for N-sigma error
// TGraph *grcont01 = (TGraph*)gMinuit->Contour(80,1,0);
// TGraph *grcont02 = (TGraph*)gMinuit->Contour(80,2,0);
// TGraph *grcont12 = (TGraph*)gMinuit->Contour(80,2,1);

#endif
