profile_bin_root()
{

  vector< TString > v_runs;
  v_runs.push_back("run027_1layer");
  v_runs.push_back("run028_1layer");
  v_runs.push_back("run029_1layer");
//  v_runs.push_back("run030_2layer");
//  v_runs.push_back("run031_2layer");
//  v_runs.push_back("run032_2layer");
//  v_runs.push_back("run033_3layer");
//  v_runs.push_back("run034_3layer");
//  v_runs.push_back("run035_3layer");
//  v_runs.push_back("run036_5layer");
//  v_runs.push_back("run037_5layer");
//  v_runs.push_back("run038_5layer");
//  v_runs.push_back("run042_10layer");
//  v_runs.push_back("run043_10layer");
//  v_runs.push_back("run044_10layer");
//  v_runs.push_back("run045_20layer");


  TProfile *prof1 = new TProfile("prof1","",100,0,500);

  for ( unsigned i_run = 0; i_run < v_runs.size(); i_run++ )
    {
      TTree *t1 = new TTree();
      TString sname = v_runs.at(i_run);
      sname.Append("/fit_results.txt");
      t1->ReadFile(sname,"column1/F:column2:column3:column4:column5:column6:column7:column8:column9:column10");

      //t1->Draw("(column1-column3):column1","","same");

      t1->Draw("(column1-column3):column1 >> prof1","","");
    }

  TCanvas *c2 = new TCanvas();
  TH1F* hframe = new TH1F("hframe","",50,0,500);
  hframe->GetYaxis()->SetRangeUser(0,500);
  hframe->Draw();

  prof1->Draw("prof,same");

}
