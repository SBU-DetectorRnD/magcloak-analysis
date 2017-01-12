plot_BvZ_1()
{
  gStyle->SetOptStat(0);

  TTree* t_MagnetOn_noCryo_1 = new TTree();
  t_MagnetOn_noCryo_1->ReadFile("data_bvz_copy/DataFile_160614_104404_MagnetOn_noCryo.txt", "z/F:B:dB:U:dU");
  t_MagnetOn_noCryo_1->Draw("B:z-600");
  TGraph *g_MagnetOn_noCryo_1 = new TGraph( t_MagnetOn_noCryo_1->GetEntries(), t_MagnetOn_noCryo_1->GetV2(), t_MagnetOn_noCryo_1->GetV1() );
  g_MagnetOn_noCryo_1->SetMarkerColor(kRed);
  g_MagnetOn_noCryo_1->SetLineColor(kRed);

  TTree* t_noMagnet_noCryo_1 = new TTree();
  t_noMagnet_noCryo_1->ReadFile("data_bvz_copy/DataFile_160614_102654_noMagnet_noCryo.txt", "z/F:B:dB:U:dU");
  t_noMagnet_noCryo_1->Draw("B:z-600");
  TGraph *g_noMagnet_noCryo_1 = new TGraph( t_noMagnet_noCryo_1->GetEntries(), t_noMagnet_noCryo_1->GetV2(), t_noMagnet_noCryo_1->GetV1() );

  TTree* t_MagnetOn_Cryo_1 = new TTree();
  t_MagnetOn_Cryo_1->ReadFile("data_bvz_copy/DataFile_160613_164327_MagnetOn_Cryo.txt", "z/F:B:dB:U:dU");
  t_MagnetOn_Cryo_1->Draw("-1*B:z-600");
  TGraph *g_MagnetOn_Cryo_1 = new TGraph( t_MagnetOn_Cryo_1->GetEntries(), t_MagnetOn_Cryo_1->GetV2(), t_MagnetOn_Cryo_1->GetV1() );
  g_MagnetOn_Cryo_1->SetMarkerColor(kBlue);
  g_MagnetOn_Cryo_1->SetLineColor(kBlue);

  TTree* t_MagnetOn_Cryo_2 = new TTree();
  t_MagnetOn_Cryo_2->ReadFile("data_bvz_copy/DataFile_160613_170851_MagnetOn_Cryo.txt", "z/F:B:dB:U:dU");
  t_MagnetOn_Cryo_2->Draw("-1*B:z-600");
  TGraph *g_MagnetOn_Cryo_2 = new TGraph( t_MagnetOn_Cryo_2->GetEntries(), t_MagnetOn_Cryo_2->GetV2(), t_MagnetOn_Cryo_2->GetV1() );
  g_MagnetOn_Cryo_2->SetMarkerColor(kYellow);
  g_MagnetOn_Cryo_2->SetLineColor(kYellow);

  TH1F* hframe = new TH1F("hframe","",100,-630,630);
  hframe->SetLineColor(kWhite);
  hframe->GetYaxis()->SetRangeUser(-1,25);
  hframe->GetXaxis()->SetNdivisions(505);
  hframe->GetXaxis()->SetTitle("position [mm]");
  hframe->GetYaxis()->SetTitle("B [mT]");

  TCanvas *c1 = new TCanvas();
  hframe->Draw();
  g_MagnetOn_noCryo_1->Draw("lpsame");
  g_noMagnet_noCryo_1->Draw("lpsame");
  g_MagnetOn_Cryo_1->Draw("lpsame");
  //  g_MagnetOn_Cryo_2->Draw("lpsame");

  c1->Print("beam_shield_mark3_BvZ_1.png");
}
