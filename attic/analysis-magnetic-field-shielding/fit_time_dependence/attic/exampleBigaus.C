double bigaus_function(double *x, double *p) { 
      double u = (x[0]-p[1])/p[2];
      double v = (x[1]-p[3])/p[4];
      double rho = p[5];
      double c = 1. - rho*rho;
      double z = u*u - 2.*rho*u*v + v*v;
      return  p[0]/(2 * TMath::Pi() * p[2] * p[4] * std::sqrt(c) )* std::exp(- z / (2. * c) );
}

void exampleBigaus() { 

   int n = 100000;
   int nbins = 50;
   double xmin = -10; 
   double xmax = 10; 
   TH2 * h2 = new TH2D("h1","h2",nbins,xmin,xmax, nbins, xmin, xmax);
   // for the ROOT master and  6.07.06
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,7,6)   
   TF2 * f2 = new TF2("bigaus","bigaus");
#else
   // for older ROOT version
   TF2 * f2 = new TF2("bigaus",bigaus_function,xmin,xmax,xmin,xmax,6);
#endif

   f2->SetParameters(1,2,2,-1,3,0.3);

   h2->FillRandom("bigaus"); 

   f2->SetParameters(100,2,2,-1,3,0.3);

   h2->Fit("bigaus"); 

   h2->Draw("COLZ"); 

   // draw contours of fit result for sigmax and sigmay

   auto fitter = (TBackCompFitter *) TVirtualFitter::GetFitter();
   int par1 = 2; // index of sigmax
   int par2 = 4; // index of sigmax
   // make contour at desired CL
   double cl1 = 0.68;
   // make a contour with 80 points
   auto gr1 = new TGraph( 80 );
   fitter->Contour( par1, par2, gr1, cl1);
   new TCanvas(); 
   gr1->SetFillColor(42);
   auto gr2 = new TGraph( 80 );
   double cl2 = 0.95;
   fitter->Contour( par1, par2, gr2, cl2);
   gr2->SetFillColor(38);
   gr2->Draw("ALF"); 
   gr1->Draw("LF");
  

}
