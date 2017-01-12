//Credit to these functions to Rene Brun. Some modifications by me.
// https://root.cern.ch/phpBB3/viewtopic.php?t=5520

void shade(TCanvas &c1, TF1 &f1, TF1 &f2)
{
	//shade the area between f1 and f2. Thanks Rene!
	c1.cd();

	//create a TGraph to store the function values
	//shaded area is the fill/color/style of f1
	TGraph *gr = new TGraph();
	gr->SetFillColor(f1.GetLineColor());
	gr->SetFillStyle(3013);

	//get picture range
//	Double_t xmin = c1.GetUxmin();
//	Double_t xmax = c1.GetUxmax();
//	Double_t ymin = c1.GetUymin();
//	Double_t ymax = c1.GetUymax();

	Double_t xmin = 0;
	Double_t xmax = 1500;
	Double_t ymin = 0;
	Double_t ymax = 1500;

	//process first function
	Int_t npoints=0;
	Double_t dx = (xmax-xmin)/10000;
	Double_t x = xmin+0.5*dx;
	while (x <= xmax) 
	{
		Double_t y = f1.Eval(x);
		if (y < ymin) y = ymin;
		if (y > ymax) y = ymax;
		gr->SetPoint(npoints,x,y);
		npoints++;
		x += dx;
	}
	//process second function
	x = xmax-0.5*dx;
	while (x >= xmin) 
	{
		Double_t y = f2.Eval(x);
		if (y < ymin) y = ymin;
		if (y > ymax) y = ymax;
		gr->SetPoint(npoints,x,y);
		npoints++;
		x -= dx;
	}
	gr->Draw("f SAME");     //draw graph with fill area option
}


void shade(TCanvas &c1, TF1 &f1, TF1 &f2, double xmin, double xmax, double ymin, double ymax)
{
	//shade the area between f1 and f2. Thanks Rene!
	c1.cd();

	//create a TGraph to store the function values
	//shaded area is the fill/color/style of f1
	TGraph *gr = new TGraph();
	gr->SetFillColor(f1.GetLineColor());
	gr->SetFillStyle(3013);

	//process first function
	Int_t npoints=0;
	Double_t dx = (xmax-xmin)/10000;
	Double_t x = xmin+0.5*dx;
	while (x <= xmax) 
	{
		Double_t y = f1.Eval(x);
		if (y < ymin) y = ymin;
		if (y > ymax) y = ymax;
		gr->SetPoint(npoints,x,y);
		npoints++;
		x += dx;
	}
	//process second function
	x = xmax-0.5*dx;
	while (x >= xmin) 
	{
		Double_t y = f2.Eval(x);
		if (y < ymin) y = ymin;
		if (y > ymax) y = ymax;
		gr->SetPoint(npoints,x,y);
		npoints++;
		x -= dx;
	}
	gr->Draw("f SAME");     //draw graph with fill area option
}


void shade(TCanvas &c1, TF1 &f1, TF1 &f2, TF1 &f3) 
{
	//shade the area between f1 and f2 and draw f3 on top
	c1.cd();
	//create a TGraph to store the function values
	//shaded area is the fill/color/style of f1
	TGraph *gr = new TGraph();
	gr->SetFillColor(f1.GetFillColor());
	gr->SetFillStyle(f1.GetFillStyle());
	f3->Draw("l");
	c1->Update();
	//get picture range
	Double_t xmin = c1.GetUxmin();
	Double_t xmax = c1.GetUxmax();
	Double_t ymin = c1.GetUymin();
	Double_t ymax = c1.GetUymax();

	//process first function
	Int_t npx = f3.GetNpx();
	Int_t npoints=0;
	Double_t dx = (xmax-xmin)/npx;
	Double_t x = xmin+0.5*dx;
	while (x <= xmax) 
	{
		Double_t y = f1.Eval(x);
		if (y < ymin) y = ymin;
		if (y > ymax) y = ymax;
		gr->SetPoint(npoints,x,y);
		npoints++;
		x += dx;
	}
	//process second function
	x = xmax-0.5*dx;
	while (x >= xmin) 
	{
		Double_t y = f2.Eval(x);
		if (y < ymin) y = ymin;
		if (y > ymax) y = ymax;
		gr->SetPoint(npoints,x,y);
		npoints++;
		x -= dx;
	}
	gr->Draw("f SAME");     //draw graph with fill area option
	f3->Draw("pl SAME"); //superimpose function
}
