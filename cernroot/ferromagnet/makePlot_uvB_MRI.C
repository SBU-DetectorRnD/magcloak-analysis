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


TGraphErrors* plot_point(
			     const TString infile,
			     double R,
			     double R_sig
			     )
{

  TTree *TData = new TTree();
  TData->ReadFile(infile, "t/D:x:y:z:B1:B1Range:B2:B2Range:B3:B3Range:V:T1:T2:T3:T4:T5:T6:T7:T8");

  TH1D *h = new TH1D("h", "", 10000, 0, 10000);
  TH1D *h2 = new TH1D("h2", "", 10000, 0, 10000);
  TData->Draw("TMath::Abs(B2)>>h", "", "goff");
  TData->Draw("TMath::Abs(B3)>>h2", "", "goff");

  vector<double> B_ext, B_in, Bratio, u, u_err, B_err;

  B_in.push_back(h2->GetMean(1));
  B_ext.push_back(h->GetMean(1));

  Bratio.push_back(B_in[0] / B_ext[0]);
  u.push_back( (Bratio[0]*(R**2) + Bratio[0] - 2 - 2*sqrt((Bratio[0]**2)*(R**2) - Bratio[0]*(R**2) - Bratio[0] + 1 ) ) / (Bratio[0]*(R**2) - Bratio[0]) );

  B_err.push_back( 0.00 );
  u_err.push_back( 0.00 );

  TGraphErrors *g_uvB = new TGraphErrors(u.size(), &B_ext[0], &u[0], &B_err[0], &u_err[0]);

  h->Delete();
  h2->Delete();

  return g_uvB;

}

