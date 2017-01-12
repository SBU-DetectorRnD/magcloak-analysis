#ifndef BIN_DATA
#define BIN_DATA

TGraphErrors* binData( TGraphErrors* gin, int nbins, double minbin, double maxbin )
{

  /* Create TProfile to store data */
  TProfile *p1 = new TProfile("p1","",nbins,minbin,maxbin);

  /* TProfile error options:
  ""  -> bin error = standard deviation of points / sqrt(entries)
  "s" -> bin error = standard deviation of points */
  p1->SetErrorOption("");
  cout << "TProfile error option: " << p1->GetErrorOption() << endl;

  /* Fill TProfile with data from input TGraph */
  for ( int i = 0; i < gin->GetN(); i++ )
  {
    p1->Fill( gin->GetX()[i], gin->GetY()[i] );
  }

  /* Store TProfile bin values and uncertainties in vectors */
  vector< double > x_new;
  vector< double > y_new;
  vector< double > ex_new;
  vector< double > ey_new;

  for ( int i = 1; i <= nbins; i++ )
  {
    /* drop bins with < N entries */
    if ( p1->GetBinEntries( i ) < 10 )
    {
      continue;
    }

    /* append points to vectors */
    x_new.push_back( p1->GetBinCenter(i) );
    y_new.push_back( p1->GetBinContent(i) );

    /* if points in single readout bin, use 1/3 bin as uncertainty */
    if ( p1->GetBinError(i) == 0 )
    {
      cout << "Bin error 0 at " << p1->GetBinCenter(i) << endl;
      ex_new.push_back( 0. ); // estimate uncertainty
      ey_new.push_back( 0.03 ); // bin width = 2/3 of half Gaussmeter readout precision of 0.1
    }
    else{
      ex_new.push_back( 0. ); // estimate uncertainty
      ey_new.push_back( p1->GetBinError(i) );
    }
  }

  /* Create TGraphErrors from TProfile data */
  TGraphErrors *gout = new TGraphErrors(
    x_new.size(),
    &(x_new[0]),
    &(y_new[0]),
    &(ex_new[0]),
    &(ey_new[0])
  );

  /* Return TGraphErrors */
  return gout;
}

#endif
