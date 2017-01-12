/**
 *
 */
makePlot_field_v1()
{

  vector <string> field_refs;
  field_refs.push_back("2");
  field_refs.push_back("5");
  field_refs.push_back("10");
  field_refs.push_back("15");

  TCanvas *ctemp = new TCanvas();

  TH1F* h_frame_z = new TH1F("h_frame_z","",180,-100,100);
  h_frame_z->GetYaxis()->SetTitle("B_{y} [mT]");
  h_frame_z->GetXaxis()->SetTitle("z - z_{c} [mm]");
  h_frame_z->GetXaxis()->SetTitleOffset(1.5);

  TH1F* h_frame_x = new TH1F("h_frame_x","",180,-100,100);
  h_frame_x->GetYaxis()->SetTitle("B_{y} [mT]");
  h_frame_x->GetXaxis()->SetTitle("x - x_{c} [mm]");
  h_frame_x->GetXaxis()->SetTitleOffset(1.5);

  gStyle->SetOptStat(0);

  for( int i = 0; i < field_refs.size(); i++ )
    {
      string field_ref = field_refs.at(i);

      h_frame_z->GetYaxis()->SetRangeUser(0,15.1);
      h_frame_x->GetYaxis()->SetRangeUser(0,15.1);

      ctemp->cd();

      /* get input trees */
      TTree *t1_warm = new TTree();
      TTree *t2_warm = new TTree();
      TTree *t3_warm = new TTree();

      TTree *t1_cold = new TTree();
      TTree *t2_cold = new TTree();
      TTree *t3_cold = new TTree();

      if ( field_ref == "2" )
        {
          t1_warm->ReadFile("Input_Data/Data_2014_06_20_BvsZ_x128_y98_2mT_warm.txt","");
          t2_warm->ReadFile("Input_Data/Data_2014_06_20_BvsX_y98_z158_2mT_warm.txt","");
          t3_warm->ReadFile("Input_Data/Data_2014_06_20_BvsX_y70_z158_2mT_warm.txt","");

          t1_cold->ReadFile("Input_Data/Data_2014_06_20_BvsZ_x128_y98_2mT.txt","");
          t2_cold->ReadFile("Input_Data/Data_2014_06_20_BvsX_y98_z158_2mT.txt","");
          t3_cold->ReadFile("Input_Data/Data_2014_06_20_BvsX_y70_z158_2mT.txt","");
        }
      else if ( field_ref == "5" )
        {
          t1_warm->ReadFile("Input_Data/Data_2014_06_20_BvsZ_x128_y98_5mT_warm.txt","");
          t2_warm->ReadFile("Input_Data/Data_2014_06_23_BvsX_y98_z158_5mT_warm.txt","");
          t3_warm->ReadFile("Input_Data/Data_2014_06_23_BvsX_y70_z158_5mT_warm.txt","");

          t1_cold->ReadFile("Input_Data/Data_2014_06_20_BvsZ_x128_y98_5mT.txt","");
          t2_cold->ReadFile("Input_Data/Data_2014_06_20_BvsX_y98_z158_5mT.txt","");
          t3_cold->ReadFile("Input_Data/Data_2014_06_20_BvsX_y70_z158_5mT.txt","");
        }
      else if ( field_ref == "10" )
        {
          t1_warm->ReadFile("Input_Data/Data_2014_06_20_BvsZ_x128_y98_10mT_warm.txt","");
          t2_warm->ReadFile("Input_Data/Data_2014_06_23_BvsX_y98_z158_10mT_warm.txt","");
          t3_warm->ReadFile("Input_Data/Data_2014_06_23_BvsX_y70_z158_10mT_warm.txt","");

          t1_cold->ReadFile("Input_Data/Data_2014_06_20_BvsZ_x128_y98_10mT.txt","");
          t2_cold->ReadFile("Input_Data/Data_2014_06_20_BvsX_y98_z158_10mT.txt","");
          t3_cold->ReadFile("Input_Data/Data_2014_06_20_BvsX_y70_z158_10mT.txt","");
        }
      else if ( field_ref == "15" )
        {
          t1_warm->ReadFile("Input_Data/Data_2014_06_20_BvsZ_x128_y98_15mT_warm.txt","");
          t2_warm->ReadFile("Input_Data/Data_2014_06_20_BvsX_y98_z158_15mT_warm.txt","");
          t3_warm->ReadFile("Input_Data/Data_2014_06_20_BvsX_y70_z158_15mT_warm.txt","");

          t1_cold->ReadFile("Input_Data/Data_2014_06_20_BvsZ_x128_y98_15mT.txt","");
          t2_cold->ReadFile("Input_Data/Data_2014_06_20_BvsX_y98_z158_15mT.txt","");
          t3_cold->ReadFile("Input_Data/Data_2014_06_20_BvsX_y70_z158_15mT.txt","");
        }

      /* create graphs */
      t1_warm->Draw("B-(B_I0 - B_I0_T0) - B_I0_T0:z-158");
      TGraphErrors* g1_warm = new TGraphErrors( t1_warm->GetEntries(""), t1_warm->GetV2(), t1_warm->GetV1() );
      g1_warm->SetMarkerColor(kRed);

      t2_warm->Draw("B-(B_I0 - B_I0_T0) - B_I0_T0:x-128-3");
      TGraphErrors* g2_warm = new TGraphErrors( t2_warm->GetEntries(""), t2_warm->GetV2(), t2_warm->GetV1() );
      g2_warm->SetMarkerColor(kRed);

      t3_warm->Draw("B-(B_I0 - B_I0_T0) - B_I0_T0:x-128-3");
      TGraphErrors* g3_warm = new TGraphErrors( t3_warm->GetEntries(""), t3_warm->GetV2(), t3_warm->GetV1() );
      g3_warm->SetMarkerColor(kRed);

      t1_cold->Draw("B-(B_I0 - B_I0_T0) - B_I0_T0:z-158");
      TGraphErrors* g1_cold = new TGraphErrors( t1_cold->GetEntries(""), t1_cold->GetV2(), t1_cold->GetV1() );
      g1_cold->SetMarkerColor(kBlue);

      t2_cold->Draw("B-(B_I0 - B_I0_T0) - B_I0_T0:x-128-3");
      TGraphErrors* g2_cold = new TGraphErrors( t2_cold->GetEntries(""), t2_cold->GetV2(), t2_cold->GetV1() );
      g2_cold->SetMarkerColor(kBlue);

      t3_cold->Draw("B-(B_I0 - B_I0_T0) - B_I0_T0:x-128-3");
      TGraphErrors* g3_cold = new TGraphErrors( t3_cold->GetEntries(""), t3_cold->GetV2(), t3_cold->GetV1() );
      g3_cold->SetMarkerColor(kBlue);


      // -----
      /* Create ratio plots */
      // ... ?


      // -----
      /* lines */
      TLine *lLeft_z  = new TLine(112 - 158, 0, 112 - 158, 15.1);
      TLine *lRight_z = new TLine(202 - 158, 0, 202 - 158, 15.1);

      TLine *lLeft_x  = new TLine(-17.3, 0, -17.3, 15.1);
      TLine *lRight_x = new TLine( 17.3, 0,  17.3, 15.1);

      lLeft_z->SetLineStyle(2);
      lRight_z->SetLineStyle(2);

      lLeft_x->SetLineStyle(2);
      lRight_x->SetLineStyle(2);

      // -----

      /* Update legend */
      TLegend *leg1 = new TLegend(0.75,0.75,0.9,0.9);
      leg1->SetNColumns(1);
      leg1->AddEntry(g1_warm,"warm","p");
      leg1->AddEntry(g1_cold,"cold","p");

      TLegend *leg2 = new TLegend(0.75,0.75,0.9,0.9);
      leg2->SetNColumns(1);
      leg2->AddEntry(g2_warm,"warm","p");
      leg2->AddEntry(g2_cold,"cold","p");

      TLegend *leg3 = new TLegend(0.75,0.75,0.9,0.9);
      leg3->SetNColumns(1);
      leg3->AddEntry(g3_warm,"warm","p");
      leg3->AddEntry(g3_cold,"cold","p");

      // -----

      TCanvas *c1 = new TCanvas();
      h_frame_z->Draw("rad");
      lLeft_z->Draw("same");
      lRight_z->Draw("same");
      leg1->Draw();
      g1_warm->Draw("LPsame");
      g1_cold->Draw("LPsame");
      gPad->RedrawAxis();

      TCanvas *c2 = new TCanvas();
      h_frame_x->Draw("rad");
      lLeft_x->Draw("same");
      lRight_x->Draw("same");
      leg2->Draw();
      g2_warm->Draw("Psame");
      g2_cold->Draw("Psame");
      gPad->RedrawAxis();

      TCanvas *c3 = new TCanvas();
      h_frame_x->Draw("rad");
      lLeft_x->Draw("same");
      lRight_x->Draw("same");
      leg3->Draw();
      g3_warm->Draw("Psame");
      g3_cold->Draw("Psame");
      gPad->RedrawAxis();

      c1->Print( TString("../Plots/Public/FieldDisturbance_HelmholtzCoils_v1_warm_cryo_").Append(field_ref.c_str()).Append("mT_1.eps") );
      c1->Print( TString("../Plots/Public/FieldDisturbance_HelmholtzCoils_v1_warm_cryo_").Append(field_ref.c_str()).Append("mT_1.png") );

      c2->Print( TString("../Plots/Public/FieldDisturbance_HelmholtzCoils_v1_warm_cryo_").Append(field_ref.c_str()).Append("mT_2.eps") );
      c2->Print( TString("../Plots/Public/FieldDisturbance_HelmholtzCoils_v1_warm_cryo_").Append(field_ref.c_str()).Append("mT_2.png") );

      c3->Print( TString("../Plots/Public/FieldDisturbance_HelmholtzCoils_v1_warm_cryo_").Append(field_ref.c_str()).Append("mT_3.eps") );
      c3->Print( TString("../Plots/Public/FieldDisturbance_HelmholtzCoils_v1_warm_cryo_").Append(field_ref.c_str()).Append("mT_3.png") );

    }
}
