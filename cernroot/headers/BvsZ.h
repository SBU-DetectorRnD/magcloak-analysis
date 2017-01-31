int color_BvsZ = 2;				//Make sure no plots are the same color in the same macro

int BvsZ_plot(
		std::string title, 		//Title of plot. Will be used in drawning legend entry
		std::string file_path,		//Path to input file
		std::string file_name, 		//Name of input file
		TCanvas &c00, 			//Canvas to draw on
		TLegend &leg, 			//Legend of plot
		double z_offset, 		//Factor to multiply z position by (to convert between mm and cm for example)
		double z_scale, 		//Factor to offset z position by
		double field_offset, 		//Factor to offset field by
		double field_scale)		//Factor to multiply the field by (-1 to invert for example)
{

	cout << endl << "Beginning: " << file_name << endl;
	cout << "     " << title << endl;
	TTree *t = new TTree();
	std::string file = file_path + file_name;

	//Check to see if file exists. If so read in file into TTree.
	if(exists_test(file))
	{
		t->ReadFile(file.c_str(),"z:field:x1:x2:x3");
	}
	else
	{
		cerr << "ERROR: File not found" << endl;
		return -1;
	}

	//Create new branches for offsets and multiplicative factors
	TBranch *b_field_offset = t->Branch("field_offset",&field_offset,"pt/D");
	TBranch *b_field_scale = t->Branch("field_scale",&field_scale,"pt/D");
	TBranch *b_z_offset = t->Branch("z_offset",&z_offset,"pt/D");
	TBranch *b_z_scale = t->Branch("z_scale",&z_scale,"pt/D");

	//Plot the data with appropriate offsets
	c00.cd();
	t->SetMarkerColor(color_BvsZ);
	t->SetLineColor(color_BvsZ);
	color_BvsZ++;
	if (color_BvsZ == 5) color_BvsZ++;
	t->Draw("(field_scale*field)-field_offset:(z_scale*z)-z_offset", "", " pl SAME");

	//Add this plots entry to the legend
	leg.AddEntry(t,title.c_str(),"pl");
	
	cout << "All done with: " << file_name << endl;

	return 0;
}


double current_normalization_denom = 99999;

int BvsZ_plot_Inorm(
		std::string title, 		//Title of plot. Will be used in drawning legend entry
		std::string file_path,		//Path to input file
		std::string file_name, 		//Name of input file
		TCanvas &c00, 			//Canvas to draw on
		TLegend &leg, 			//Legend of plot
		double z_offset, 		//Factor to multiply z position by (to convert between mm and cm for example)
		double z_scale, 		//Factor to offset z position by
		double field_offset, 		//Factor to offset field by
		double field_scale)		//Factor to multiply the field by (-1 to invert for example)
{

	cout << endl << "Beginning: " << file_name << endl;
	cout << "     " << title << endl;
	TTree *t = new TTree();
	std::string file = file_path + file_name;

	//Check to see if file exists. If so read in file into TTree.
	if(exists_test(file))
	{
		t->ReadFile(file.c_str(),"z:field:x1:current:x3");
	}
	else
	{
		cerr << "ERROR: File not found" << endl;
		return -1;
	}

	if(current_normalization_denom == 99999) 
	{
		cout << "      USING THIS AS REFERENCE CURRENT FOR NORMALIZATION" << endl;
		TCanvas *c2 = new TCanvas();
		c2->cd();
		t->Draw("current:z");
		TGraph *gh = (TGraph*)c2->GetListOfPrimitives()->FindObject("Graph");
		current_normalization_denom = gh->GetMean(2);
		c2->Close();
	}

	TCanvas *c2 = new TCanvas();
	c2->cd();
	t->Draw("current:z");
	TGraph *gh2 = (TGraph*)c2->GetListOfPrimitives()->FindObject("Graph");
	double current_normalization_num = gh2->GetMean(2);
	c2->Close();

	double current_normalization_factor = current_normalization_num/current_normalization_denom;
	cout << current_normalization_factor << endl;	

	//Create new branches for offsets and multiplicative factors
	TBranch *b_field_offset = t->Branch("field_offset",&field_offset,"pt/D");
	TBranch *b_field_scale = t->Branch("field_scale",&field_scale,"pt/D");
	TBranch *b_z_offset = t->Branch("z_offset",&z_offset,"pt/D");
	TBranch *b_z_scale = t->Branch("z_scale",&z_scale,"pt/D");
	TBranch *b_norm = t->Branch("current_normalization_factor",&current_normalization_factor,"pt/D");

	//Plot the data with appropriate offsets
	c00.cd();
	t->SetMarkerColor(color_BvsZ);
	t->SetLineColor(color_BvsZ);
	color_BvsZ++;
	if (color_BvsZ == 5) color_BvsZ++;
	t->Draw("((field_scale*field)-field_offset)/current_normalization_factor:(z_scale*z)-z_offset", "", " pl SAME");

	//Add this plots entry to the legend
	leg.AddEntry(t,title.c_str(),"pl");
	
	cout << "All done with: " << file_name << endl;

	return 0;
}


int BvsZ_plot(std::string title, std::string file_path, std::string file_name, TCanvas &c00, TLegend &leg)
{

	cout << endl << "Beginning: " << file_name << endl;
	cout << "Warning: No offset set for file: " << file_name << endl;

	TTree *t = new TTree();
	std::string file = file_path + file_name;
	//Check to see if file exists
	if(exists_test(file))
	{
		t->ReadFile(file.c_str(),"z:field");
	}
	else
	{
		cerr << "ERROR: File not found" << endl;
		return -1;
	}

	c00.cd();
	t->SetMarkerColor(color_BvsZ);
	t->SetLineColor(color_BvsZ);
	color_BvsZ++;
	t->Draw("field:z", "", "SAME");

	leg.AddEntry(t,title.c_str(),"pl");

	cout << "All done with: " << file_name << endl;
	return 0;
}


int BvsZ_plot(std::string title, std::string file_path, std::string file_name)
{

	cout << endl << "Beginning: " << file_name << endl;

	cout << "      Warning: No offset set for file: " << file_name << endl;
	cout << "      Warning: No canvas set for file: " << file_name << ". Plotting by itself." << endl;

	TCanvas *c1 = new TCanvas();
	TTree *t = new TTree();
	std::string file = file_path + file_name;
	//Check to see if file exists
	if(exists_test(file))
	{
		t->ReadFile(file.c_str(),"z:field");
	}
	else
	{
		cerr << "ERROR: File not found" << endl;
		return -1;
	}

	t->SetMarkerColor(color_BvsZ);
	t->SetLineColor(color_BvsZ);
	color_BvsZ++;
	t->Draw("field:z");

	cout << "All done with: " << file_name << endl;
	return 0;
}

