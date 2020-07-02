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



bool compare(const pair<string, float>& t1, const pair<string,float>& t2){
    return (t1.second > t2.second);
}


void TriggerAcceptance(const string& trig_file, const string& data_file){


	vector<std::pair<string, float>> trigAcc={};
	
  	TFile* dafile = new TFile(data_file.c_str());
    TTree* da_tree = (TTree*)dafile->Get("Events");

	TH1F *h = new TH1F("h","h",2,0,2);

	ifstream in(trig_file);
	ofstream out ("TrigAcce.txt");
	string trig;
	while(getline(in, trig)){
		delete h;
		h = new TH1F("h","h",2,0,2);
		
		da_tree->Draw((trig+">>h").c_str(),"","goff");
		float acc = float(h->GetBinContent(2))/(h->GetEntries())*100;
		cout << trig << " acc %: "<<acc<<endl;    
		std::pair<string, float> p(trig, acc);
		trigAcc.push_back(p);		
	}
	std::sort(trigAcc.begin(), trigAcc.end(), compare);
	for (auto p:trigAcc) out<<p.first<<" acc % "<<p.second<<endl;

	

	
	//TString trigs[]={};
	//while(getline(in, trig)){
	//	trigs.Append(trig.c_str());
	//}



/*
    //array di stringhe per il trigger
    TString trigs[] ={
            "HLT_Dimuon13_PsiPrime",
            "HLT_Dimuon13_Upsilon",
            "HLT_Dimuon20_Jpsi", 
            "HLT_DoubleMu33NoFiltersNoVtx",
            "HLT_DoubleMu38NoFiltersNoVtx", 
            "HLT_DoubleMu23NoFiltersNoVtxDisplaced", 
            "HLT_DoubleMu28NoFiltersNoVtxDisplaced", 
            "HLT_DoubleMu0", 
            "HLT_DoubleMu4_3_Bs", 
            "HLT_DoubleMu4_3_Jpsi_Displaced",
            "HLT_DoubleMu4_JpsiTrk_Displaced", 
            "HLT_DoubleMu4_LowMassNonResonantTrk_Displaced", 
            "HLT_DoubleMu3_Trk_Tau3mu", 
            "HLT_DoubleMu4_PsiPrimeTrk_Displaced", 
            "HLT_Dimuon0er16_Jpsi_NoOS_NoVertexing", 
            "HLT_Dimuon0er16_Jpsi_NoVertexing", 
            "HLT_Dimuon6_Jpsi_NoVertexing", 
            "HLT_DoubleMu18NoFiltersNoVtx",
            "HLT_Dimuon0_Jpsi_Muon", 
            "HLT_Dimuon0_Upsilon_Muon", 
            "HLT_DoubleMu8_Mass8_PFHT250", 
            "HLT_DoubleMu8_Mass8_PFHT300",
            "HLT_DoubleMu3_PFMET50",
            "HLT_Dimuon16_Jpsi", 
            "HLT_Dimuon10_Jpsi_Barrel",
            "HLT_Dimuon8_PsiPrime_Barrel",
            "HLT_Dimuon8_Upsilon_Barrel", 
            "HLT_Dimuon0_Phi_Barrel"};

    TH1F *h = new TH1F("h","h",2,0,2);
    for(auto T:trigs){
        delete h;
        h = new TH1F("h","h",2,0,2);

        da_tree->Draw((T+">>h"),"","goff");
        float acc = float(h->GetBinContent(2))/(h->GetEntries())*100;
        cout << T << " acc %: "<<acc<<endl;
        
        
        }

   */


    

}
