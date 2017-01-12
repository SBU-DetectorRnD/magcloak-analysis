

beamspot_nocompile( TString fname="webcam/2016_07_19/IMG_2016_07_19T09_28_26_SN_0219.JPG", TString qualitycheck="",TString rootname="" )
{

  gSystem->Load("beamspot_analysis_C.so");

  beamspot_analysis(fname,qualitycheck,rootname);

}
