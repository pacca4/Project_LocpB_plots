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
#include "util.h"


void acceTrigger(const string& data_file){

    TFile* dafile = new TFile(data_file.c_str());
    TTree* da_tree = (TTree*)dafile->Get("Events");

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

   


    

}