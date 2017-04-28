#include "makePlot_uvB.h"


int uvsB(){

  //// Cryo Diameters and Thickness ////
  TString do_cryo_574 = "fm574_do_cryo.txt";
  TString di_cryo_574 = "fm574_th_cryo.txt";
  TString do_cryo_699 = "fm699_do_cryo.txt";
  TString di_cryo_699 = "fm699_th_cryo.txt";
  TString do_cryo_625 = "fm625_do_cryo.txt";
  TString di_cryo_625 = "fm625_th_cryo.txt";
  TString do_cryo_651 = "fm651_do_cryo.txt";
  TString di_cryo_651 = "fm651_th_cryo.txt";
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

  //// Room Temp Diameters and Thickness////
  TString do_room_303 = "fm303_do_room.txt";
  TString di_room_303 = "fm303_th_room.txt";
  TString do_room_409 = "fm409_do_room.txt";
  TString di_room_409 = "fm409_th_room.txt";
  TString do_room_104 = "fm104_do_room.txt";
  TString di_room_104 = "fm104_th_room.txt";
  TString do_room_199 = "fm199_do_room.txt";
  TString di_room_199 = "fm199_th_room.txt";
  TString do_room_503 = "fm554_do_room.txt";
  TString di_room_503 = "fm554_th_room.txt";
  TString do_room_548 = "fm548_do_room.txt";
  TString di_room_548 = "fm548_th_room.txt";
  TString do_room_554 = "fm554_do_room.txt";
  TString di_room_554 = "fm554_th_room.txt";
  TString do_room_574 = "fm574_do_room.txt";
  TString di_room_574 = "fm574_th_room.txt";
  TString do_room_590 = "fm590_do_room.txt";
  TString di_room_590 = "fm590_th_room.txt";
  TString do_room_602 = "fm590_do_room.txt";
  TString di_room_602 = "fm590_th_room.txt";
  TString do_room_612 = "fm618_do_room.txt";
  TString di_room_612 = "fm618_th_room.txt";
  TString do_room_618 = "fm618_do_room.txt";
  TString di_room_618 = "fm618_th_room.txt";
  TString do_room_625 = "fm625_do_room.txt";
  TString di_room_625 = "fm625_th_room.txt";
  TString do_room_651 = "fm651_do_room.txt";
  TString di_room_651 = "fm651_th_room.txt";
  TString do_room_673 = "fm673_do_room.txt";
  TString di_room_673 = "fm673_th_room.txt";
  TString do_room_699 = "fm699_do_room.txt";
  TString di_room_699 = "fm699_th_room.txt";
  TString do_room_745 = "fm745_do_room.txt";
  TString th_room_745 = "fm745_th_room.txt";
  

  
  ////Cryo Measurements With First Calibration//////
   TString calibration1 = "DataFile_170222_182044.txt";
  
  TString fm574_cryo = "DataFile_170222_163542.txt";
  TString name_574_cryo = "574";
  

  TString fm699_cryo = "DataFile_170222_173644.txt";
  TString name_699_cryo = "699";


  TString fm625_cryo = "DataFile_170222_180859.txt";
  TString name_625_cryo = "625";

 
  ////Cryo Measurements With Second Calibration////

  TString calibration2 = "DataFile_170224_133120.txt";
  
  TString fm651_cryo = "DataFile_170224_145236.txt";
  TString name_651_cryo = "651";
  
  TString fm104_cryo = "DataFile_170224_134823.txt";
  TString name_104_cryo = "104";

  TString fm199_cryo = "DataFile_170224_140046.txt";
  TString name_199_cryo = "199";
  
  TString fm590_cryo = "DataFile_170224_143402.txt";
  TString name_590_cryo = "590";

  TString fm554_cryo = "DataFile_170224_152749.txt";
  TString name_554_cryo = "554";

  TString fm618_cryo = "DataFile_170224_155104.txt";
  TString name_618_cryo = "618";

  /// Cryo measurements third calibration ////

  TString calibration3 = "DataFile_170308_161602.txt";

  TString fm303_cryo = "DataFile_170308_170218.txt";
  TString name_303_cryo = "303";
  

  TString fm409_cryo = "DataFile_170308_172608.txt";
  TString name_409_cryo = "409";
 
  
  TString fm745_cryo1 = "DataFile_170308_174950.txt";
  TString fm745_cryo2 = "DataFile_170324_192856.txt";
  TString fm745_cryo3 = "DataFile_170324_193413.txt";
  TString name_745_3_cryo = "745";
  
  
  
  
  //////Check room temp permeabilities from paper//////
  TString calibration4 = "DataFile_170228_170204.txt";
  TString fm303_room = "DataFile_170228_171013.txt";
  TString name_303_room = "303";
  
  
  TString fm409_room = "DataFile_170228_173752.txt";
  TString name_409_room = "409";
  
  TString calibration5 = "DataFile_161102_181415.txt";
  TString fm104_room = "DataFile_161102_183041.txt";
  TString name_104_room = "104";
  

  TString calibration6 = "DataFile_161007_194516.txt";
  TString fm199_room = "DataFile_161007_205423.txt";
  TString name_199_room = "199";
  

  
  TString calibration7 = "DataFile_161006_144403.txt";
  TString fm503_room = "DataFile_161006_151934.txt";
  TString name_503_room = "503";

  
  TString fm548_room = "DataFile_161007_201544.txt";
  TString name_548_room = "548";
  

  TString fm554_room = "DataFile_161102_183814.txt";
  TString name_554_room = "554";
  

  TString fm574_room = "DataFile_161102_182323.txt";
  TString name_574_room = "574";
  

  TString calibration8 = "DataFile_160727_162618.txt";
  TString fm602_room = "DataFile_160727_163533.txt";
  TString name_602_room = "602";
  

  TString calibration9 = "DataFile_160805_142716.txt";
  TString fm612_room = "DataFile_160805_143508.txt";
  TString name_612_room = "612";
  

  TString fm625_room = "DataFile_161006_150305.txt";
  TString name_625_room = "625";
  

  TString calibration10 = "DataFile_160916_211714.txt";
  TString fm651_room = "DataFile_160916_212729.txt";
  TString name_651_room = "651";
 

  TString calibration11 = "DataFile_160928_193601.txt";
  TString fm673_room = "DataFile_160928_200210.txt";
  TString name_673_room = "673";
  
  

  
  /// More recent room temperature permeabilities ///
  TString calibration12 = "DataFile_170303_132135.txt";
 
  TString fm590_room = "DataFile_170303_145218.txt";
  TString name_590_room = "590";

  TString fm618_room = "DataFile_170303_141544.txt";
  TString name_618_room = "618";
 
  TString fm699_room = "DataFile_170303_152553.txt";
  TString name_699_room = "699";

  TString calibration13 = "DataFile_170330_131038.txt";

  TString fm745_room = "DataFile_170330_132332.txt";
  TString name_745_room = "745";
  
  
  //////Make Graph//////////
  
  TCanvas *c_uvB = new TCanvas();
  TH1 *h_uvB = c_uvB->DrawFrame(0, 0, 60, 6);
  h_uvB->SetTitle("Permeability of Ferromagnets");
  h_uvB->GetXaxis()->SetTitle("B_{0} (mT)");
  h_uvB->GetXaxis()->SetTitleOffset(1.5);
  h_uvB->GetYaxis()->SetTitleOffset(1.0);
  TLegend *leg_uvB = new TLegend(0.55,0.65,0.65,0.87);
  leg_uvB->SetNColumns(1);
  

  
  /////////////Plot Permeabilities///////////
  vector<double> u_val,u_err;
  
  ////cryo temp best fitting plots////////
  TString u_title = "#mu_{cryo}";
  h_uvB->GetYaxis()->SetTitle("#mu_{cryo}");
  const unsigned npoints=10;
  double Fm[npoints]= {0.104,0.199,0.303,0.409,0.554,0.590,0.618,0.651,0.699,0.745};
  makePlot_uvB(calibration2,fm104_cryo,name_104_cryo,th_cryo_104,do_cryo_104,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration2,fm199_cryo,name_199_cryo,th_cryo_199,do_cryo_199,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration3,fm303_cryo,name_303_cryo,th_cryo_303,do_cryo_303,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration3,fm409_cryo,name_409_cryo,th_cryo_409,do_cryo_409,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration2,fm554_cryo,name_554_cryo,th_cryo_554,do_cryo_554,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration2,fm590_cryo,name_590_cryo,th_cryo_590,do_cryo_590,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration2,fm618_cryo,name_618_cryo,th_cryo_618,do_cryo_618,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration2,fm651_cryo,name_651_cryo,di_cryo_651,do_cryo_651,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration1,fm699_cryo,name_699_cryo,di_cryo_699,do_cryo_699,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration3,fm745_cryo3,name_745_3_cryo,th_cryo_745,do_cryo_745,*leg_uvB,u_val,u_err);
  
  /*
   ////////Room Temp best fitting plots//////
   TString u_title = "#mu_{room}";
  h_uvB->GetYaxis()->SetTitle("#mu_{room}");
  const unsigned npoints=17;
  double Fm[npoints]= {0.104,0.199,0.303,0.409,0.503,0.548,0.554,0.574,0.590,0.602,0.612,0.618,0.625,0.651,0.673,0.699,0.745};
  makePlot_uvB(calibration5,fm104_room,name_104_room,di_room_104,do_room_104,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration6,fm199_room,name_199_room,di_room_199,do_room_199,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration4,fm303_room,name_303_room,di_room_303,do_room_303,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration4,fm409_room,name_409_room,di_room_409,do_room_409,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration7,fm503_room,name_503_room,di_room_503,do_room_503,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration7,fm548_room,name_548_room,di_room_548,do_room_548,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration7,fm554_room,name_554_room,di_room_554,do_room_554,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration7,fm574_room,name_574_room,di_room_574,do_room_574,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration12,fm590_room,name_590_room,di_room_590,do_room_590,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration8,fm602_room,name_602_room,di_room_602,do_room_602,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration9,fm612_room,name_612_room,di_room_612,do_room_612,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration12,fm618_room,name_618_room,di_room_618,do_room_618,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration9,fm625_room,name_625_room,di_room_625,do_room_625,*leg_uvB,u_val,u_err);  
  makePlot_uvB(calibration10,fm651_room,name_651_room,di_room_651,do_room_651,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration11,fm673_room,name_673_room,di_room_673,do_room_673,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration12,fm699_room,name_699_room,di_room_699,do_room_699,*leg_uvB,u_val,u_err);
  makePlot_uvB(calibration13,fm745_room,name_745_room,th_room_745,do_room_745,*leg_uvB,u_val,u_err);
  */
  
 

  leg_uvB->Draw();
  

 
  
  /////////Plot permeability versus fractional mass///////////  
  TCanvas *c_2 = new TCanvas();
  TH1 *f = c_2->DrawFrame(0, .8, 2, 3.5);
  
   
  TGraphErrors *gr = new TGraphErrors(npoints,&Fm[0],&u_val[0],0,&u_err[0]);
  gr->Draw("AP");
  
  
  TF1 *fit = new TF1("fit", "[0]/tan([1]*x + [2]) + [3]", 0, 1.0);
  fit->SetParameter(0, 2.0);
  fit->SetParameter(1, 1.0);
  fit->SetParameter(2, 1.0);
  fit->SetParameter(3, 1.0);

    
  TFitResultPtr func = gr->Fit("fit","S");
  double p0 = func->Value(0);
  double p1 = func->Value(1);
  double p2 = func->Value(2);
  double p3 = func->Value(3);
  double p0_err = func->Error(0);
  double p1_err = func->Error(1);
  double p2_err = func->Error(2);
  double p3_err = func->Error(3);
  
  gr->GetXaxis()->SetTitle("Fm");
  gr->GetYaxis()->SetTitle( h_uvB->GetYaxis()->GetTitle() );
  gr->SetTitle("");

  
  double chi2 = gr->GetFunction("fit")->GetChisquare();
  double ndf = gr->GetFunction("fit")->GetNDF();

  cout<<"Chi2 = "<<chi2<<endl;
  cout<<"NDF = "<<ndf<<endl;
  cout<<"Chi2/NDF = "<<chi2/ndf<<endl;
  
  double res[npoints], res_err[npoints];
  double fit_err;

  TCanvas *c_3 = new TCanvas();  
  
  for(int i=0;i<npoints;i++){
    res[i] = (u_val[i]-(p0/tan(p1*Fm[i] + p2)+p3))/(p0/tan(p1*Fm[i] + p2)+p3);
    fit_err = pow(p0_err/tan(p1*Fm[i]+p2),2)+pow(p1_err*p0*Fm[i]/pow(sin(p1*Fm[i]+p2),2),2)+pow(p2_err*p0/pow(sin(p1*Fm[i]+p2),2),2)+pow(p3_err,2);
    res_err[i] = sqrt(pow(u_err[i]/(p0/tan(p1*Fm[i] + p2)+p3),2)+pow(u_val[i]/pow((p0/tan(p1*Fm[i] + p2)+p3),2),2)*fit_err);
  }

  c_3->cd();
  TGraphErrors *fr = new TGraphErrors(npoints,&Fm[0],&res[0],0,&u_err[0]);
  fr->Draw("AP");
  TString Title = "(fit-"+u_title+")/fit";
  fr->GetYaxis()->SetTitle(Title);
  fr->GetXaxis()->SetTitle("Fm");
  fr->GetYaxis()->SetTitleOffset(1.6);
  fr->SetTitle("Residuals");

  
  return 0;
 
}


