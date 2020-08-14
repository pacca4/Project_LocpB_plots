void copytree()
{
	/*
	TString dir = "../data/skimmed/sig.root";
	gSystem->ExpandPathName(dir);
	const char* filename = gSystem->AccessPathName(dir) ? "./sig.root" : "../data/skimmed/sig.root";

	cout << filename << endl;

	TFile* oldfile = TFile::Open("../data/skimmed/sig.root");
	// TFile oldfile(filename);
	TTree* oldtree;
	oldfile->GetObject("T", oldtree);
	*/


	TFile* input = TFile::Open("../data/skimmed/sig.root");
	// TTree* inputtree;
	cout << "1 **************" << endl;
	TTree* inputtree = (TTree*) input->Get("Events");
	cout << "2 **************" << endl;
	// input->GetObject("Events", inputtree);
	TFile* output = TFile::Open("../data/skimmed/sig_reduced.root","RECREATE");
	cout << "3 **************" << endl;

	inputtree->SetBranchStatus("*",1);
	//inputtree->SetBranchStatus("Muon_dxy",0);
	//inputtree->SetBranchStatus("Muon_dxyErr",0);
	//inputtree->SetBranchStatus("Muon_dz",0);
	//inputtree->SetBranchStatus("Muon_dzErr",0);
	//inputtree->SetBranchStatus("Muon_ip3d",0);
	//inputtree->SetBranchStatus("Muon_jetPtRelv2",0);
	//inputtree->SetBranchStatus("Muon_jetRelIso",0);
	//inputtree->SetBranchStatus("Muon_miniPFRelIso_all",0);
	//inputtree->SetBranchStatus("Muon_miniPFRelIso_chg",0);
	//inputtree->SetBranchStatus("Muon_segmentComp",0);
	//inputtree->SetBranchStatus("Muon_sip3d",0);
	cout << "4 **************" << endl;

	TTree* outputtree = inputtree->CloneTree(-1,"fast");
	cout << "5 **************" << endl;
	output->Write();
	cout << "6 **************" << endl;
	delete input;
	delete output;
	cout << "7 **************" << endl;

	/*
	// Activate all branches
	oldtree->SetBranchStatus("*", 1);

	// Deactivate only four of them
	for (auto activeBranchName : {"Muon_dxy", "Muon_dxyErr", "Muon_dz", "Muon_dzErr", "Muon_ip3d", "Muon_jetPtRelv2", "Muon_jetRelIso", "Muon_miniPFRelIso_all", "Muon_miniPFRelIso_chg", "Muon_segmentComp", "Muon_sip3d", })
		oldtree->SetBranchStatus(activeBranchName, 1);

	// Create a new file + a clone of old tree in new file
	TFile newfile("sig_reduced.root", "recreate");
	auto newtree = oldtree->CloneTree();

	newtree->Print();
	newfile.Write();
	*/
}
