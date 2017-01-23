/*==========Permeability Plotting Macro===========
 * Project: A Compact Magnetic Field Cloaking Device
 * Author: Thomas Krahulik
 * Date Updated: December 6, 2016
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
  Double_t sig_inner = h_din->GetMeanError(1);
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

TGraphErrors* plot_uvB(
			 const TString scan_file,
			 double R,
			 double R_sig
)
{

  /*Read in Data File to ROOT Tree*/
  cout<< "processing file " << scan_file <<endl;
  TTree *TData = new TTree();
  TData->ReadFile(scan_file, "t/D:x:y:z:B1:B1Range:B2:B2Range:B3:B3Range:V:T1:T2:T3:T4:T5:T6:T7:T8");
  /*Use TTree Draw command to write branches to usable vector*/
  int n = TData->Draw("TMath::Abs(B2):TMath::Abs(B3):0.0:0.005", "", "goff");

  vector<double> B_ext, B_in, Bratio, u, u_err;
  for(int i = 0; i < n; i++)
    {
  //     if(i == 0)
  //     {
  //     	B_ext.push_back(100.00);
  //     }
  //     else{
  //     	  B_ext.push_back( TData->GetV1()[i]);
  //     }
      B_ext.push_back( TData->GetV1()[i] );
      B_in.push_back( TData->GetV2()[i]);
      // B_in.push_back( 90.53);
      // if(TMath::Abs(B_in[i]) <= TMath::Abs(B_ext[i]))
      // {
	Bratio.push_back( B_in[i] / B_ext[i]);
	u.push_back( (Bratio[i]*(R**2) + Bratio[i] - 2 - 2*sqrt((Bratio[i]**2)*(R**2) - Bratio[i]*(R**2) - Bratio[i] + 1 ) ) / (Bratio[i]*(R**2) - Bratio[i]) );
	// u_err.push_back( u[i]*( 5.5*((0.005/Bratio[i])**2.) + 10*((R_sig/R)**2.)  )**0.5 );
	u_err.push_back( 0.01 );
	// u_err.push_back( u[i]*( 5.5*((0.01/Bratio[i])**2.))  )**0.5 );
	// }
      }

  TGraphErrors *g_uvB = new TGraphErrors(n, &B_ext[0], &u[0], TData->GetV3(), &u_err[0]);

  return g_uvB;
}

TGraphErrors* plot_uvB_Old(
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
      if(B_in[i] <= B_ext[i])
      	{
	  Bratio.push_back( B_in[i] / B_ext[i]);
	  u.push_back( (Bratio[i]*(R**2) + Bratio[i] - 2 - 2*sqrt((Bratio[i]**2)*(R**2) - Bratio[i]*(R**2) - Bratio[i] + 1 ) ) / (Bratio[i]*(R**2) - Bratio[i]) );
	  u_err.push_back( u[i]*( 5.5*((0.005/Bratio[i])**2.) + 10*((R_sig/R)**2.)  )**0.5 );
	  // u_err.push_back( u[i]*( 5.5*((0.01/Bratio[i])**2.))  )**0.5 );
	}
    }

  TGraphErrors *g_uvB = new TGraphErrors(n, &B_ext[0], &u[0], TData->GetV3(), &u_err[0]);

  return g_uvB;
}

/* ====================================
 * Plot Magnetic Permeability vs Field
   ====================================*/

