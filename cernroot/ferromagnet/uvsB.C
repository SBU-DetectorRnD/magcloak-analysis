#include "/home/sean/research/headers/makePlot_uvB.h"

/*
To run: root uvsB.C

Note: current version uses all ferromagnet scans and
diameters taken from the Dropbox directory

*/
int uvsB(){

  //// Cryo Diameters and Thickness ////
  TString do_cryo_574 = "fm574_do_cryo.txt";
  TString di_cryo_574 = "fm574_di_cryo.txt";
  TString do_cryo_699 = "fm699_do_cryo.txt";
  TString di_cryo_699 = "fm699_di_cryo.txt";
  TString do_cryo_625 = "fm625_do_cryo.txt";
  TString di_cryo_625 = "fm625_di_cryo.txt";
  TString do_cryo_651 = "fm651_do_cryo.txt";
  TString di_cryo_651 = "fm651_di_cryo.txt";
  TString do_cryo_104 = "fm104_do_cryo.txt";
  TString th_cryo_104 = "fm104_th_cryo.txt";
  TString do_cryo_199 = "fm199_do_cryo.txt";
  TString th_cryo_199 = "fm199_th_cryo.txt";
  TString do_cryo_590 = "fm590_do_cryo.txt";
  TString th_cryo_590 = "fm590_th_cryo.txt";
  TString do_cryo_554 = "fm554_do_cryo.txt";
  TString th_cryo_554 = "fm554_th_cryo.txt";
  TString do_cryo_618 = "fm618_do_cryo.txt";
  TString th_cryo_618 = "fm618_th_cryo.txt";
  TString do_cryo_303 = "fm303_do_cryo.txt";
  TString th_cryo_303 = "fm303_th_cryo.txt";
  TString do_cryo_409 = "fm409_do_cryo.txt";
  TString th_cryo_409 = "fm409_th_cryo.txt";
  TString do_cryo_745 = "fm745_do_cryo.txt";
  TString th_cryo_745 = "fm745_th_cryo.txt";


  
  ////Cryo Measurements With First Calibration//////
   TString calibration1 = "DataFile_170222_182044.txt";
  
  TString fm574_cryo = "DataFile_170222_163542.txt";
  TString name_574 = "574";
  bool inches_574 = false;
  

  TString fm699_cryo = "DataFile_170222_173644.txt";
  TString name_699 = "699";
  bool inches_699 = false;


  TString fm625_cryo = "DataFile_170222_180859.txt";
  TString name_625 = "625";
  bool inches_625 = false;

 
  ////Cryo Measurements With Second Calibration////

  TString calibration2 = "DataFile_170224_133120.txt";
  
  TString fm651_cryo = "DataFile_170224_145236.txt";
  TString name_651 = "651";
  bool inches_651 = false;
  
  TString fm104_cryo = "DataFile_170224_134823.txt";
  TString name_104 = "104";
  bool inches_104 = true;

  TString fm199_cryo = "DataFile_170224_140046.txt";
  TString name_199 = "199";
  bool inches_199 = true;
  
  TString fm590_cryo = "DataFile_170224_143402.txt";
  TString name_590 = "590";
  bool inches_590 = true;

  TString fm554_cryo = "DataFile_170224_152749.txt";
  TString name_554 = "554";
  bool inches_554 = true;

  TString fm618_cryo = "DataFile_170224_155104.txt";
  TString name_618 = "618";
  bool inches_618 = true;

  /// Cryo measurements third calibration ////

  TString calibration3 = "DataFile_170308_161602.txt";

  TString fm303_cryo = "DataFile_170308_170218.txt";
  TString name_303 = "303";
  bool inches_303 = true;

  TString fm409_cryo = "DataFile_170308_172608.txt";
  TString name_409 = "409";
  bool inches_409 = true;
  
  TString fm745_cryo = "DataFile_170308_174950.txt";
  TString name_745 = "745";
  bool inches_745 = true;
  
  
  
 
  
  //////Make Graph//////////
  
  TCanvas *c_uvB = new TCanvas();
  TH1 *h_uvB = c_uvB->DrawFrame(0, 0.8, 60, 5);
  h_uvB->SetTitle("Permeability of Epoxy/Steel Ferromagnets");
  h_uvB->GetXaxis()->SetTitle("B_{0} (mT)");
  h_uvB->GetXaxis()->SetTitleOffset(1.5);
  h_uvB->GetYaxis()->SetTitle("#mu_{cryo}");
  h_uvB->GetYaxis()->SetTitleOffset(1.0);
  TLegend *leg_uvB = new TLegend(0.55,0.65,0.65,0.87);
  leg_uvB->SetNColumns(1);
  

  
  /////////////Plot Permeabilities///////////
  vector<double> u_val,u_err;
  double Fm[10]= {0.104,0.199,0.303,0.409,0.554,0.590,0.618,0.651,0.699,0.745};

  ////cryo temp best fitting plots////////
  makePlot_uvB(calibration2,fm104_cryo,name_104,th_cryo_104,do_cryo_104,inches_104,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration2,fm199_cryo,name_199,th_cryo_199,do_cryo_199,inches_199,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration3,fm303_cryo,name_303,th_cryo_303,do_cryo_303,inches_303,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration3,fm409_cryo,name_409,th_cryo_409,do_cryo_409,inches_409,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration2,fm554_cryo,name_554,th_cryo_554,do_cryo_554,inches_554,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration2,fm590_cryo,name_590,th_cryo_590,do_cryo_590,inches_590,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration2,fm618_cryo,name_618,th_cryo_618,do_cryo_618,inches_618,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration2,fm651_cryo,name_651,di_cryo_651,do_cryo_651,inches_651,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration1,fm699_cryo,name_699,di_cryo_699,do_cryo_699,inches_699,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration3,fm745_cryo,name_745,th_cryo_745,do_cryo_745,inches_745,*leg_uvB,u_val,u_err);
 

  leg_uvB->Draw();
  

 
  
  /////////Plot permeability versus fractional mass///////////  
  TCanvas *c_2 = new TCanvas();
  TH1 *f = c_2->DrawFrame(0, .8, 2, 3.5);
  
  
 
  gr = new TGraphErrors(10,&Fm[0],&u_val[0],0,&u_err[0]);
  gr->Draw("AP");
  
  
  TF1 *fit = new TF1("fit", "[0]/TMath::Tan([1]*x + [2]) + [3]", 0, 1.0);
  fit->SetParameter(0, 2.0);
  fit->SetParameter(1, 1.0);
  fit->SetParameter(2, 1.0);
  fit->SetParameter(3, 1.0);
  gr->Fit("fit");
 

  gr->SetTitle("");
  gr->GetXaxis()->SetTitle("Fm");
  gr->GetYaxis()->SetTitle("#mu_{cryo}");

  cout <<gr->GetFunction("fit")->GetChisquare() << endl;
  cout << gr->GetFunction("fit")->GetNDF() << endl;
  

  return 0;
 
}
