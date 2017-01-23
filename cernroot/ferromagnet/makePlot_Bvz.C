/*==============B vs z Plotting Macro=============
 * Project: A Compact Magnetic Field Cloaking Device
 * Author: Thomas Krahulik
 * Date: June 22, 2016
 * Purpose: To plot B vs z measurements
 * To run macro: root -l makePlot_Bvz.C
 =================================================*/

/* =================================
 * B vs z Analysis Plotting Function
   =================================*/
/*
 * This functions reads in a data
 * file of B vs z measurements
 * and generates a plot.
 */

TGraphErrors* plot_Bvz(
		const TString f_data,
		double offset
)
{
  /*Read in Data File*/
  cout << "Processing File " << f_data << endl;
  /*Read in B vs z data to a ROOT tree*/
  TTree *TData = new TTree();
  TData->ReadFile(f_data, "z/D:B:B_err:I:I_err");
  /*Obtain number of points and set branches of tree to arrays*/
  Int_t n = TData->Draw("z:TMath::Abs(B):0.5:B_err" , "", "goff");
  /*Graph B vs z data points*/
  TGraphErrors *g_Bvz = new TGraphErrors(n, TData->GetV1(), TData->GetV2(), TData->GetV3(), TData->GetV4());

  /*Center the B vs z Plot around the Center of the FM*/
  for (int i = 0; i < g_Bvz->GetN(); i++)
    {
      /*Subtract Out Center Point from z Position of FM*/
      g_Bvz->SetPoint( i, g_Bvz->GetX()[i] - offset, g_Bvz->GetY()[i] );
      /*Calculate Errors on Each Point*/
      g_Bvz->SetPointError(i , sqrt((g_Bvz->GetEX()[i])**2 + (0.5)**2) , g_Bvz->GetEY()[i] );
    }

  return g_Bvz;

}

/*===============================================
 * Plotting B vs z for Collection of Measurements
 ================================================*/
/*
 * Plot and compare several measurements of B vs z
 * Calling format function for each plot:
 * plot_Bvz("/path/to/data_file.txt", z_offset)
 */

