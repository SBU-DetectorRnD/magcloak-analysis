#include "headers/exist_test.h"


int stability_hall_probe()
{

	//File to be read in should be 3 numbers in the following format: seconds_since_start voltage magnetic_field
        std::string file_path = 
			"/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160729_143845.txt";
        TTree *t = new TTree();
        TCanvas *c = new TCanvas();
        gStyle->SetOptFit();
        gStyle->SetOptStat();

	//Checks if file exists
	if(exists_test(file_path))
	{
	        t->ReadFile(file_path.c_str(),"seconds:voltage:field");
	}
	else
	{
		cerr << endl;
		cerr << "**************** ERROR: File not found ****************" << endl;
		cerr << "Please check the file path and/or allow file syncronization services (Dropbox, etc.) to finish." << endl << endl;
		c->Close();
		return -1;
	}

	//Draw graph and fit a line to it. A line may not always describe the drift perfectly, but it is usually a good approximation.
        t->Draw("field:seconds");
        TF1 *fit1 = new TF1("fit1","pol1", t->GetEntries()*0.0, t->GetEntries());
        Graph->Fit("fit1","R Q");

	//Get the slope and error of the line
        double slope = fit1->GetParameter(1);
        double slope_err = fit1->GetParError(1);

	//Gaussian fit to histogram of values... Good test of stability.
	TCanvas *c3 = new TCanvas();
	t->Draw("field");
	TF1 *fit2 = new TF1("fit2","gaus");
	htemp->Fit("fit2","Q");
	double mean = htemp->GetMean();
	double stdev = htemp->GetStdDev();

	//Draw a version of the above plot without the line on a different canvas
	TCanvas *c2 = new TCanvas();
	t->Draw("field:seconds/60");
	TLine *mean_line = new TLine(0, mean, c2->GetUxmax(), mean);
		mean_line->SetLineColor(kBlue-1);
		mean_line->Draw("SAME");
	TLine *stdev_line = new TLine(0, mean - stdev, c2->GetUxmax(), mean - stdev);
		stdev_line->SetLineColor(kBlue-2);
		stdev_line->SetLineStyle(7);
		stdev_line->Draw("SAME");
	TLine *stdev_line2 = new TLine(0, mean + stdev, c2->GetUxmax(), mean + stdev);
		stdev_line2->SetLineColor(kBlue-2);
		stdev_line2->SetLineStyle(7);
		stdev_line2->Draw("SAME");
	TLine *two_stdev_line = new TLine(0, mean - 2*stdev, c2->GetUxmax(), mean - 2*stdev);
		two_stdev_line->SetLineColor(kBlue-3);
		two_stdev_line->SetLineStyle(3);
		two_stdev_line->Draw("SAME");
	TLine *two_stdev_line2 = new TLine(0, mean + 2*stdev, c2->GetUxmax(), mean + 2*stdev);
		two_stdev_line2->SetLineColor(kBlue-3);
		two_stdev_line2->SetLineStyle(3);
		two_stdev_line2->Draw("SAME");
        c2->Update();

	//Output the mean and StDev of the data.
	cout << endl << "The Mean of the field value is:         " << mean << endl;
	cout << "The standard deviation of the field is: " << stdev << endl;

	//Output the mT/[time] slopes for different times
	cout << endl << "Assuming Linear Drift:" << endl;
	cout << "    Slope (mT/second): " << slope  << " +/- " << slope_err << endl;
	cout << "    Slope (mT/minute): " << slope*60.0   << " +/- " << slope_err*60.0 << endl;
	cout << "    Slope (mT/hour):   " << slope*60.0*60.0   << " +/- " << slope_err*60.0*60.0 << endl << endl;

	//Output some drift values for common measurements
	cout << "In a 1.5 hour dipole measurement, the probe will drift by " << slope*60.0*60.0*1.5 << " mT" << endl;
	cout << "In a 50 point ferromagnet B vs. z measurement (5 sec/point), the probe will drift by "	<< slope*5*50 << " mT" << endl;

        return 0;
}
