/**
 *
 */

makePlots_fit_example()
{

  gStyle->SetOptStat(0);

  // Offset before cooling: 0.0027 +- 0.0006
  // Offset after cooling: -0.073 +- 0.0008

  //  float b_offset = ( -0.073 - 0.0027 );
  //  float b_offset_err = sqrt( 0.0006 * 0.0006 + 0.0008 * 0.0008 );;

  float b_offset = ( -0.073 );
  float b_offset_err = 0.0008;

  float b_err_meas = 0.00075;
  float b_err_resolution = 0.0005;

  float b_err = sqrt(b_err_meas*b_err_meas + b_err_resolution*b_err_resolution + b_offset_err*b_offset_err);

  //-----

  TTree *t_flat = new TTree();
  t_flat->ReadFile("DataFile_140611_142837_flat_line_example.txt","t/F:I:B");

  vector<float> v_x_flat, v_y_flat, v_ex_flat, v_ey_flat;
  t_flat->Draw( (TString("B - ") += b_offset ).Append(":t") );
  for ( int i = 0; i < t_flat->GetEntries(""); i++ )
    {
      v_x_flat.push_back(t_flat->GetV2()[i]);
      v_y_flat.push_back(t_flat->GetV1()[i]);
      v_ex_flat.push_back(0);
      v_ey_flat.push_back(b_err);
    }
  TGraphErrors* g_flat = new TGraphErrors( t_flat->GetEntries(""), &(v_x_flat[0]), &(v_y_flat[0]), &(v_ex_flat[0]), &(v_ey_flat[0]) );
  g_flat->SetLineColor(kBlue);
  g_flat->SetMarkerColor(kBlue);
  g_flat->SetMarkerStyle(20);
  g_flat->SetMarkerSize(1.1);

  //-----

  TTree* t_log = new TTree();
  //t_log->ReadFile("DataFile_140611_145727_log_example.txt","t/F:I:B");
  t_log->ReadFile("DataFile_140611_145513_log_example.txt","t/F:I:B");

  vector<float> v_x_log, v_y_log, v_ex_log, v_ey_log;
  t_log->Draw( (TString("B - ") += b_offset ).Append(":t") );
  for ( int i = 0; i < t_log->GetEntries(""); i++ )
    {
      v_x_log.push_back(t_log->GetV2()[i]);
      v_y_log.push_back(t_log->GetV1()[i]);
      v_ex_log.push_back(0);
      v_ey_log.push_back(b_err);
    }
  TGraphErrors* g_log = new TGraphErrors( t_log->GetEntries(""), &(v_x_log[0]), &(v_y_log[0]), &(v_ex_log[0]), &(v_ey_log[0]) );
  g_log->SetLineColor(kBlue);
  g_log->SetMarkerColor(kBlue);
  g_log->SetMarkerStyle(20);
  g_log->SetMarkerSize(1.1);

  //-----

  // Fit functions- results from PErl scripts
  TF1 *f_flat = new TF1("f_flat","[0]");
  f_flat->SetRange(0,100);

  //a*np.log(x + 1 + b) + c
  TF1 *f_log1 = new TF1("f_log1","[0]*log(x+1+[1])+[2]");
  f_log1->SetRange(0,100);
  f_log1->SetParameter(0,0.0212);
  f_log1->SetParameter(1,33.0944);
  f_log1->SetParameter(2,0.0762);

  //  TF1* f_log2 = new TF1();

  //  TF1* f_lin = new TF1();

  //-----

  TH1F* h_frame_flat = new TH1F("h_frame_flat","",71,-0.5,70.5);
  h_frame_flat->GetXaxis()->SetTitle("t [s]");
  h_frame_flat->GetYaxis()->SetTitle("B_{i} [mT]");
  h_frame_flat->GetYaxis()->SetTitleOffset(1.5);
  h_frame_flat->GetYaxis()->SetNdivisions(503);
  h_frame_flat->GetYaxis()->SetRangeUser(0.0,0.02);

  TH1F* h_frame_log = new TH1F("h_frame_log","",71,-0.5,70.5);
  h_frame_log->GetXaxis()->SetTitle("t [s]");
  h_frame_log->GetYaxis()->SetTitle("B_{i} [mT]");
  h_frame_log->GetYaxis()->SetTitleOffset(1.5);
  h_frame_log->GetYaxis()->SetNdivisions(503);
  h_frame_log->GetYaxis()->SetRangeUser(0.14,0.17);

  //-----
  TCanvas *c1 = new TCanvas();
  h_frame_flat->Draw("rad");
  g_flat->Draw("Psame");
  g_flat->Fit(f_flat);
  gPad->RedrawAxis();

  c1->Print("Plots/BvsT_1layer_5mT.png");
  c1->Print("Plots/BvsT_1layer_5mT.eps");

  TCanvas *c2 = new TCanvas();
  h_frame_log->Draw();
  g_log->Draw("Psame");
  g_log->Fit(f_log1);

  gPad->RedrawAxis();

  c2->Print("Plots/BvsT_1layer_21mT.png");
  c2->Print("Plots/BvsT_1layer_21mT.eps");

}
