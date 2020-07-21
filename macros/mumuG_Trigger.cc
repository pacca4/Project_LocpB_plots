#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <vector>

//this macro is supposed to be runned after the mumuGselector
//all the branch will be saved
//root -l -b '../../fitModelRQ_v15.cc (" ", ...); exit(0)'
void mumuG_Trigger( const string& file,  const string& outfile ) {
/*
the trigger selcted are
1- IsoMu27
2- HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ
3- mixed triggers 
 HLT_DoubleMu20_7_Mass0to30_Photon23 and HLT_Mu12_Photon25_CaloIdL
*/

    TFile *oldfile = new TFile(file.c_str());
    TTree *oldtree = (TTree*)oldfile->Get("Events");
   
    oldtree->SetBranchStatus("*",1);
 	oldtree->SetBranchStatus("HLT_*",0);
    oldtree->SetBranchStatus("HLT_IsoMu27", 1);
    oldtree->SetBranchStatus("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ", 1);

    //Create a new file + a clone of old tree in new file
    TFile *newfile = new TFile(("Cutted_"+outfile+".root").c_str(),"recreate");
    TTree *newtree_Iso    = oldtree->CloneTree(0); //no trigger selection
    TTree *newtree_TrkIso = oldtree->CloneTree(0); //no trigger selection
    TTree *newtree_2MuPh  = oldtree->CloneTree(0); //no trigger selection

    newtree_Iso    -> SetName("Events_IsoMu");
	newtree_TrkIso -> SetName("Events_IsoTrk");
	newtree_2MuPh  -> SetName("Events_2MuPh");

	auto l = oldtree->GetListOfBranches();

    TTreeReader myread(oldtree);
    TTreeReaderArray<Bool_t> Trig1 = {myread, "HLT_IsoMu27" };
	TTreeReaderArray<Bool_t> Trig2 = {myread, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ" };

    TTreeReaderArray<Bool_t> *Trig3;

    if(l->Contains("HLT_DoubleMu20_7_Mass0to30_Photon23")) {
    	oldtree->SetBranchStatus("HLT_DoubleMu20_7_Mass0to30_Photon23", 1);
    	Trig3 = new TTreeReaderArray<Bool_t> (myread, "HLT_DoubleMu20_7_Mass0to30_Photon23" );
   	}
	else {
    	oldtree->SetBranchStatus("HLT_Mu12_Photon25_CaloIdL", 1);
		Trig3 = new TTreeReaderArray<Bool_t> (myread, "HLT_Mu12_Photon25_CaloIdL" );
	}

	for (Long64_t i=0;i<oldtree->GetEntries(); i++) {
			myread.Next();
			if (Trig1[0]==1) { oldtree->GetEntry(i); newtree_Iso    ->Fill(); }
			if (Trig2[0]==1) { oldtree->GetEntry(i); newtree_TrkIso ->Fill(); }
			if (Trig3->At(0)==1) { oldtree->GetEntry(i); newtree_2MuPh  ->Fill(); }
	}


	/*
    int saved=0;

	if(l->Contains("HLT_DoubleMu20_7_Mass0to30_Photon23")){
		TTreeReaderArray<Bool_t> Trig1 = {myread, "HLT_DoubleMu20_7_Mass0to30_Photon23" };
		for (Long64_t i=0;i<oldtree->GetEntries(); i++) {
			myread.Next();
			if (Trig1[0]==1) { oldtree->GetEntry(i); newtree->Fill(); saved++; }
	}} else {
		TTreeReaderArray<Bool_t> Trig1 = {myread, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ" };
		for (Long64_t i=0;i<oldtree->GetEntries(); i++) {
			myread.Next();
			if (Trig1[0]==1) { oldtree->GetEntry(i); newtree->Fill(); saved++; }
	}}*/

/*
  
	int saved=0;

    for (Long64_t i=0;i<oldtree->GetEntries(); i++) {
        myread.Next();
		//cout<<"\n******************\n";
		//cout<<l->Contains("HLT_DoubleMu20_7_Mass0to30_Photon23")<<endl;
		//cout<<Trig1[0]<<"\t"<<Trig2[0]<<endl;
		//cout<<"\n******************\n";
        
        if (l->Contains("HLT_DoubleMu20_7_Mass0to30_Photon23"))
        	 { if (Trig1[0]==1) { newtree->Fill(); saved++; }}
        else { if (Trig2[0]==1) { newtree->Fill(); saved++; }}  
    }*/

    //cout<<"Total saved: "<<saved   <<" -> "<<float(saved  )*100/(oldtree->GetEntries())<<"%\n";
    //newtree->Draw("MuMuG_invMass");
    //newtree->Print();
    newfile->Write();
    //delete oldfile;
    //delete newfile;
    return;
}
