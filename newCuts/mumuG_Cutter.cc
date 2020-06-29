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
#include "cutParamsCutter.h"

//this macro is supposed to be runned after the mumuGselector
//all the branch will be saved
//root -l -b '../../fitModelRQ_v15.cc (" ", ...); exit(0)'
void mumuG_Cutter( const string& file,  const string& outfile ) {

    TFile *oldfile = new TFile(file.c_str());
    TTree *oldtree = (TTree*)oldfile->Get("Events");
   
    oldtree->SetBranchStatus("*",1);
    
    //oldtree->SetBranchStatus("nMuon",1);

    //oldtree->SetBranchStatus("Muon_pt",1);
    //oldtree->SetBranchStatus("Muon_eta",1);
    //oldtree->SetBranchStatus("Muon_phi",1);
    //oldtree->SetBranchStatus("Muon_charge",1);
    //oldtree->SetBranchStatus("Muon_mass",1);
//
    //oldtree->SetBranchStatus("Photon_pt",1);
    //oldtree->SetBranchStatus("Photon_eta",1);
    //oldtree->SetBranchStatus("Photon_phi",1);
    ////#oldtree->SetBranchStatus("Photon_charge",1);
    //oldtree->SetBranchStatus("Photon_mass",1);
//
    //oldtree->SetBranchStatus("MuMu_invMass",1);
    //oldtree->SetBranchStatus("MuMuG_invMass",1);
    //oldtree->
    

    //Create a new file + a clone of old tree in new file
    TFile *newfile = new TFile(("Cutted_"+outfile+".root").c_str(),"recreate");
    TTree *newtree_noT   = oldtree->CloneTree(0); //no trigger selection
    TTree *newtree_T1    = oldtree->CloneTree(0); //selectio with HLT_IsoMu27
    TTree *newtree_T2    = oldtree->CloneTree(0); //selection with HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ
    TTree *newtree_T1_T2 = oldtree->CloneTree(0); //selection with triggers logic OR
    TTree *newtree_T12   = oldtree->CloneTree(0); //selection with trigger logic AND

    newtree_T1    -> SetName("Events_TriggerIsoMu27");
    newtree_T2    -> SetName("Events_TriggerMu17Mu8Iso");    
    newtree_T1_T2 -> SetName("Events_TriggerOR");
    newtree_T12   -> SetName("Events_TriggerAND");

    TTreeReader myread(oldtree);
	TTreeReaderArray<Float_t> muon_pt     = {myread, "Muon_pt"       };
	TTreeReaderArray<Int_t>   muon_ch     = {myread, "Muon_charge"   }; 
	TTreeReaderArray<Float_t> muon_eta    = {myread, "Muon_eta"      };
	TTreeReaderArray<Float_t> muon_phi    = {myread, "Muon_phi"      };
	TTreeReaderArray<Float_t> muon_mass   = {myread, "Muon_mass"     };
	TTreeReaderArray<Float_t> photon_pt   = {myread, "Photon_pt"     };
    TTreeReaderArray<Float_t> photon_phi  = {myread, "Photon_phi"    };
	TTreeReaderArray<Float_t> photon_eta  = {myread, "Photon_eta"    };
	TTreeReaderArray<Float_t> jpsi_pt     = {myread, "Jpsi_pt"       };
    TTreeReaderArray<Float_t> jpsi_phi    = {myread, "Jpsi_phi"      };
	TTreeReaderArray<Float_t> jpsi_eta    = {myread, "Jpsi_eta"      };
	TTreeReaderArray<Float_t> photon_mass = {myread, "Photon_mass"   };
	TTreeReaderArray<Float_t> mm_mass     = {myread, "MuMu_invMass"  };
	TTreeReaderArray<Float_t> mmg_mass    = {myread, "MuMuG_invMass" };
    TTreeReaderArray<Float_t> mAng        = {myread, "mumuAng"       };
    TTreeReaderArray<Float_t> JAng        = {myread, "JpsiGAng"      };
	TTreeReaderArray<Bool_t>  Trig1       = {myread, "HLT_IsoMu27"   };
	TTreeReaderArray<Bool_t>  Trig2       = {myread, "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ" };
	
    int saved=0, save1=0, save2=0, saveOR=0, saveAND=0;

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
        
        //
        if( JAng[0] > cutParams::JPsiGAngle_max ||  
            JAng[0] < cutParams::JPsiGAngle_min ) save=false;
        if( mAng[0] > cutParams::MuMuAngle_max  || 
            mAng[0] < cutParams::MuMuAngle_min  ) save=false;

        float dphi=abs(jpsi_phi[0]-photon_phi[0]);
        float deta=abs(jpsi_eta[0]-photon_eta[0]);
        float dR =sqrt(pow(dphi,2)+pow(deta,2));
        if(dphi<cutParams::JpsiGdPhi_min) save=false;
        if(dR  <cutParams::JpsiGdR_min  ) save=false;

        float dR_mu0G=sqrt(pow(muon_phi[0]-photon_phi[0],2)+
                           pow(muon_eta[0]-photon_eta[0],2));
        float dR_mu1G=sqrt(pow(muon_phi[1]-photon_phi[0],2)+
                           pow(muon_eta[1]-photon_eta[0],2));
        if (dR_mu0G<cutParams::muGdR_min ||
            dR_mu1G<cutParams::muGdR_min  ) save=false; 

        if(save){
            oldtree->GetEntry(i);
            newtree_noT->Fill(); saved++;
            if(Trig1[0]==1 )                {newtree_T1    ->Fill(); save1++;  }
            if(Trig2[0]==1 )                {newtree_T2    ->Fill(); save2++;  }
            if(Trig1[0]==1 || Trig2[0]==1 ) {newtree_T1_T2 ->Fill(); saveOR++; }
            if(Trig1[0]==1 && Trig2[0]==1 ) {newtree_T12   ->Fill(); saveAND++;}
        }      
    }

    cout<<"Total saved: "<<saved   <<" -> "<<float(saved  )*100/(oldtree->GetEntries())<<"%\n";
    cout<<"T1    saved: "<<save1   <<" -> "<<float(save1  )*100/(oldtree->GetEntries())<<"%\n";
    cout<<"T2    saved: "<<save2   <<" -> "<<float(save2  )*100/(oldtree->GetEntries())<<"%\n";
    cout<<"T1|T2 saved: "<<saveOR  <<" -> "<<float(saveOR )*100/(oldtree->GetEntries())<<"%\n";
    cout<<"T1&t2 saved: "<<saveAND <<" -> "<<float(saveAND)*100/(oldtree->GetEntries())<<"%\n";

    //newtree->Draw("MuMuG_invMass");
    //newtree->Print();
    newfile->Write();
    //delete oldfile;
    //delete newfile;
    return;
}