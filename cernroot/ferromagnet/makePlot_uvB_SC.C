/*==========Permeability Plotting Macro===========
 * Project: A Compact Magnetic Field Cloaking Device
 * Author: Thomas Krahulik
 * Date Updated: October 20, 2016
 * Purpose: To plot relative magnetic permeability
 *          of ferromagnets as a function of
 *          external magnetic field
 * To run macro: root -l makePlot_uvB.C
 =================================================*/
/*
 * Quantify Uncertainty
 */
double ratio(const char* f_inner, const char* f_outer)
{
  TTree *TDiIn = new TTree();
  TDiIn->ReadFile(f_inner, "di/D");
  TH1D *h_din = new TH1D("h_din", "", 10000, 0, 100);
  TDiIn->Draw("di>>h_din", "", "goff");
  Double_t d_inner = h_din->GetMean(1);
  //  Double_t sig_inner = h_din->GetMeanError(1);
  //  cout << "Mean Inner Diameter: " << d_inner  << " +/- " << sig_inner << endl;

  //  cout<< "Reading Outer Diameters..." << f_outer <<endl;
  TTree *TDiOut = new TTree();
  TDiOut->ReadFile(f_outer, "do/D");
  TH1D *h_dout = new TH1D("h_dout", "", 10000, 0, 100);
  TDiOut->Draw("do>>h_dout", "", "goff");
  Double_t d_outer = h_dout->GetMean(1);
  //  Double_t sig_outer = h_dout->GetMeanError(1);
  //  cout << "Mean Outer Diameter: " << d_outer << " +/- " << sig_outer << endl;

  double Ratio = d_inner/d_outer;
  h_din->Delete();
  h_dout->Delete();
  return Ratio;
}

double r_sig(const char* f_inner, const char* f_outer)
{
  TTree *TDiIn = new TTree();
  TDiIn->ReadFile(f_inner, "di/D");
  TH1D *h_din = new TH1D("h_din", "", 10000, 0, 100);
  TDiIn->Draw("di>>h_din", "", "goff");
  Double_t d_inner = h_din->GetMean(1);
  Double_t sig_inner = h_din->GetMeanError(1);
  //  cout << "Mean Inner Diameter: " << d_inner  << " +/- " << sig_inner << endl;

  //  cout<< "Reading Outer Diameters..." << f_outer <<endl;
  TTree *TDiOut = new TTree();
  TDiOut->ReadFile(f_outer, "do/D");
  TH1D *h_dout = new TH1D("h_dout", "", 10000, 0, 100);
  TDiOut->Draw("do>>h_dout", "", "goff");
  Double_t d_outer = h_dout->GetMean(1);
  Double_t sig_outer = h_dout->GetMeanError(1);
  //  cout << "Mean Outer Diameter: " << d_outer << " +/- " << sig_outer << endl;

  double Ratio = d_inner/d_outer;
  double Sigma = Ratio*sqrt( pow((sig_inner/d_inner),2) + pow((sig_outer/d_outer),2)  );
  //  cout << Sigma << endl;
  h_din->Delete();
  h_dout->Delete();

  return Sigma;
}

/* =====================================
 * Calibration Measurements and Analysis
   ===================================== */
/*
 * The Calibration function reads in the
 * calibration file and returns a graph
 * that is used to find the relation
 * between current and magentic field
 * for the Helmholtz coil.
 */
TF1* Calibrate(
		    const char* f_calib
		    )
{
  /*Read in Calibration File*/
  cout<< "processing file " << f_calib <<endl;
  TTree *TCalib = new TTree();
  TCalib->ReadFile(f_calib, "t/D:I:B");
  int n = TCalib->Draw("I:TMath::Abs(B)", "", "goff");
  TGraph *g_calib = new TGraph(n, TCalib->GetV1(), TCalib->GetV2() );
  g_calib->SetTitle("");
  // g_calib->Draw("AP");
  g_calib->Fit("pol1", "q");
  TF1* calib_fit = g_calib->GetFunction("pol1");

  return calib_fit;

}

/* =====================================
 *      Ferromagnet Scans Analysis
   ===================================== */
/*
 * The Ferromagnet_Scan function reads in
 * a data file of magentic field scans
 * from within the ferromagnet and returns
 * a graph of the magnetic permeability
 * of the ferromagnet vs the external field
 * provided by the Helmholtz coil.
 */
TGraphErrors* plot_uvB(
			 const TString scan_file,
			 TF1* calib_fit,
			 double R,
			 double R_sig
)
{

  /*Read in Data File to ROOT Tree*/
  cout<< "processing file " << scan_file <<endl;
  TTree *TData = new TTree();
  TData->ReadFile(scan_file, "t/D:I:B");
  /*Use TTree Draw command to write branches to usable vector*/
  int n = TData->Draw("I:TMath::Abs(B):0.0:0.005", "", "goff");

  vector<double> B_ext, B_in, Bratio, u, u_err;
  for(int i = 0; i < n; i++)
    {
      B_ext.push_back( calib_fit->Eval(TData->GetV1()[i]) );
      B_in.push_back( TData->GetV2()[i] );
      Bratio.push_back( B_in[i] / B_ext[i]);
      u.push_back( (Bratio[i]*(R**2) + Bratio[i] - 2 - 2*sqrt((Bratio[i]**2)*(R**2) - Bratio[i]*(R**2) - Bratio[i] + 1 ) ) / (Bratio[i]*(R**2) - Bratio[i]) );
      u_err.push_back( u[i]*( 5.5*((0.01/Bratio[i])**2.) + 10*((R_sig/R)**2.)  )**0.5 );
    }

  TGraphErrors *g_uvB = new TGraphErrors(n, &B_ext[0], &u[0], TData->GetV3(), &u_err[0]);

  return g_uvB;
}

