#ifndef READ_DATA
#define READ_DATA

TTree* readData( TString fname )
{
  TTree *tdata = new TTree();
  tdata->ReadFile(fname,"time/F:I:B");

  return tdata;
}

#endif