void makePlot_Bvz()
{
  /*Choose Which FM to Plot B vs z*/
  const bool plot_Cloak = false;
  const bool plot_Cloak2 = false;
  const bool plot_FM_Fm651 = false;
  const bool plot_FM_Fm554 = true;
  const bool plot_FM_Fm503 = false;
  const bool plot_FM_Fm548 = false;
  const bool plot_FM_Fm602 = false;
  const bool plot_FM_Fm625 = false;
  const bool plot_FM_Fm612 = false;

  if(plot_Cloak)
    {
      const TString ref_file = "../Data/Bvz_Scan_Data/DataFile_160922_221623.txt";
      const TString Bvz_file = "../Data/Bvz_Scan_Data/DataFile_160922_223055.txt";
      double center = 133;
      double length = 122;
      double B_max = 50.0;
      double B_min = 0.0;
      const TString label = "Outside";
      const TString png_out = "../../Plots/Bvz/Bvz_FM_Fm651.png";
      const TString eps_out = "../../Plots/Bvz/Bvz_FM_Fm651.eps";
    }

  if(plot_Cloak2)
    {
      const TString ref_file = "../Data/Bvz_Scan_Data/DataFile_160922_221623.txt";
      const TString Bvz_file = "../Data/Bvz_Scan_Data/DataFile_160922_224036.txt";
      double center = 133;
      double length = 122;
      double B_max = 50.0;
      double B_min = 0.0;
      const TString label = "Outside";
      const TString png_out = "../../Plots/Bvz/Bvz_FM_Fm651.png";
      const TString eps_out = "../../Plots/Bvz/Bvz_FM_Fm651.eps";
    }


  /*Plot B vs z for FM Fm 0.503*/
  if(plot_FM_Fm651)
    {
      const TString ref_file = "../Data/Bvz_Scan_Data/DataFile_160916_201420.txt";
      const TString Bvz_file = "../Data/Bvz_Scan_Data/DataFile_160916_210246.txt";
      double center = 130;
      double length = 122;
      double B_max = 25.0;
      double B_min = 20.0;
      const TString label = "F_{m} = 0.651 FM Scan";
      const TString png_out = "../../Plots/Bvz/Bvz_FM_Fm651.png";
      const TString eps_out = "../../Plots/Bvz/Bvz_FM_Fm651.eps";
    }

  /*Plot B vs z for FM Fm 0.503*/
  if(plot_FM_Fm503)
    {
      const TString ref_file = "../Data/Bvz_Scan_Data/DataFile_160916_201420.txt";
      const TString Bvz_file = "../Data/Bvz_Scan_Data/DataFile_160916_203302.txt";
      double center = 120;
      double length = 122;
      double B_max = 25.0;
      double B_min = 22.0;
      const TString label = "F_{m} = 0.503 FM Scan";
      const TString png_out = "../../Plots/Bvz/Bvz_FM_Fm503.png";
      const TString eps_out = "../../Plots/Bvz/Bvz_FM_Fm503.eps";
    }

  if(plot_FM_Fm548)
    {
      const TString ref_file = "../Data/Bvz_Scan_Data/DataFile_160916_201420.txt";
      const TString Bvz_file = "../Data/Bvz_Scan_Data/DataFile_161007_214103.txt";
      double center = 120;
      double length = 122;
      double B_max = 25.0;
      double B_min = 22.0;
      const TString label = "F_{m} = 0.548 FM Scan";
      const TString png_out = "../../Plots/Bvz/Bvz_FM_Fm503.png";
      const TString eps_out = "../../Plots/Bvz/Bvz_FM_Fm503.eps";
    }

  if(plot_FM_Fm554)
    {
      const TString ref_file = "../Data/Bvz_Scan_Data/DataFile_161020_195414.txt";
      const TString Bvz_file = "../Data/Bvz_Scan_Data/DataFile_161020_200815.txt";
      double center = 130;
      double length = 122;
      double B_max = 26.0;
      double B_min = 23.0;
      const TString label = "F_{m} = 0.554 FM Scan";
      const TString png_out = "../../Plots/Bvz/Bvz_FM_Fm554.png";
      const TString eps_out = "../../Plots/Bvz/Bvz_FM_Fm554.eps";
    }

  /*Plot B vs z for FM Fm = 0.612*/
  if(plot_FM_Fm612)
    {
      const TString ref_file = "../Data/Bvz_Scan_Data/DataFile_160805_135738.txt";
      const TString Bvz_file = "../Data/Bvz_Scan_Data/DataFile_160805_141237.txt";
      double center = 120;
      double length = 122;
      double B_max = 25.0;
      double B_min = 21.0;
      const TString label = "F_{m} = 0.612 FM Scan";
      const TString png_out = "../../Plots/Bvz/Bvz_FM_Fm612.png";
      const TString eps_out = "../../Plots/Bvz/Bvz_FM_Fm612.eps";
    }

  /*Plot B vs z for FM Fm = 0.625*/
  if(plot_FM_Fm625)
    {
      const TString ref_file = "../Data/Bvz_Scan_Data/DataFile_160801_114549.txt";
      const TString Bvz_file = "../Data/Bvz_Scan_Data/DataFile_160801_115913.txt";
      double center = 120;
      double length = 122;
      double B_max = 27.0;
      double B_min = 23.0;
      const TString label = "F_{m} = 0.625 FM Scan";
      const TString png_out = "../../Plots/Bvz/Bvz_FM_Fm625.png";
      const TString eps_out = "../../Plots/Bvz/Bvz_FM_Fm625.eps";
    }

  /*Plot B vs z for FM Fm 0.602*/
  if(plot_FM_Fm602)
    {
      const TString ref_file = "../Data/Bvz_Scan_Data/DataFile_160727_160723.txt";
      const TString Bvz_file = "../Data/Bvz_Scan_Data/DataFile_160727_161501.txt";
      double center = 120;
      double length = 122;
      double B_max = 27.0;
      double B_min = 23.0;
      const TString label = "F_{m} = 0.602 FM Scan";
      const TString png_out = "../../Plots/Bvz/Bvz_FM_Fm602.png";
      const TString eps_out = "../../Plots/Bvz/Bvz_FM_Fm602.eps";
    }

  /*Create blank histogram to have a method of modifying axes and labels*/
  TCanvas *c_data = new TCanvas;
  /*To set ranges: DrawFrame(x_min, y_min, x_max, y_max)*/
  TH1 *h_data = c_data->DrawFrame(-1.0*length, 0.98*B_min, length, 1.03*B_max);
  h_data->SetTitle("");
  h_data->GetXaxis()->SetTitle("z (mm)");
  h_data->GetXaxis()->SetTitleOffset(1.0);
  h_data->GetYaxis()->SetTitle("B_{0} (mT)");
  h_data->GetYaxis()->SetTitleOffset(1.3);

  /*Plot B vs z for a Reference Field*/
  TGraphErrors *g_Ref = plot_Bvz(ref_file, center);
  g_Ref->Draw("LP");
  g_Ref->SetLineColor(kBlue);
  g_Ref->SetMarkerColor(kBlue);
  /*Plot B vs z Measurement for Ferromagnet*/
  TGraphErrors *g_BvzScan1 = plot_Bvz(Bvz_file, center);
  g_BvzScan1->Draw("LP");
  g_BvzScan1->SetLineColor(kRed);
  g_BvzScan1->SetMarkerColor(kRed);

  /*Add Vertical Lines at Important Points*/
  /*To set locations of lines: TLine(x_min, y_min, x_max, y_max)*/
  TLine *FM_start = new TLine(-1.0*length/2.0, 0.99*B_min, -1.0*length/ 2.0, 1.02*B_max);
  FM_start->SetLineStyle(2);
  FM_start->SetLineColor(kGreen+2);
  FM_start->Draw();
  TLine *FM_end = new TLine((length/2.0), 0.99*B_min, (length/2.0), 1.02*B_max);
  FM_end->SetLineStyle(2);
  FM_end->SetLineColor(kGreen+2);
  FM_end->Draw();
  TLine *FM_center = new TLine(0.0, 0.99*B_min, 0.0, 1.02*B_max);
  FM_center->SetLineStyle(2);
  FM_center->Draw();

  /*Create Legend*/
  TLegend *l_FMscan = new TLegend(0.52,0.77,0.8,0.87);
  l_FMscan->SetNColumns(1);
  l_FMscan->AddEntry( g_Ref , "Reference Field" , "lp");
  l_FMscan->AddEntry( g_BvzScan1 , label , "lp");
  l_FMscan->AddEntry( FM_end , "End of Ferromagnet" , "l");
  l_FMscan->AddEntry( FM_center , "Center" , "l");
  l_FMscan->Draw();
  
  /*Save plots to png and eps files*/
  
  // c_data->Print(png_out);
  // c_data->Print(eps_out);
  
  return;
}