/* ====================================
 * Plot Magnetic Permeability vs Field
   ====================================*/
void makePlot_uvB_SC()
{


  const bool ideal_cloak = false;

  const bool plot_SC_45l = true;
  const bool plot_SC_10l = true;
  const bool plot_SC_3l = true;
  const bool plot_SC_2l = false;
  const bool plot_SC_1l = true;

  TCanvas *c_uvB = new TCanvas();
  TH1 *h_uvB = c_uvB->DrawFrame(0, 1.0, 60, 25.);
  h_uvB->SetTitle("Permeability of Superconductor Samples");
  h_uvB->GetXaxis()->SetTitle("B_{0} (mT)");
  h_uvB->GetXaxis()->SetTitleOffset(1.5);
  h_uvB->GetYaxis()->SetTitle("#mu_{r}");
  h_uvB->GetYaxis()->SetTitleOffset(1.0);
  /*Create Legend*/
  TLegend *leg_uvB = new TLegend(0.55,0.65,0.8,0.87);
  leg_uvB->SetNColumns(1);
  leg_uvB->Draw();

  vector<double> u, L, u_err, L_err;

  /*Include Point for Fm = 0.0, No Steel Powder*/
  u.push_back(1.0);
  L.push_back(0.0);
  u_err.push_back(0.01);
  L_err.push_back(0.0);

  /*Include Point for Fm = 1.0, Only Steel Powder*/
  // u.push_back(1000.);
  // Fm.push_back(1.0);
  // u_err.push_back(0.0);
  // Fm_err.push_back(0.0);

  if(plot_SC_45l)
    {
      /*
       * Permeability Plot for 45 Layer Superconductor
       */
      /*Run calibration*/
      const TString calib_sc45l = "../Data/Calib_Data/DataFile_161027_162816.txt";
      const TString fmscan_sc45l = "../Data/FMScan_Data/DataFile_161027_163959.txt";
      TF1 *calib_fit1 = Calibrate(calib_sc45l);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/Inner_Diameter_45.txt";
      const TString do_file = "../Data/Calib_Data/Outer_Diameter_45.txt";
      double R_sc45l = ratio(di_file, do_file);
      double R_sig_sc45l = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_sc45l = plot_uvB(fmscan_sc45l, calib_fit1, R_sc45l, R_sig_sc45l);
      g_sc45l->Draw("LP"); 
      g_sc45l->SetLineColor(kRed);
      g_sc45l->SetMarkerColor(kRed);
      leg_uvB->AddEntry( g_sc45l , "45 Layer SC" , "lp");

      u.push_back( g_sc45l->Eval(40., 0, "") );
      L.push_back( 45.0 );
      u_err.push_back( 1.10 );
      L_err.push_back( 0.0 );
    }

  if(plot_SC_10l)
    {
      /*
       * Permeability Plot for 10 Layer Superconductor
       */
      /*Run calibration*/
      const TString calib_sc10l = "../Data/Calib_Data/DataFile_161027_162816.txt";
      const TString fmscan_sc10l = "../Data/FMScan_Data/DataFile_161027_165756.txt";
      TF1 *calib_fit1 = Calibrate(calib_sc10l);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/Inner_Diameter_10.txt";
      const TString do_file = "../Data/Calib_Data/Outer_Diameter_10.txt";
      double R_sc10l = ratio(di_file, do_file);
      double R_sig_sc10l = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_sc10l = plot_uvB(fmscan_sc10l, calib_fit1, R_sc10l, R_sig_sc10l);
      g_sc10l->Draw("LP"); 
      g_sc10l->SetLineColor(kOrange+7);
      g_sc10l->SetMarkerColor(kOrange+7);
      leg_uvB->AddEntry( g_sc10l , "10 Layer SC" , "lp");

      u.push_back( g_sc10l->Eval(40., 0, "") );
      L.push_back( 10.0 );
      u_err.push_back( 0.30 );
      L_err.push_back( 0.0 );
    }

  if(plot_SC_3l)
    {
      /*
       * Permeability Plot for 3 Layer Superconductor
       */
      /*Run calibration*/
      const TString calib_sc3l = "../Data/Calib_Data/DataFile_161027_162816.txt";
      const TString fmscan_sc3l = "../Data/FMScan_Data/DataFile_161027_171217.txt";
      TF1 *calib_fit1 = Calibrate(calib_sc3l);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/Inner_Diameter_3.txt";
      const TString do_file = "../Data/Calib_Data/Outer_Diameter_3.txt";
      double R_sc3l = ratio(di_file, do_file);
      double R_sig_sc3l = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_sc3l = plot_uvB(fmscan_sc3l, calib_fit1, R_sc3l, R_sig_sc3l);
      g_sc3l->Draw("LP"); 
      g_sc3l->SetLineColor(kGreen+3);
      g_sc3l->SetMarkerColor(kGreen+3);
      leg_uvB->AddEntry( g_sc3l , "3 Layer SC" , "lp");

      u.push_back( g_sc3l->Eval(40., 0, "") );
      L.push_back( 3.0 );
      u_err.push_back( 0.15 );
      L_err.push_back( 0.0 );
    }

  if(plot_SC_2l)
    {
      /*
       * Permeability Plot for 2 Layer Superconductor
       */
      /*Run calibration*/
      const TString calib_sc2l = "../Data/Calib_Data/DataFile_161027_162816.txt";
      const TString fmscan_sc2l = "../Data/FMScan_Data/DataFile_161027_172429.txt";
      TF1 *calib_fit1 = Calibrate(calib_sc2l);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/Inner_Diameter_2.txt";
      const TString do_file = "../Data/Calib_Data/Outer_Diameter_2.txt";
      double R_sc2l = ratio(di_file, do_file);
      double R_sig_sc2l = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_sc2l = plot_uvB(fmscan_sc2l, calib_fit1, R_sc2l, R_sig_sc2l);
      g_sc2l->Draw("LP"); 
      g_sc2l->SetLineColor(kBlue);
      g_sc2l->SetMarkerColor(kBlue);
      leg_uvB->AddEntry( g_sc2l , "2 Layer SC" , "lp");

      u.push_back( g_sc2l->Eval(40., 0, "") );
      L.push_back( 2.0 );
      u_err.push_back( 0.05 );
      L_err.push_back( 0.0 );
    }

  if(plot_SC_1l)
    {
      /*
       * Permeability Plot for 1 Layer Superconductor
       */
      /*Run calibration*/
      const TString calib_sc1l = "../Data/Calib_Data/DataFile_161027_162816.txt";
      const TString fmscan_sc1l = "../Data/FMScan_Data/DataFile_161027_173957.txt";
      TF1 *calib_fit1 = Calibrate(calib_sc1l);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/Inner_Diameter_1.txt";
      const TString do_file = "../Data/Calib_Data/Outer_Diameter_1.txt";
      double R_sc1l = ratio(di_file, do_file);
      double R_sig_sc1l = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_sc1l = plot_uvB(fmscan_sc1l, calib_fit1, R_sc1l, R_sig_sc1l);
      g_sc1l->Draw("LP"); 
      g_sc1l->SetLineColor(kViolet);
      g_sc1l->SetMarkerColor(kViolet);
      leg_uvB->AddEntry( g_sc1l , "1 Layer SC" , "lp");

      u.push_back( g_sc1l->Eval(40., 0, "") );
      L.push_back( 1.0 );
      u_err.push_back( 0.04 );
      L_err.push_back( 0.0 );
    }


  if(ideal_cloak)
    {
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/fm612_di.txt";
      const TString do_file = "../Data/Calib_Data/fm612_do.txt";
      double R_fv30 = ratio(di_file, do_file);
      // cout << R_fv30 << endl;
      double R_sig_fv30 = r_sig(di_file, do_file);
      // cout << R_sig_fv30 << endl;
      /*Calculate theoretical permeability of ferromagnet*/
      double u_cloak = (1. + pow(R_fv30,2.)) / (1. - pow(R_fv30,2.));
      cout << "Desired Permeability: " << u_cloak << endl;
      /*Draw Line for Permeability of Ideal Cloak*/
      TLine *l_ucloak = new TLine(0.0, u_cloak, 60.0, u_cloak);
      l_ucloak->SetLineStyle(2);
      l_ucloak->Draw();
      leg_uvB->AddEntry( l_ucloak , "Ideal Cloak" , "l");
    }

  TGraphErrors *g_uvL = new TGraphErrors(L.size(), &(L[0]), &(u[0]), &(L_err[0]), &(u_err[0]) );
  TCanvas *c_uvL = new TCanvas();
  g_uvL->Draw("AP");

  TF1 *fit = new TF1("fit", "[0]*log([1]*x + [2])", 0.0, 50.0);
  fit->SetParameters(1., 1., 1.);
  // TF1 *fit = new TF1("fit", "pol1", 0.0, 50.0);
  // fit->SetParameters(1., 1.);
  g_uvL->Fit("fit");

  cout << fit->GetChisquare() << endl;
  cout << fit->GetNDF() << endl;
  cout << fit->GetChisquare() / fit->GetNDF() << endl;
  /*Save plots to png and eps files*/     
      
  // c_uvB->Print("../../Plots/uvB/EpoxySteel_uvB_SC.png");
  // c_uvL->Print("../../Plots/uvB/EpoxySteel_uvL_SC.png");
  
  return;
}
