makePlots_critFieldPeak_v1()
{

  TGraphErrors* g_critField = new TGraphErrors(4);

  // use mean of gauss
  g_critField->SetPoint(1,1,31.4);
  g_critField->SetPoint(2,2,52.7);
  g_critField->SetPoint(3,3,64.3);
  g_critField->SetPoint(4,4,81.7);

  // use mean error of gauss
//  g_critField->SetPointError(1,0,0.01);
//  g_critField->SetPointError(2,0,0.01);
//  g_critField->SetPointError(3,0,0.02);
//  g_critField->SetPointError(4,0,0.02);

  // use sigma of gauss
//  g_critField->SetPointError(1,0,15.5);
//  g_critField->SetPointError(2,0,26.0);
//  g_critField->SetPointError(3,0,29.7);
//  g_critField->SetPointError(4,0,35.7);

  // use point distance
  g_critField->SetPointError(1,0,1);
  g_critField->SetPointError(2,0,1);
  g_critField->SetPointError(3,0,1);
  g_critField->SetPointError(4,0,1);


  g_critField->Draw("AP");

}
