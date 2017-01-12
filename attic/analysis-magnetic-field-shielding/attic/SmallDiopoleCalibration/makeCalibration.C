/** Script for small dipole calibration
 *
 */

makeCalibration()
{
  TTree *t0 = new TTree();
  t0->ReadFile("dipole_calibration_combined.txt","");
  t0->Print();

  TProfile* prof0 = new TProfile("prof0","",27,-0.5,26.5);

  t0->Draw("B:I >> prof0","","");

  TCanvas *c1 = new TCanvas();
  prof0->Draw("");

  c1->Print("test.C");


}