int makePlot_uvB_Compare()
{

  TCanvas *c_uvB = new TCanvas();
  TH1 *h_uvB = c_uvB->DrawFrame(0, 1.0, 200, 5.0);
  // h_uvB->SetTitle("Permeability of Epoxy/Steel Ferromagnets");
  h_uvB->SetTitle("");
  h_uvB->GetXaxis()->SetTitle("B_{0} (mT)");
  h_uvB->GetXaxis()->SetTitleOffset(1.5);
  h_uvB->GetYaxis()->SetTitle("#mu_{r}");
  h_uvB->GetYaxis()->SetTitleOffset(1.0);
  /*Create Legend*/
  TLegend *leg_uvB = new TLegend(0.5,0.7,0.8,0.85);
  leg_uvB->SetNColumns(1);
  leg_uvB->Draw();

  const bool plot_FM_Fm699 = true;

  const bool plot_FM_Fm554 = false;
  const bool plot_FM_Fm625 = false;
  const bool plot_FM2 = false;
  const bool plot_FM3 = false;
  const bool plot_FM4 = false;
  const bool plot_FM_Fm554_new = false;

//   vector<double> u, Fm, u_err, Fm_err;

//   /*Include Point for Fm = 0.0, No Steel Powder*/
//   u.push_back(1.0);
//   Fm.push_back(0.0);
//   u_err.push_back(0.03);
//   Fm_err.push_back(0.0);

//   /*Include Point for Fm = 1.0, Only Steel Powder*/
//   // u.push_back(1000.);
//   // Fm.push_back(1.0);
//   // u_err.push_back(0.0);
//   // Fm_err.push_back(0.0);


  if(plot_FM_Fm699)
    {
      /*
       * Permeability Plot for FM = 0.699 Ferromagnet 
       */
      /*Run calibration*/
      const TString calib_fm699 = "../Ferromagnet/analysis-ferromagnet/Data/Calib_Data/DataFile_161102_181415.txt";
      const TString fmscan_fm699 = "../Ferromagnet/analysis-ferromagnet/Data/FMScan_Data/DataFile_161102_183814.txt";
      TF1 *calib_fit1 = Calibrate(calib_fm699);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm699_di.txt";
      const TString do_file = "Data/FMDiameters/fm699_do.txt";
      double R_fm699 = ratio(di_file, do_file);
      double R_sig_fm699 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm699 = plot_uvB_Old(fmscan_fm699, calib_fit1, R_fm699, R_sig_fm699);
      g_fm699->Draw("P");
      // g_fm699->SetLineColor(kGreen+2);
      // g_fm699->SetMarkerColor(kGreen+2);
      leg_uvB->AddEntry( g_fm699 , "F_{m} = 0.699 SBU" , "p");
      // g_fm699->Fit("pol1", "", "", 20., 150.);

      cout << g_fm699->Eval(40.) << endl;

      // u.push_back( g_fm699->Eval(40., 0, "") );
      // Fm.push_back( 0.699 );
      // u_err.push_back( 0.05 );
      // Fm_err.push_back( 0.0 );
    }








  if(plot_FM2)
    {
      /*
       * Permeability Plot for Fm = 0.554 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-45-58.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm618_di.txt";
      const TString do_file = "Data/FMDiameters/fm618_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_uvB(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      // g_fm554->SetLineColor(kRed);
      g_fm554->SetMarkerColor(kGreen+2);
      leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.554 Run 1" , "p");

      // u.push_back( g_fm104->Eval(40., 0, "") );
      // Fm.push_back( 0.104 );
      // u_err.push_back( 0.03 );
      // Fm_err.push_back( 0.0 );
    }


  if(plot_FM_Fm554_new)
    {
      /*
       * Permeability Plot for Fm = 0.554 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_19-56-40.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm554_di.txt";
      const TString do_file = "Data/FMDiameters/fm554_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_uvB(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      // g_fm554->SetLineColor(kRed);
      g_fm554->SetMarkerColor(kGreen+2);
      leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.554 Run 1" , "p");

      // u.push_back( g_fm104->Eval(40., 0, "") );
      // Fm.push_back( 0.104 );
      // u_err.push_back( 0.03 );
      // Fm_err.push_back( 0.0 );
    }


  if(plot_FM_Fm554)
    {
      /*
       * Permeability Plot for Fm = 0.554 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_19-56-40.txt";
      // const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-06_14-32-38.txt";
      // const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-07_02-02-14.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm554_di.txt";
      const TString do_file = "Data/FMDiameters/fm554_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_uvB(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      // g_fm554->SetLineColor(kRed);
      g_fm554->SetMarkerColor(kBlue);
      leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.554 Run 1" , "p");

      // u.push_back( g_fm104->Eval(40., 0, "") );
      // Fm.push_back( 0.104 );
      // u_err.push_back( 0.03 );
      // Fm_err.push_back( 0.0 );
    }

  if(plot_FM_Fm554)
    {
      /*
       * Permeability Plot for Fm = 0.554 Ferromagnet
       */

      // const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-06_14-32-38.txt";
      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-07_02-02-14.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm554_di.txt";
      const TString do_file = "Data/FMDiameters/fm554_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_uvB(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      // g_fm554->SetLineColor(kRed);
      g_fm554->SetMarkerColor(kRed);
      leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.554 Run 2" , "p");

      // u.push_back( g_fm104->Eval(40., 0, "") );
      // Fm.push_back( 0.104 );
      // u_err.push_back( 0.03 );
      // Fm_err.push_back( 0.0 );
    }

  if(plot_FM_Fm554)
    {
      /*
       * Permeability Plot for FM = 0.554 Ferromagnet 
       */
      /*Run calibration*/
      const TString calib_fm554 = "../Ferromagnet/analysis-ferromagnet/Data/Calib_Data/DataFile_161102_181415.txt";
      const TString fmscan_fm554 = "../Ferromagnet/analysis-ferromagnet/Data/FMScan_Data/DataFile_161102_183814.txt";
      TF1 *calib_fit1 = Calibrate(calib_fm554);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm554_di.txt";
      const TString do_file = "Data/FMDiameters/fm554_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_uvB_Old(fmscan_fm554, calib_fit1, R_fm554, R_sig_fm554);
      g_fm554->Draw("P");
      // g_fm554->SetLineColor(kGreen+2);
      // g_fm554->SetMarkerColor(kGreen+2);
      leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.554 SBU" , "p");
      // g_fm554->Fit("pol1", "", "", 20., 150.);

      cout << g_fm554->Eval(40.) << endl;

      // u.push_back( g_fm554->Eval(40., 0, "") );
      // Fm.push_back( 0.554 );
      // u_err.push_back( 0.05 );
      // Fm_err.push_back( 0.0 );
    }


  if(plot_FM_Fm625)
    {
      /*
       * Permeability Plot for FM = 0.554 Ferromagnet 
       */
      /*Run calibration*/
      const TString calib_fm554 = "../Ferromagnet/analysis-ferromagnet/Data/Calib_Data/DataFile_161006_144403.txt";
      const TString fmscan_fm554 = "../Ferromagnet/analysis-ferromagnet/Data/FMScan_Data/DataFile_161006_150305.txt";
      TF1 *calib_fit1 = Calibrate(calib_fm554);
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm554_di.txt";
      const TString do_file = "Data/FMDiameters/fm554_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_uvB_Old(fmscan_fm554, calib_fit1, R_fm554, R_sig_fm554);
      g_fm554->Draw("P");
      // g_fm554->SetLineColor(kGreen+2);
      // g_fm554->SetMarkerColor(kGreen+2);
      leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.554 SBU" , "p");
      // g_fm554->Fit("pol1", "", "", 20., 150.);

      cout << g_fm554->Eval(40.) << endl;

      // u.push_back( g_fm554->Eval(40., 0, "") );
      // Fm.push_back( 0.554 );
      // u_err.push_back( 0.05 );
      // Fm_err.push_back( 0.0 );
    }


//   if(ideal_cloak)
//     {
//       /*Calculate Radius Ratio and Uncertainty*/
//       const TString di_file = "../Data/Calib_Data/fm612_di.txt";
//       const TString do_file = "../Data/Calib_Data/fm612_do.txt";
//       double R_fv30 = ratio(di_file, do_file);
//       // cout << R_fv30 << endl;
//       double R_sig_fv30 = r_sig(di_file, do_file);
//       // cout << R_sig_fv30 << endl;
//       /*Calculate theoretical permeability of ferromagnet*/
//       double u_cloak = (1. + pow(R_fv30,2.)) / (1. - pow(R_fv30,2.));
//       cout << "Desired Permeability: " << u_cloak << endl;
//       /*Draw Line for Permeability of Ideal Cloak*/
//       TLine *l_ucloak = new TLine(0.0, u_cloak, 60.0, u_cloak);
//       l_ucloak->SetLineStyle(2);
//       l_ucloak->Draw();
//       leg_uvB->AddEntry( l_ucloak , "Ideal Cloak" , "l");
//     }

//   /*
//    * Plot of Permeability vs Fractional Mass of Steel Powder in Ferromagnet
//    */
//   TGraphErrors *g_uvFm = new TGraphErrors(Fm.size(), &(Fm[0]), &(u[0]), &(Fm_err[0]), &(u_err[0]) );
//   //TGraph *g_uvFm = new TGraph(Fm.size(), &(Fm[0]), &(u[0]) );
//   TCanvas *c_uvFm = new TCanvas();
//   g_uvFm->Draw("AP");
//   TF1 *fit = new TF1("fit", "[0]/TMath::Tan([1]*x + [2]) + [3]", 0.0, 1.0);
//   fit->SetParameter(0, 2.0);
//   fit->SetParameter(1, 1.0); 
//   fit->SetParameter(2, 1.0);
//   fit->SetParameter(3, 1.0);
//   g_uvFm->Fit("fit");
//   // g_uvFm->Fit("pol3");
//   g_uvFm->SetTitle("#mu_{r} vs F_{m}");
//   g_uvFm->GetXaxis()->SetTitle("F_{m}");
//   g_uvFm->GetYaxis()->SetTitle("#mu_{r}");
//   cout << g_uvFm->GetFunction("fit")->GetChisquare() << endl;
//   cout << g_uvFm->GetFunction("fit")->GetNDF() << endl;

//   /*Save plots to png and eps files*/     
      
//   // c_uvB->Print("../../Plots/uvB/EpoxySteel_uvB_161208.png");
//   // c_uvFm->Print("../../Plots/uvB/EpoxySteel_uvFm_161208.png");

  return 0;
}
