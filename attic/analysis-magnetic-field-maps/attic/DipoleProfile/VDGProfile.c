/* 6/18/2014 Ben
Magnetic Cloak: Phase III
Van De Graff Profile Of the whole length
of the dipole turning magnet
*/
  VDGProfile()
  {
	//Style setting
	gStyle->SetOptStat(0);                                                                                                                                                                                                                    

	//Set up tree for the data sheet
	TTree *t1 = new TTree();                                                                                                                                                                                                                  
	t1->ReadFile("VDGProfile1.txt","x/F:y");   
	t1->Print();
	t1->SetLineColor(2);
	
	//Formatting
	TH1F* hframe = new TH1F("hframe","",10,-10,150);                                                                                                                                                                                      
	hframe->GetYaxis()->SetRangeUser(0,25); 
	hframe->GetYaxis()->SetTitle("B_{y}[mT]");  
	hframe->GetXaxis()->SetTitle("z [cm]");   
	hframe->GetYaxis()->CenterTitle();
	hframe->GetXaxis()->CenterTitle();  

	//Make canvas
	TCanvas *c1 = new TCanvas();                                                                                                                                                                                                              
	hframe->Draw();           
	gPad->RedrawAxis();
	
	//Draw it
	t1->Draw("y:x","","Lsame");   
	
	//Save it
	c1->SaveAs("VDGDipole.eps");
	c1->SaveAs("VDGDipole.png");
		
}