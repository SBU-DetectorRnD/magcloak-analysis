TGraph* smoothData( TGraphErrors* gin )
{

  TGraph* gout;

  // Kernel Smoother
  // create new kernel smoother and smooth data with bandwidth = 2.0
     TGraphSmooth *gs = new TGraphSmooth("normal");
  //   gout = gs->SmoothKern(gin,"normal",20);
  //   DrawSmooth(1,"Kernel Smoother: bandwidth = 2.0","times","accel");

  // // redraw ksmooth with bandwidth = 5.0
  //    grout = gs->SmoothKern(grin,"normal",5.0);
  //    DrawSmooth(2,"Kernel Smoother: bandwidth = 5.0","","");
  //
  // // Lowess Smoother
  // // create new lowess smoother and smooth data with fraction f = 2/3
  //    grout = gs->SmoothLowess(grin,"",0.67);
  //    DrawSmooth(3,"Lowess: f = 2/3","","");
  //
  // // redraw lowess with fraction f = 0.2
  //    grout = gs->SmoothLowess(grin,"",0.2);
  //    DrawSmooth(4,"Lowess: f = 0.2","","");
  //
  // // Super Smoother
  // // create new super smoother and smooth data with default bass = 0 and span = 0
      gout = gs->SmoothSuper(gin,"",0,0);
  //    DrawSmooth(5,"Super Smoother: bass = 0","","");
  //
  // // redraw supsmu with bass = 3 (smoother curve)
  //    grout = gs->SmoothSuper(grin,"",3);
  //    DrawSmooth(6,"Super Smoother: bass = 3","","");

//  gout = gin;
  return gout;
}
