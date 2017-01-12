#ifndef LIN_2_LOG
#define LIN_2_LOG

TGraphErrors* lin2log( TGraphErrors *gin )
{
  TGraphErrors *gout = (TGraphErrors*) gin->Clone("gout");

  for ( int p = 0; p < gout->GetN(); p++ )
  {
    gout->SetPoint( p, log(gout->GetX()[p]), gout->GetY()[p] );
  }

  return gout;
}

#endif