TGraphErrors* plot_ramp(
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


/* ====================================
 * Plot Magnetic Permeability vs Field
   ====================================*/

int makePlot_uvB_MRI()
{

  TCanvas *c_uvB = new TCanvas();
  TH1 *h_uvB = c_uvB->DrawFrame(0, 1.0, 550, 5.0);
  // h_uvB->SetTitle("Permeability of Epoxy/Steel Ferromagnets");
  h_uvB->SetTitle("");
  h_uvB->GetXaxis()->SetTitle("B_{0} (mT)");
  h_uvB->GetXaxis()->SetTitleOffset(1.5);
  h_uvB->GetYaxis()->SetTitle("#mu_{r}");
  h_uvB->GetYaxis()->SetTitleOffset(1.0);
  /*Create Legend*/
  TLegend *leg_uvB = new TLegend(0.45,0.65,0.83,0.85);
  leg_uvB->SetNColumns(1);
  leg_uvB->Draw();

  const bool plot_554ramp = true;
  const bool plot_699ramp2 = true;
  const bool plot_699ramp3 = true;
  const bool plot_699ramp4 = true;
  const bool plot_699ramp5 = true;
  const bool plot_618ramp = true;

  const bool plot_554at200 = true;
  const bool plot_618at200 = true;
  const bool plot_673at200 = true;
  const bool plot_699at200 = true;

  const bool plot_554at300 = true;
  const bool plot_618at300 = true;
  const bool plot_673at300 = true;
  const bool plot_699at300 = true;

  const bool plot_554at400 = true;
  const bool plot_618at400 = true;
  const bool plot_673at400 = true;
  const bool plot_699at400 = true;

  const bool plot_554at500 = true;
  const bool plot_618at500 = true;
  const bool plot_673at500 = true;
  const bool plot_699at500 = true;

  const bool ideal_cloak = true;
  const bool exp_cloak = true;

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

  if(plot_554at200)
    {
      /*
       * Permeability Plot for Fm = 0.544 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_20-31-45.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm554_di.txt";
      const TString do_file = "Data/FMDiameters/fm554_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      // g_fm554->SetLineColor(kRed);
      g_fm554->SetMarkerColor(kBlue);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
      leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.554" , "p");
    }

  if(plot_554at300)
    {
      /*
       * Permeability Plot for Fm = 0.544 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_20-58-06.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm554_di.txt";
      const TString do_file = "Data/FMDiameters/fm554_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kBlue);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
    }

  if(plot_554at400)
    {
      /*
       * Permeability Plot for Fm = 0.544 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-18-15.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm554_di.txt";
      const TString do_file = "Data/FMDiameters/fm554_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kBlue);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
    }

  if(plot_554at500)
    {
      /*
       * Permeability Plot for Fm = 0.544 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-37-52.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm554_di.txt";
      const TString do_file = "Data/FMDiameters/fm554_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kBlue);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
    }

  if(plot_618at200)
    {
      /*
       * Permeability Plot for Fm = 0.618 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_20-35-17.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm618_di.txt";
      const TString do_file = "Data/FMDiameters/fm618_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kGreen+2);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
      leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.618" , "p");

    }

  if(plot_618at300)
    {
      /*
       * Permeability Plot for Fm = 0.618 Ferromagnet
       */
      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-00-57.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm618_di.txt";
      const TString do_file = "Data/FMDiameters/fm618_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kGreen+2);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
    }

  if(plot_618at400)
    {
      /*
       * Permeability Plot for Fm = 0.618 Ferromagnet
       */
      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-20-51.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm618_di.txt";
      const TString do_file = "Data/FMDiameters/fm618_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kGreen+2);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
    }

  if(plot_618at500)
    {
      /*
       * Permeability Plot for Fm = 0.618 Ferromagnet
       */
      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-40-20.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm618_di.txt";
      const TString do_file = "Data/FMDiameters/fm618_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kGreen+2);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
    }

  if(plot_673at200)
    {
      /*
       * Permeability Plot for Fm = 0.673 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_20-37-24.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm673_di.txt";
      const TString do_file = "Data/FMDiameters/fm673_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P");
      g_fm554->SetMarkerColor(kMagenta);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
      leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.673" , "p");

    }

  if(plot_673at300)
    {
      /*
       * Permeability Plot for Fm = 0.618 Ferromagnet
       */
      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-03-21.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm673_di.txt";
      const TString do_file = "Data/FMDiameters/fm673_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kMagenta);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
    }

  if(plot_673at400)
    {
      /*
       * Permeability Plot for Fm = 0.618 Ferromagnet
       */
      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-22-51.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm673_di.txt";
      const TString do_file = "Data/FMDiameters/fm673_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kMagenta);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
    }

  if(plot_673at500)
    {
      /*
       * Permeability Plot for Fm = 0.618 Ferromagnet
       */
      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-42-13.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm673_di.txt";
      const TString do_file = "Data/FMDiameters/fm673_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kMagenta);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
    }


  if(plot_699at200)
    {
      /*
       * Permeability Plot for Fm = 0.699 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_20-29-24.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm699_di.txt";
      const TString do_file = "Data/FMDiameters/fm699_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kRed);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
      leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.699" , "p");

    }

  if(plot_699at300)
    {
      /*
       * Permeability Plot for Fm = 0.699 Ferromagnet
       */
      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_20-54-13.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm699_di.txt";
      const TString do_file = "Data/FMDiameters/fm699_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kRed);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
    }

  if(plot_699at400)
    {
      /*
       * Permeability Plot for Fm = 0.699 Ferromagnet
       */
      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-15-59.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm699_di.txt";
      const TString do_file = "Data/FMDiameters/fm699_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kRed);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
    }

  if(plot_699at500)
    {
      /*
       * Permeability Plot for Fm = 0.699 Ferromagnet
       */
      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-36-02.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm699_di.txt";
      const TString do_file = "Data/FMDiameters/fm699_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_point(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kRed);
      g_fm554->SetMarkerStyle(22);
      g_fm554->SetMarkerSize(1.5);
    }

  if(plot_554ramp)
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
      TGraphErrors *g_fm554 = plot_ramp(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      // g_fm554->SetLineColor(kRed);
      g_fm554->SetMarkerColor(kBlue);
      // leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.554 Ramp" , "p");

    }

  if(plot_699ramp2)
    {
      /*
       * Permeability Plot for Fm = 0.699 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_20-14-15.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm699_di.txt";
      const TString do_file = "Data/FMDiameters/fm699_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_ramp(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kRed);
      // leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.699 Ramp" , "p");

    }

  if(plot_699ramp3)
    {
      /*
       * Permeability Plot for Fm = 0.699 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_20-41-31.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm699_di.txt";
      const TString do_file = "Data/FMDiameters/fm699_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_ramp(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kRed);
      // leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.699 Ramp" , "p");

    }

  if(plot_699ramp4)
    {
      /*
       * Permeability Plot for Fm = 0.699 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-06-35.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm699_di.txt";
      const TString do_file = "Data/FMDiameters/fm699_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_ramp(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kRed);
      // leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.699 Ramp" , "p");

    }

  if(plot_699ramp5)
    {
      /*
       * Permeability Plot for Fm = 0.699 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-25-26.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm699_di.txt";
      const TString do_file = "Data/FMDiameters/fm699_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_ramp(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kRed);
      // leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.699 Ramp" , "p");

    }

  if(plot_618ramp)
    {
      /*
       * Permeability Plot for Fm = 0.618 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-45-58.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm699_di.txt";
      const TString do_file = "Data/FMDiameters/fm699_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_ramp(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kGreen+2);
      // leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.699 Ramp" , "p");

    }

  if(plot_673ramp)
    {
      /*
       * Permeability Plot for Fm = 0.673 Ferromagnet
       */

      const TString fmscan_fm554 = "Data/PermeabilityScans/DataFile_2016-12-08_21-45-58.txt";
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm673_di.txt";
      const TString do_file = "Data/FMDiameters/fm673_do.txt";
      double R_fm554 = ratio(di_file, do_file);
      double R_sig_fm554 = r_sig(di_file, do_file);
      /*Plot u vs B for FM*/
      TGraphErrors *g_fm554 = plot_ramp(fmscan_fm554, R_fm554, R_sig_fm554);
      g_fm554->Draw("P"); 
      g_fm554->SetMarkerColor(kGreen+2);
      // leg_uvB->AddEntry( g_fm554 , "F_{m} = 0.699 Ramp" , "p");

    }

  if(ideal_cloak)
    {
      /*Calculate Radius Ratio and Uncertainty*/
      const TString di_file = "Data/FMDiameters/fm554_di.txt";
      const TString do_file = "Data/FMDiameters/fm554_do.txt";
      double R_fv30 = ratio(di_file, do_file);
      // cout << R_fv30 << endl;
      double R_sig_fv30 = r_sig(di_file, do_file);
      // cout << R_sig_fv30 << endl;
      /*Calculate theoretical permeability of ferromagnet*/
      double u_cloak = (1. + pow(R_fv30,2.)) / (1. - pow(R_fv30,2.));
      cout << "Desired Permeability: " << u_cloak << endl;
      /*Draw Line for Permeability of Ideal Cloak*/
      TLine *l_ucloak = new TLine(0.0, u_cloak, 500.0, u_cloak);
      l_ucloak->SetLineStyle(2);
      l_ucloak->Draw();
      leg_uvB->AddEntry( l_ucloak , "Ideal Cloak - Theory" , "l");

      TLine *l_ucloak2 = new TLine(0.0, 2.0706, 500.0, 2.0706);
      l_ucloak2->SetLineStyle(2);
      l_ucloak2->SetLineColor(kBlue);
      l_ucloak2->Draw();
      leg_uvB->AddEntry( l_ucloak2 , "Ideal Cloak - Experiment" , "l");
    }

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
