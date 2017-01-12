makePlots_uncertainty()
{
  TTree *t1 = new TTree();
  //  t1->ReadFile("data_2013_12_05_FieldAtCylinderSurfce.dat","");
  t1->ReadFile("data_2013_11_26_FieldFixCurrent.dat","");
  t1->Print();

  t1->Draw("Bcenter","I>0.01");

}
