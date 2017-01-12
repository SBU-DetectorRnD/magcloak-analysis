/*---------------Beam Spot Analysis-----------------
 * Project: A Compact Magnetic Field Cloaking Device
 * Author: Thomas Krahulik
 * Date: June 2, 2016
 * Purpose: To develop a program that locates the
 * center of a beam spot and performs an analysis
 * of any shift of the beam.
 * To compile and execute run the command:
 * root -L beamspot_analysis.C++
 ---------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>

#include <TROOT.h>
#include <TFile.h>
#include <TF1.h>
#include <TImage.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TH2D.h>
#include <TStyle.h>
#include <TFormula.h>
#include <TLine.h>
#include <TFrame.h>

using namespace std;


void beamspot_analysis(
		       TString in_image = "../Images/Test_2016_07_08/IMG_0017_edit.png",
		       TString qualitycheck = "",
		       TString root_output = ""
		       )
{
  gStyle->SetOptStat(kFALSE);
  gStyle->SetPalette(55);
  gStyle->SetCanvasDefH(6000);
  gStyle->SetCanvasDefW(4000);

  TCanvas* c_qcheck = new TCanvas();
  c_qcheck->Divide(2,3);
  TImage *image1 = TImage::Open(in_image);
  c_qcheck->cd(1);
  image1->Draw();
  UInt_t xPixels1 = image1->GetWidth();
  UInt_t yPixels1 = image1->GetHeight();
  UInt_t *argb1 = image1->GetArgbArray();

  float I_lowcut1 = 0.03;
  float I_highcut1 = 1.0;
  /*
   *---------Beam Spot Histograms-----------
   * Turn beam spot images into two-dimensional histograms with intensity on z-axis
   * One histogram is entire image without intensity cut
   * Second histogram makes intensity cuts
   * and removes lower right corner to exclude time stamp
   */

  TH2D* h_rep1 = new TH2D("h_rep1","Beam Spot",xPixels1,0,xPixels1,yPixels1,-1.0*yPixels1,0);
  TH2D* h_cut1 = new TH2D("h_cut1","Beam Spot with Intensity Cut of 3%",xPixels1,0,xPixels1,yPixels1,-1.0*yPixels1,0);

  for (UInt_t row1=0; row1<xPixels1; ++row1) {
    for (UInt_t col1=0; col1<yPixels1; ++col1) {
      int index1 = col1*xPixels1+row1;
      float gray1 = float(argb1[index1]&0xff)/256.;

      h_rep1->SetBinContent(row1+1,yPixels1-col1,gray1);
      
      //Intensity Cut
      if ((gray1 >= I_lowcut1) & (gray1 <= I_highcut1)){
	  h_cut1->SetBinContent(row1+1,yPixels1-col1,gray1);
      }
    }
  }

  double convert = 1.0; 
  double pipe = 1000.0;
  cout << convert << " " << pipe << endl;

  TImage *image = TImage::Open(in_image);
  /*Use Gaussian Blur to Remove Effect of Noise (ie Single Intensity Pixels) */
  /*VERY IMPORTANT! DOES NOT WORK WITHOUT*/
  image->Blur(51, 51);
  UInt_t xPixels = image->GetWidth();
  UInt_t yPixels = image->GetHeight();
  UInt_t *argb   = image->GetArgbArray();

  std::cout << "Pixels along x-axis: " << xPixels << endl;
  std::cout << "Pixels along y-axis: " << yPixels << endl;
  /*
   *----------Intensity Histogram----------
   * Fill intensity histogram to determine 
   * where to make cuts to exclude noise
   */
  TH1D* h_I = new TH1D("h_I", "Intensity Distribution", 100, 0, 1);

  for (UInt_t row=0; row<xPixels; ++row) {
    for (UInt_t col=0; col<yPixels; ++col) {
      int index = col*xPixels+row;
      float gray = float(argb[index]&0xff)/256;
      h_I->Fill(gray);
    }
  }
  /*
  TCanvas *c_h_I = new TCanvas();
  c_h_I->Draw();
  h_I->Draw();
  */
  //  float I_lowcut = 0.8;
  float I_lowcut = 0.03;
  float I_highcut = 1.0;

  /*
   *---------Beam Spot Histograms-----------
   * Turn beam spot images into two-dimensional histograms with intensity on z-axis
   * One histogram is entire image without intensity cut
   * Second histogram makes intensity cuts
   * and removes lower right corner to exclude time stamp
   */

  TH2D* h_rep = new TH2D("h_rep","Beam Spot After Gaussian Blur",xPixels,0,xPixels,yPixels,-1.0*yPixels,0);
  TH2D* h_cut = new TH2D("h_cut","Beam Spot with Intensity Cut of 3% After Gaussian Blur",xPixels,0,xPixels,yPixels,-1.0*yPixels, 0);

  float weight_sumx = 0;
  float weight_sumy = 0;
  float gray_sum = 0;

  float SQweight_sumx = 0;
  float SQweight_sumy = 0;

  for (UInt_t row=0; row<xPixels; ++row) {
    for (UInt_t col=0; col<yPixels; ++col) {
      int index = col*xPixels+row;
      float gray = float(argb[index]&0xff)/256.;

      h_rep->SetBinContent(row+1,yPixels-col,gray);
      
      //Intensity Cut
      if ((gray >= I_lowcut) & (gray <= I_highcut)){
	//Cut out time stamp in lower right corner
	if ((row < yPixels-100.) & (col < xPixels-100.)){

	  h_cut->SetBinContent(row+1,yPixels-col,gray);

	  weight_sumx = weight_sumx + (row+1)*gray;
	  weight_sumy = weight_sumy + (col)*gray;
	  gray_sum = gray_sum + gray;
	  
	  //Calculate for Variance of Center Coordinates
	  SQweight_sumx = SQweight_sumx + (pow((row+1),2))*gray;
	  SQweight_sumy = SQweight_sumy + (pow(col,2))*gray;

	}      
      }
    }
  }
  //Center of Intensity
  float pix_x = weight_sumx / gray_sum;
  float pix_y = weight_sumy / gray_sum;
  //Mean of the Squares
  float SQpix_x = SQweight_sumx / gray_sum;
  float SQpix_y = SQweight_sumy / gray_sum;
  //Standard Deviation
  float stdx = sqrt(SQpix_x - pow(pix_x,2));
  float stdy = sqrt(SQpix_y - pow(pix_y,2));
  //Convert Pixels to Physical Distances
  float centerx = pix_x * convert;
  float centery = pix_y * convert;

  std::cout << "Mean x: " << centerx << endl;
  std::cout << "std x: " << stdx << endl;
  std::cout << "Mean y: " << centery << endl;
  std::cout << "std y: " << stdy << endl;

  c_qcheck->cd(2);
  h_rep1->Draw("colz");
  c_qcheck->cd(3);
  h_cut1->Draw("colz");
  
  c_qcheck->cd(4);
  TH1D* h_proj1 = h_cut1->ProjectionX("h_proj1", 0, -1, "d");
  double ytop1 = h_proj1->GetBinContent( h_proj1->GetMaximumBin() );
  double ybot1 = 0.0;

  TH1D* h_empty1 = new TH1D("h_empty1", "Projection of Beam Spot Intensity Along X-Axis", 10*stdx, centerx-5*stdx, centerx+5*stdx);
  h_empty1->GetYaxis()->SetRangeUser( ybot1, ytop1*1.05 );
  h_empty1->GetXaxis()->SetTitle("x Position (Pixels)");
  h_empty1->GetYaxis()->SetTitle("Sum of Intensities at x Position");
  h_empty1->GetYaxis()->SetTitleOffset(1.1);
  h_empty1->SetLineColor(kWhite);

  h_empty1->Draw();
  h_proj1->Draw("same");
  
  TLine *beamcenter1 = new TLine(centerx, ybot1, centerx, ytop1*1.05);
  beamcenter1->SetLineColor(kRed);
  beamcenter1->Draw();
  TLine *beamstdup1 = new TLine(centerx+stdx, ybot1, centerx+stdx, ytop1*1.05);
  beamstdup1->SetLineColor(kRed);
  beamstdup1->SetLineStyle(2);
  beamstdup1->Draw();
  TLine *beamstddown1 = new TLine(centerx-stdx, ybot1, centerx-stdx, ytop1*1.05);
  beamstddown1->SetLineColor(kRed);
  beamstddown1->SetLineStyle(2);
  beamstddown1->Draw();


  c_qcheck->cd(5);
  h_cut->Draw("colz");

  c_qcheck->cd(6);
  TH1D* h_proj = h_cut->ProjectionX("h_proj", 0, -1, "d");
  double ytop = h_proj->GetBinContent( h_proj->GetMaximumBin() );
  double ybot = 0.0;

  TH1D* h_empty = new TH1D("h_empty", "Projection of Beam Spot Intensity Along X-Axis After Gaussian Blur", 10*stdx, centerx-5*stdx, centerx+5*stdx);
  h_empty->GetYaxis()->SetRangeUser( ybot, ytop*1.05 );
  h_empty->GetXaxis()->SetTitle("x Position (Pixels)");
  h_empty->GetYaxis()->SetTitle("Sum of Intensities at x Position");
  h_empty->GetYaxis()->SetTitleOffset(1.1);
  h_empty->SetLineColor(kWhite);

  h_empty->Draw();
  h_proj->Draw("same");

  TLine *beamcenter = new TLine(centerx, ybot*0.5, centerx, ytop*1.05);
  beamcenter->SetLineColor(kRed);
  beamcenter->Draw();
  TLine *beamstdup = new TLine(centerx+stdx, ybot*0.5, centerx+stdx, ytop*1.05);
  beamstdup->SetLineColor(kRed);
  beamstdup->SetLineStyle(2);
  beamstdup->Draw();
  TLine *beamstddown = new TLine(centerx-stdx, ybot*0.5, centerx-stdx, ytop*1.05);
  beamstddown->SetLineColor(kRed);
  beamstddown->SetLineStyle(2);
  beamstddown->Draw();
  /*  TCanvas *c_rep = new TCanvas();
  c_rep->Draw();
  h_rep->Draw("colz");
  */
  image1->DrawEllips(pix_x, pix_y, stdx, stdy, 1, "#FF0000", 3);

  if(qualitycheck != "")
    {
      c_qcheck->Print(qualitycheck);
    }

  if ( root_output != "" )
    {
      TFile *fout = new TFile( root_output , "RECREATE" );
      h_I->Write();
      image->Write();
      fout->Close();
    }

  return;
}
