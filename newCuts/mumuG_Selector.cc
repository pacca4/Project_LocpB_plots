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
#include "cutParamsLight.h"


//root -l -b '../../fitModelRQ_v15.cc (" ", ...); exit(0)'
void mumuG_Selector( const string& file,  const string& outfile ) {

    TFile *oldfile = new TFile(file.c_str());
    TTree *oldtree = (TTree*)oldfile->Get("Events");
    oldtree->SetBranchStatus("*",0);
    
    //oldtree->SetBranchStatus("nMuon",1);

    oldtree->SetBranchStatus("Muon_pt",1);
    oldtree->SetBranchStatus("Muon_eta",1);
    oldtree->SetBranchStatus("Muon_phi",1);
    oldtree->SetBranchStatus("Muon_charge",1);
    oldtree->SetBranchStatus("Muon_mass",1);

    oldtree->SetBranchStatus("Photon_pt",1);
    oldtree->SetBranchStatus("Photon_eta",1);
    oldtree->SetBranchStatus("Photon_phi",1);
    //#oldtree->SetBranchStatus("Photon_charge",1);
    oldtree->SetBranchStatus("Photon_mass",1);

    oldtree->SetBranchStatus("MuMu_invMass",1);
    oldtree->SetBranchStatus("MuMuG_invMass",1);



    //triggers
    auto list = oldtree->GetListOfBranches();
    if(list->Contains("HLT_Dimuon20_Jpsi")) oldtree->SetBranchStatus("HLT_Dimuon20_Jpsi",1);
    if(list->Contains("HLT_DoubleMu0")) oldtree->SetBranchStatus("HLT_DoubleMu0",1);
    if(list->Contains("HLT_Dimuon0er16_Jpsi_NoOS_NoVertexing")) oldtree->SetBranchStatus("HLT_Dimuon0er16_Jpsi_NoOS_NoVertexing",1);
    if(list->Contains("HLT_Dimuon0er16_Jpsi_NoVertexing")) oldtree->SetBranchStatus("HLT_Dimuon0er16_Jpsi_NoVertexing",1);
    if(list->Contains("HLT_Dimuon6_Jpsi_NoVertexing")) oldtree->SetBranchStatus("HLT_Dimuon6_Jpsi_NoVertexing",1);
    if(list->Contains("HLT_DoubleMu18NoFiltersNoVtx")) oldtree->SetBranchStatus("HLT_DoubleMu18NoFiltersNoVtx",1);
    if(list->Contains("HLT_Dimuon16_Jpsi")) oldtree->SetBranchStatus("HLT_Dimuon16_Jpsi",1);
    if(list->Contains("HLT_Dimuon10_Jpsi_Barrel")) oldtree->SetBranchStatus("HLT_Dimuon10_Jpsi_Barrel",1);
    
        //Create a new file + a clone of old tree in new file
    TFile *newfile = new TFile(("Cutted_"+outfile+".root").c_str(),"recreate");
    TTree *newtree = oldtree->CloneTree(0);
    //new branches
    float mumuAng, JpsiGAng, Jpsi_eta, Jpsi_pt, Jpsi_phi;
    auto BrmumuAng  = newtree->Branch("mumuAng" , &mumuAng , "mumuAng/F"  );  
    auto BrJpsiGAng = newtree->Branch("JpsiGAng", &JpsiGAng, "JpsiGAng/F" );  
    auto BrJpsi_eta = newtree->Branch("Jpsi_eta", &Jpsi_eta, "Jpsi_eta/F" );  
    auto BrJpsi_pt  = newtree->Branch("Jpsi_pt" , &Jpsi_pt , "Jpsi_pt/F"  ); 
    auto BrJpsi_phi = newtree->Branch("Jpsi_phi", &Jpsi_phi, "Jpsi_phi/F" );   


    TTreeReader myread(oldtree);
	TTreeReaderArray<Float_t> muon_pt     = {myread, "Muon_pt"       };
	TTreeReaderArray<Int_t>   muon_ch     = {myread, "Muon_charge"   }; 
	TTreeReaderArray<Float_t> muon_eta    = {myread, "Muon_eta"      };
	TTreeReaderArray<Float_t> muon_phi    = {myread, "Muon_phi"      };
	TTreeReaderArray<Float_t> muon_mass   = {myread, "Muon_mass"     };
	TTreeReaderArray<Float_t> photon_pt   = {myread, "Photon_pt"     };
    TTreeReaderArray<Float_t> photon_phi  = {myread, "Photon_phi"    };
	TTreeReaderArray<Float_t> photon_eta  = {myread, "Photon_eta"    };
	TTreeReaderArray<Float_t> photon_mass = {myread, "Photon_mass"   };
	TTreeReaderArray<Float_t> mm_mass     = {myread, "MuMu_invMass"  };
	TTreeReaderArray<Float_t> mmg_mass    = {myread, "MuMuG_invMass" };
	//TTreeReaderArray<Bool_t>  trig_disp   = {myread, "HLT_DoubleMu4_3_Jpsi_Displaced" };


    int saved=0;

    for (Long64_t i=0;i<oldtree->GetEntries(); i++) {
        myread.Next();
        bool save=true;
        if(abs(mm_mass[0]-Consts::massJPsi)>cutParams::massJPsi_diff) save=false;
        if(  muon_ch[0]==muon_ch[1])             save=false;
        if(  muon_pt[0]<cutParams::muonPt0_min   )save=false; //cut over the least energetic muon(of the ones considered for inv mass)
        if(  muon_pt[1]<cutParams::muonPt1_min   )save=false;
        if(photon_pt[0]<cutParams::photonPt_min )save=false;
        //eta variables are not so interesting to cut
        //if(abs(muon_eta[0])>mu_eta || abs(muon_eta[1])>mu_eta  ) save=false;
        if(abs(photon_eta[0])>cutParams::photonEta_max  ) save=false;
        if(mmg_mass[0] > cutParams::massMuMuG_max || 
           mmg_mass[0] < cutParams::massMuMuG_min ) save=false;
        
        //compute angles
        JpsiGAng = JpsiGamma_angle(muon_pt[0]  , muon_phi[0]  , muon_eta[0],
                                    muon_pt[1]  , muon_phi[1]  , muon_eta[1],
                                    photon_pt[0], photon_phi[0], photon_eta[0]);
        mumuAng =MuMu_angle(muon_pt[0], muon_phi[0], muon_eta[0],
                               muon_pt[1], muon_phi[1], muon_eta[1] );
        if( JpsiGAng > cutParams::JPsiGAngle_max ||  
            JpsiGAng < cutParams::JPsiGAngle_min ) save=false;
        if( mumuAng  > cutParams::MuMuAngle_max  || 
            mumuAng  < cutParams::MuMuAngle_min  ) save=false;
        //compute Jpsi variables
        cart_to_spher( muon_pt[0] +muon_pt[1] ,muon_phi[0]+muon_phi[1] ,muon_eta[0]+muon_eta[1] ,  
                       Jpsi_pt, Jpsi_phi, Jpsi_eta );

        if(save){
            oldtree->GetEntry(i);
            newtree->Fill();
            saved++;
        }      
    }

    cout<<"Total saved: "<<saved<<" -> "<<float(saved)*100/(oldtree->GetEntries())<<"%\n";

    //newtree->Draw("MuMuG_invMass");
    //newtree->Print();
    newfile->Write();
    //delete oldfile;
    //delete newfile;
    return;
}



//HLT_Dimuon20_Jpsi acc %: 55.4944
//HLT_DoubleMu0 acc %: 56.7406
//HLT_Dimuon0er16_Jpsi_NoOS_NoVertexing acc %: 50.7693
//HLT_Dimuon0er16_Jpsi_NoVertexing acc %: 49.506
//HLT_Dimuon6_Jpsi_NoVertexing acc %: 62.2059
//HLT_DoubleMu18NoFiltersNoVtx acc %: 24.8517
//HLT_Dimuon16_Jpsi acc %: 56.0414
//HLT_Dimuon10_Jpsi_Barrel acc %: 39.0694