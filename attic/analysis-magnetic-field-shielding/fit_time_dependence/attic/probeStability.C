void probeStability()
{
  TTree* tdata = new TTree();
  tdata->ReadFile("data/DataFile_160817_122836.txt","time/F:I:B");

  tdata->Draw("B");

  return;
}
