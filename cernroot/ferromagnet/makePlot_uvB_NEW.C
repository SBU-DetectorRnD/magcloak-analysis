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
double calc_R(const char* f_inner, const char* f_outer)
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

double calc_Rerr(const char* f_inner, const char* f_outer)
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

vector<double> calc_u(
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

  return u;
}

vector<double> calc_uerr(
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

  return u_err;
}

/* ====================================
 * Plot Magnetic Permeability vs Field
   ====================================*/
void makePlot_uvB_NEW()
{


  const bool ideal_cloak = false;

  const bool plot_FM_Fm104 = false;
  const bool plot_FM_Fm199 = true;
  const bool plot_FM_Fm503 = false;
  const bool plot_FM_Fm548 = false;
  const bool plot_FM_Fm554 = false;
  const bool plot_FM_Fm574 = false;
  const bool plot_FM_Fm602 = false;
  const bool plot_FM_Fm612 = false;
  const bool plot_FM_Fm625 = false;
  const bool plot_FM_Fm651 = false;
  const bool plot_FM_Fm673 = false;


  TCanvas *c_uvB = new TCanvas();
  TH1 *h_uvB = c_uvB->DrawFrame(0, 1.0, 60, 4.5);
  h_uvB->SetTitle("Permeability of Epoxy/Steel Ferromagnets");
  h_uvB->GetXaxis()->SetTitle("B_{0} (mT)");
  h_uvB->GetXaxis()->SetTitleOffset(1.5);
  h_uvB->GetYaxis()->SetTitle("#mu_{r}");
  h_uvB->GetYaxis()->SetTitleOffset(1.0);
  /*Create Legend*/
  TLegend *leg_uvB = new TLegend(0.55,0.65,0.8,0.87);
  leg_uvB->SetNColumns(1);
  leg_uvB->Draw();

  vector<double> u, Fm, u_err, Fm_err;

  /*Include Point for Fm = 0.0, No Steel Powder*/
  u.push_back(1.0);
  Fm.push_back(0.0);
  u_err.push_back(0.08);
  Fm_err.push_back(0.0);

  /*Include Point for Fm = 1.0, Only Steel Powder*/
  // u.push_back(1000.);
  // Fm.push_back(1.0);
  // u_err.push_back(0.0);
  // Fm_err.push_back(0.0);

  if(plot_FM_Fm104)
    {
      /*
       * Permeability Plot for FM = 0.104 Ferromagnet
       */
      /*Run calibration*/
      const TString calib_fm104 = "../Data/Calib_Data/DataFile.txt";
      const TString fmscan_fm104 = "../Data/FMScan_Data/DataFile.txt";
      TF1 *calib_fit1 = Calibrate(calib_fm104);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/fm503_di.txt";
      const TString do_file = "../Data/Calib_Data/fm503_do.txt";
      double R_fm104 = ratio(di_file, do_file);
      double R_sig_fm104 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm104 = plot_uvB(fmscan_fm104, calib_fit1, R_fm104, R_sig_fm104);
      g_fm104->Draw("LP"); 
      g_fm104->SetLineColor(kRed);
      g_fm104->SetMarkerColor(kRed);
      leg_uvB->AddEntry( g_fm104 , "F_{m} = 0.104" , "lp");

      u.push_back( g_fm104->Eval(40., 0, "") );
      Fm.push_back( 0.104 );
      u_err.push_back( 0.08 );
      Fm_err.push_back( 0.0 );
    }

  if(plot_FM_Fm199)
    {
      /*
       * Permeability Plot for FM = 0.199 Ferromagnet
       */
      /*Run calibration*/
      const TString fCalb_fm199 = "../Data/Calib_Data/DataFile_161007_194516.txt";
      const TString fPerm_fm199 = "../Data/FMScan_Data/DataFile_161007_205423.txt";
      TF1 *fitCalb_fm199 = Calibrate(fCalb_fm199);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString fDinn_fm199 = "../Data/Calib_Data/fm503_di.txt";
      const TString fDout_fm199 = "../Data/Calib_Data/fm503_do.txt";
      double R_fm199 = calc_R(fDinn_fm199, fDout_fm199);
      double eR_fm199 = calc_Rerr(fDinn_fm199, fDout_fm199);
      vector<double> u_fm199 = calc_u(fPerm_fm199, fitCalb_fm199, R_fm199, eR_fm199);
      vector<double> eu_fm199 = calc_uerr(fPerm_fm199, fitCalb_fm199, R_fm199, eR_fm199);
      /*Plot u vs B for FM*/
      TGraphErrors *gPerm_fm199 = plot_uvB(fPerm_fm199, fitCalb_fm199, R_fm199, eR_fm199);
      // TGraphErrors *gPerm_fm199 = plot_uvB(fPerm_fm199, fitCalb_fm199, R_fm199, eR_fm199);
      gPerm_fm199->Draw("LP"); 
      gPerm_fm199->SetLineColor(kRed);
      gPerm_fm199->SetMarkerColor(kRed);
      leg_uvB->AddEntry( gPerm_fm199 , "F_{m} = 0.199" , "lp");

      u.push_back( gPerm_fm199->Eval(40., 0, "") );
      Fm.push_back( 0.199 );
      u_err.push_back( 0.08 );
      Fm_err.push_back( 0.0 );
    }

  if(plot_FM_Fm503)
    {
      /*
       * Permeability Plot for FM = 0.503 Ferromagnet
       */
      /*Run calibration*/
      // const TString calib_fm503 = "../Data/Calib_Data/DataFile_160929_173133.txt";
      // const TString fmscan_fm503 = "../Data/FMScan_Data/DataFile_160929_174418.txt";
      const TString calib_fm503 = "../Data/Calib_Data/DataFile_161006_144403.txt";
      const TString fmscan_fm503 = "../Data/FMScan_Data/DataFile_161006_151934.txt";
      TF1 *calib_fit1 = Calibrate(calib_fm503);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/fm503_di.txt";
      const TString do_file = "../Data/Calib_Data/fm503_do.txt";
      double R_fm503 = ratio(di_file, do_file);
      double R_sig_fm503 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm503 = plot_uvB(fmscan_fm503, calib_fit1, R_fm503, R_sig_fm503);
      g_fm503->Draw("LP");
      g_fm503->SetLineColor(kOrange+7);
      g_fm503->SetMarkerColor(kOrange+7);
      leg_uvB->AddEntry( g_fm503 , "F_{m} = 0.503" , "lp");

      u.push_back( g_fm503->Eval(40., 0, "") );
      Fm.push_back( 0.503 );
      u_err.push_back( 0.08 );
      Fm_err.push_back( 0.0 );
    }

  if(plot_FM_Fm548)
    {
      /*
       * Permeability Plot for FM = 0.548 Ferromagnet
       */
      /*Run calibration*/
      const TString calib_fm548 = "../Data/Calib_Data/DataFile_161007_194516.txt";
      const TString fmscan_fm548 = "../Data/FMScan_Data/DataFile_161007_201544.txt";
      TF1 *calib_fit1 = Calibrate(calib_fm548);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/fm612_di.txt";
      const TString do_file = "../Data/Calib_Data/fm612_do.txt";
      double R_fm548 = ratio(di_file, do_file);
      double R_sig_fm548 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm548 = plot_uvB(fmscan_fm548, calib_fit1, R_fm548, R_sig_fm548);
      g_fm548->Draw("LP");
      g_fm548->SetLineColor(kGreen+2);
      g_fm548->SetMarkerColor(kGreen+2);
      leg_uvB->AddEntry( g_fm548 , "F_{m} = 0.548" , "lp");

      u.push_back( g_fm548->Eval(40., 0, "") );
      Fm.push_back( 0.548 );
      u_err.push_back( 0.08 );
      Fm_err.push_back( 0.0 );
    }

  if(plot_FM_Fm554)
    {
      /*
       * Permeability Plot for FM = 0.554 Ferromagnet
       */
      /*Run calibration*/
      const TString calib_fm554 = "../Data/Calib_Data/DataFile.txt";
      const TString fmscan_fm554 = "../Data/FMScan_Data/DataFile.txt";
      TF1 *calib_fit1 = Calibrate(calib_fm554);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/fm612_di.txt";
      const TString do_file = "../Data/Calib_Data/fm612_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_uvB(fmscan_fm554, calib_fit1, R_fm554, R_sig_fm554);
      g_fm554->Draw("LP");
      g_fm554->SetLineColor(kGreen+2);
      g_fm554->SetMarkerColor(kGreen+2);
      leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.554" , "lp");

      u.push_back( g_fm554->Eval(40., 0, "") );
      Fm.push_back( 0.554 );
      u_err.push_back( 0.08 );
      Fm_err.push_back( 0.0 );
    }

  if(plot_FM_Fm574)
    {
      /*
       * Permeability Plot for FM = 0.574 Ferromagnet
       */
      /*Run calibration*/
      const TString calib_fm574 = "../Data/Calib_Data/DataFile_161007_194516.txt";
      const TString fmscan_fm574 = "../Data/FMScan_Data/DataFile_161007_201544.txt";
      TF1 *calib_fit1 = Calibrate(calib_fm574);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/fm612_di.txt";
      const TString do_file = "../Data/Calib_Data/fm612_do.txt";
      double R_fm574 = ratio(di_file, do_file);
      double R_sig_fm574 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm574 = plot_uvB(fmscan_fm574, calib_fit1, R_fm574, R_sig_fm574);
      g_fm574->Draw("LP");
      g_fm574->SetLineColor(kGreen+2);
      g_fm574->SetMarkerColor(kGreen+2);
      leg_uvB->AddEntry( g_fm574 , "F_{m} = 0.574" , "lp");

      u.push_back( g_fm574->Eval(40., 0, "") );
      Fm.push_back( 0.574 );
      u_err.push_back( 0.08 );
      Fm_err.push_back( 0.0 );
    }

  if(plot_FM_Fm602)
    {
      /*
       * Permeability Plot for FM = 0.602 Ferromagnet
       */
      /*Run calibration*/
      const TString calib_fm602 = "../Data/Calib_Data/DataFile_160727_162618.txt";
      const TString fmscan_fm602 = "../Data/FMScan_Data/DataFile_160727_163533.txt";
      TF1 *calib_fit1 = Calibrate(calib_fm602);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/fm612_di.txt";
      const TString do_file = "../Data/Calib_Data/fm612_do.txt";
      double R_fm602 = ratio(di_file, do_file);
      double R_sig_fm602 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm602 = plot_uvB(fmscan_fm602, calib_fit1, R_fm602, R_sig_fm602);
      g_fm602->Draw("LP");
      g_fm602->SetLineColor(kBlue);
      g_fm602->SetMarkerColor(kBlue);
      leg_uvB->AddEntry( g_fm602 , "F_{m} = 0.602" , "lp");

      u.push_back( g_fm602->Eval(40., 0 , "") );
      Fm.push_back( 0.602 );
      u_err.push_back( 0.08 );
      Fm_err.push_back( 0.0 );
    }

  if(plot_FM_Fm612)
    {
      /*
       * Permeability Plot for FM = 0.612 Ferromagnet
       */
      /*Run calibration*/
      const TString calib_fm612 = "../Data/Calib_Data/DataFile_160805_142716.txt";
      const TString fmscan_fm612 = "../Data/FMScan_Data/DataFile_160805_143508.txt";
      TF1 *calib_fit1 = Calibrate(calib_fm612);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/fm612_di.txt";
      const TString do_file = "../Data/Calib_Data/fm612_do.txt";
      double R_fm612 = ratio(di_file, do_file);
      double R_sig_fm612 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm612 = plot_uvB(fmscan_fm612, calib_fit1, R_fm612, R_sig_fm612);
      g_fm612->Draw("LP");
      g_fm612->SetLineColor(kViolet+3);
      g_fm612->SetMarkerColor(kViolet+3);
      leg_uvB->AddEntry( g_fm612 , "F_{m} = 0.612" , "lp");
      u.push_back( g_fm612->Eval(40., 0, "") );
      Fm.push_back( 0.612 );
      u_err.push_back( 0.08 );
      Fm_err.push_back( 0.0 );
    }

  if(plot_FM_Fm625)
    {
      /*
       * Permeability Plot for FM = 0.625 Ferromagnet
       */
      /*Run calibration*/
      const TString calib_fm625 = "../Data/Calib_Data/DataFile_161006_144403.txt";
      const TString fmscan_fm625 = "../Data/FMScan_Data/DataFile_161006_150305.txt";
      TF1 *calib_fit1 = Calibrate(calib_fm625);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/fm612_di.txt";
      const TString do_file = "../Data/Calib_Data/fm612_do.txt";
      double R_fm625 = ratio(di_file, do_file);
      double R_sig_fm625 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm625 = plot_uvB(fmscan_fm625, calib_fit1, R_fm625, R_sig_fm625);
      g_fm625->Draw("LP");
      g_fm625->SetLineColor(kRed);
      g_fm625->SetMarkerColor(kRed);
      leg_uvB->AddEntry( g_fm625 , "F_{m} = 0.625" , "lp");

      u.push_back( g_fm625->Eval(40., 0, "") );
      Fm.push_back( 0.625 );
      u_err.push_back( 0.08 );
      Fm_err.push_back( 0.0 );
    }

  if(plot_FM_Fm651)
    {
      /*
       * Permeability Plot for FM = 0.651 Ferromagnet
       */
      /*Run calibration*/
      const TString calib_fm651 = "../Data/Calib_Data/DataFile_160916_211714.txt";
      const TString fmscan_fm651 = "../Data/FMScan_Data/DataFile_160916_212729_Part2.txt";
      TF1 *calib_fit1 = Calibrate(calib_fm651);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/fm503_di.txt";
      const TString do_file = "../Data/Calib_Data/fm503_do.txt";
      double R_fm651 = ratio(di_file, do_file);
      double R_sig_fm651 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm651 = plot_uvB(fmscan_fm651, calib_fit1, R_fm651, R_sig_fm651);
      g_fm651->Draw("LP");
      g_fm651->SetLineColor(kOrange+7);
      g_fm651->SetMarkerColor(kOrange+7);
      leg_uvB->AddEntry( g_fm651 , "F_{m} = 0.651" , "lp");

      u.push_back( g_fm651->Eval(40., 0, "") );
      Fm.push_back( 0.651 );
      u_err.push_back( 0.08 );
      Fm_err.push_back( 0.0 );
    }

  if(plot_FM_Fm673)
    {
      /*
       * Permeability Plot for FM = 0.673 Ferromagnet
       */
      /*Run calibration*/
      const TString calib_fm673 = "../Data/Calib_Data/DataFile_160928_193601.txt";
      const TString fmscan_fm673 = "../Data/FMScan_Data/DataFile_160928_200210_Part2.txt";
      TF1 *calib_fit1 = Calibrate(calib_fm673);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "../Data/Calib_Data/fm503_di.txt";
      const TString do_file = "../Data/Calib_Data/fm503_do.txt";
      double R_fm673 = ratio(di_file, do_file);
      double R_sig_fm673 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm673 = plot_uvB(fmscan_fm673, calib_fit1, R_fm673, R_sig_fm673);
      g_fm673->Draw("LP");
      g_fm673->SetLineColor(kGreen+2);
      g_fm673->SetMarkerColor(kGreen+2);
      leg_uvB->AddEntry( g_fm673 , "F_{m} = 0.673" , "lp");

      u.push_back( g_fm673->Eval(40., 0, "") );
      Fm.push_back( 0.673 );
      u_err.push_back( 0.08 );
      Fm_err.push_back( 0.0 );
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

  /*
   * Plot of Permeability vs Fractional Mass of Steel Powder in Ferromagnet
   */
  TGraphErrors *g_uvFm = new TGraphErrors(Fm.size(), &(Fm[0]), &(u[0]), &(Fm_err[0]), &(u_err[0]) );
  //TGraph *g_uvFm = new TGraph(Fm.size(), &(Fm[0]), &(u[0]) );
  TCanvas *c_uvFm = new TCanvas();
  g_uvFm->Draw("AP");
  // TF1 *fit = new TF1("fit", "[2]/TMath::Tan([0]*x + [1]) + [3]", 0.0, 1.0);
  // TF1 *fit = new TF1("fit", "expo + pol0[4]", 0.0, 1.0);
  // fit->SetParameter(0, 2.0);
  // fit->SetParameter(1, 1.0); 
  // fit->SetParameter(2, 1.0);
  // fit->SetParameter(3, 1.0);
  // g_uvFm->Fit("fit");
  g_uvFm->Fit("pol3");
  g_uvFm->SetTitle("#mu_{r} vs F_{m}");
  g_uvFm->GetXaxis()->SetTitle("F_{m}");
  g_uvFm->GetYaxis()->SetTitle("#mu_{r}");
  cout << g_uvFm->GetFunction("pol3")->GetChisquare() << endl;
  cout << g_uvFm->GetFunction("pol3")->GetNDF() << endl;

  /*Save plots to png and eps files*/     
      
  // c_uvB->Print("../../Plots/uvB/EpoxySteel_uvB_161006.png");
  // c_uvB->Print("../../Plots/uvB/EpoxySteel_uvB_161006.eps");
  // c_uvFm->Print("../../Plots/uvB/EpoxySteel_uvFm_160929.png");
  // c_uvFm->Print("../../Plots/uvB/EpoxySteel_uvFm_160929.eps");
  
  return;
}
