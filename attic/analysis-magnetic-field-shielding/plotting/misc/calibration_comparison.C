int calibration_comparison()
{

	TTree *t1 = new TTree();
	t1->ReadFile("$HOME/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160629_120947.txt","time:voltage:field");
	t1->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160629_120947.txt","time:voltage:field");
	t1->Draw("voltage:field");
	t1->Draw("field:voltage");
	t1->Draw("TMath::Abs(field):voltage");
	t1->Draw("-1*field:voltage");
	TTree *t2 = new TTree();
	t2->SetMarkerColor(kRed);
	t2->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160628_201006.txt","time:voltage:field");
	t2->Draw("-1*field:voltage","","SAME");
	TTree *t3 = new TTree();
	t3->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160628_183924.txt","time:voltage:field");
	t3->SetMarkerColor(kGreen);
	t3->Draw("-1*field:voltage","","SAME");
	TTree *t4 = new TTree();
	t4->SetMarkerColor(kBlue);
	t4->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160629_132435","time:voltage:field");
	t4->ReadFile("/home/josh/Dropbox/Stony\ Brook\ Research\ Team\ Folder/LabVIEW/DATA_Gaussmeter/DataFile_160629_132435.txt","time:voltage:field");
	t4->Draw("-1*field:voltage","","SAME");

	return 0;
}
