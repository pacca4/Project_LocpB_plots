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
#include "cutParams.h"


//root -l -b '../../fitModelRQ_v15.cc (" ", ...); exit(0)'
void mumuG_cuts( const string& file,  const string& outfile ) {

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
    

    //Create a new file + a clone of old tree in new file
    TFile *newfile = new TFile(("Cutted_"+outfile+".root").c_str(),"recreate");
    TTree *newtree = oldtree->CloneTree(0);

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

    //bool hasAngles=( oldtree->GetListOfBranches()->Contains("Muon_angle") &&
    //                 oldtree->GetListOfBranches()->Contains("Photon_angle") );
//
    //float mu_ang, ph_ang;
    //if (!hasAngles){
    //    auto MuBranch = oldtree->Branch("Muon_angle",   &mu_ang, "mu_ang/F");
    //    auto PhBranch = oldtree->Branch("Photon_angle", &ph_ang, "ph_ang/F");
    //} 
    
    int saved=0;

    //while(myread.Next()){
    for (Long64_t i=0;i<oldtree->GetEntries(); i++) {
        myread.Next();
        bool save=true;
        if(abs(mm_mass[0]-Consts::massJPsi)>cutParams::massJPsi_diff) save=false;
        if(  muon_ch[0]==muon_ch[1])             save=false;
        if(  muon_pt[0]<cutParams::muonPt_min   )save=false; //cut over the least energetic muon(of the ones considered for inv mass)
        if(photon_pt[0]<cutParams::photonPt_min )save=false;
        //eta variables are not so interesting to cut
        //if(abs(muon_eta[0])>mu_eta || abs(muon_eta[1])>mu_eta  ) save=false;
        //if(abs(photon_eta[0])>g_eta  ) save=false;
        if(mmg_mass[0] > cutParams::massMuMuG_max || 
           mmg_mass[0] < cutParams::massMuMuG_min ) save=false;
        
        //if(hasAngles){
        //    if( jAng>cutParams::JPsiGAngle_max ||  jAng < cutParams::JPsiGAngle_min ) save=false;
        //    if(muAng>cutParams::MuMuAngle_max  || muAng < cutParams::MuMuAngle_min  ) save=false;
        //}
        float jAng= JpsiGamma_angle(muon_pt[0]  , muon_phi[0]  , muon_eta[0],
                                    muon_pt[1]  , muon_phi[1]  , muon_eta[1],
                                    photon_pt[0], photon_phi[0], photon_eta[0]);
        float muAng=MuMu_angle(muon_pt[0], muon_phi[0], muon_eta[0],
                               muon_pt[1], muon_phi[1], muon_eta[1] );
        
       // if(usetrig){
       //     cout<<trig_disp[0]<<endl;
       // }
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