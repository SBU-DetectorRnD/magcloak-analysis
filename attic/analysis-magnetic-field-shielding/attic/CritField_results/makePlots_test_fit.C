/**
 *
 */

makePlots_test_fit()
{

  gStyle->SetOptStat(0);

  // Offset before cooling: 0.0027 +- 0.0006
  // Offset after cooling: -0.073 +- 0.0008

  float b_offset = ( -0.073 - 0.0027 );
  float b_offset_err = sqrt( 0.0006 * 0.0006 + 0.0008 * 0.0008 );;

  float b_err_meas = 0.00075;
  float b_err_resolution = 0.0005;

  float b_err = sqrt(b_err_meas*b_err_meas + b_err_resolution*b_err_resolution + b_offset_err*b_offset_err);

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

  //a*np.log(x + 1 + b) + c
  TF1 *f_log1 = new TF1("f_log1","[0]*log(x+1+[1])+[2]");
  f_log1->SetRange(0,100);
  f_log1->SetParameter(0,0.0212);
  f_log1->SetParameter(1,33.0944);
  f_log1->SetParameter(2,0.0762);

  //  TF1* f_log2 = new TF1();

  //  TF1* f_lin = new TF1();

  //-----

  TH1F* h_frame_log = new TH1F("h_frame_log","",71,-0.5,70.5);
  h_frame_log->GetXaxis()->SetTitle("t [s]");
  h_frame_log->GetYaxis()->SetTitle("B_{i} [mT]");
  h_frame_log->GetYaxis()->SetTitleOffset(1.5);
  h_frame_log->GetYaxis()->SetNdivisions(503);
  h_frame_log->GetYaxis()->SetRangeUser(0.14,0.17);

  //-----

  TCanvas *c2 = new TCanvas();
  h_frame_log->Draw();
  g_log->Draw("Psame");
  TFitResultPtr r = g_log->Fit(f_log1,"S");

  gPad->RedrawAxis();

  double x[1] = { 3.154e7 };
  double err[1];  // error on the function at point x0

  cout << "Extrapolation: " << f_log1->Eval(x[0]) << endl;

  r->GetConfidenceIntervals(1, 1, 1, x, err, 0.683);
  cout << " function value at " << x[0] << " = " << f_log1->Eval(x[0]) << " +/- " << err[0] << endl;





  //  c2->Print("Plots/BvsT_1layer_21mT.png");
  //  c2->Print("Plots/BvsT_1layer_21mT.eps");






}
