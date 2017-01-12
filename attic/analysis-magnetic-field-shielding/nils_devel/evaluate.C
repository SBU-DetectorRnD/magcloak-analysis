#include <fstream>
#include <sstream>
#include <string>

int evaluate( std::string filelist, std::string outfile )
{
  gStyle->SetOptStat(0);

  TCanvas *ctemp = new TCanvas();

  TCanvas *cres = new TCanvas("TimeDependence");
  TH1F* hres = new TH1F("hres","",100,0,650);
  hres->GetYaxis()->SetRangeUser(0,50);
  hres->SetTitle("");
  hres->GetXaxis()->SetTitle("time (s)");
  hres->GetYaxis()->SetTitle("B_{int} (mT)");
  hres->Draw();
  leg = new TLegend(0.2,0.6,0.9,0.9);
//  leg->SetHeader("The Legend Title"); // option "C" allows to center the header
  leg->SetNColumns(5);

  vector< double > v_Bint;
  vector< double > v_BintErr;
  vector< double > v_Bext;
  vector< double > v_BextErr;

  /* Loop over all lines in input file */
  std::ifstream infilelist(filelist);
  std::string line;

  unsigned colorcounter=38;

  while (std::getline(infilelist, line))
  {
    // skip lines with '#' and empty lines
    if ( line.find("#") != string::npos )
      {
        cout << "Skip line " << line << endl;
        continue;
      }

    if ( line == "" )
      continue;

    //cout << "Processing file " << line << endl;


    TString infilename("data_calib/");
    infilename.Append(line);

    TFile *fin = new TFile( infilename );
    TTree *tin = (TTree*)fin->Get("t");

    ctemp->cd();
    tin->Draw("Bi:time");
    TGraph *gtime = new TGraph(tin->GetEntries(), &(tin->GetV2()[0]), &(tin->GetV1()[0]));
    gtime->SetLineColor(colorcounter);
    colorcounter++;

    TH1F* hBext = new TH1F("hBext","",100,0,1000);
    tin->Draw("Bo >> hBext");

    cres->cd();
    gtime->Draw("lsame");

    double Bext_i = hBext->GetMean();
    double BextErr_i = hBext->GetRMS();

    double Bint_i = gtime->Eval(590);
    double BintErr_i = 0;

    /* add legend entry */
    TString legname("B_ext ~ ");
    legname += (int)Bext_i;
    leg->AddEntry(gtime,legname,"l");

    cout << "B_ext: " << Bext_i << " \t B_int: " << Bint_i << endl;

    v_Bint.push_back(Bint_i);
    v_BintErr.push_back(BintErr_i);
    v_Bext.push_back(Bext_i);
    v_BextErr.push_back(BextErr_i);

  }

  cres->cd();
  leg->Draw();

  TGraphErrors *gfinal = new TGraphErrors(v_Bext.size(), &(v_Bext[0]), &(v_Bint[0]), &(v_BextErr[0]), &(v_BintErr[0]));
  gfinal->Sort();
  gfinal->SetName("Bint_Vs_Bext");
  gfinal->SetTitle("");
  gfinal->GetXaxis()->SetTitle("B_{ext} (mT)");
  gfinal->GetYaxis()->SetTitle("B_{int} (mT)");

  TCanvas *cfinal = new TCanvas();
  gfinal->Draw("APL");

  /* Save output graph */
  TString outfilename("output/");
  outfilename.Append(outfile);
  TFile *fout = new TFile(outfilename,"RECREATE");

  cres->Write();
  gfinal->Write();

  fout->Close();

  /* Write result to txt output file */
  TString outfilenametxt = outfilename;
  outfilenametxt.ReplaceAll(".root",".txt");

  ofstream foutxt;
  foutxt.open( outfilenametxt );
  foutxt <<  "# Bo sig_Bo Bi sig_Bi shield sig_shield sf sig_sf time_dependent" << endl;

  for ( int i = 0; i < gfinal->GetN(); i++ )
  {
    double Bo = gfinal->GetX()[i];
    double sig_Bo = gfinal->GetEX()[i];
    double Bi = gfinal->GetY()[i];
    double sig_Bi = gfinal->GetEY()[i];
    double shield = 0;
    double sig_shield = 0;
    double sf = 0;
    double sig_sf = 0;
    double time_dependent = 0;

    foutxt <<  Bo << " " << sig_Bo << " " << Bi << " " << sig_Bi << " "
         << shield << " " << sig_shield << " " << sf << " " << sig_sf
         << " " << time_dependent << endl;
  }

  return 0;
}
