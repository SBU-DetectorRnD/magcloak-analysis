calcMeanRadius()
{

  TH1F* hr = new TH1F("hr","Outer Diameter SC Cylinder", 11, 34.45, 35.55 );

  hr->Fill(34.9);
  hr->Fill(33.9);
  hr->Fill(34.8);
  hr->Fill(35.0);
  hr->Fill(34.3);
  hr->Fill(34.8);
  hr->Fill(34.9);
  hr->Fill(34.9);
  hr->Fill(33.8);
  hr->Fill(34.0);

  hr->Draw();


}
