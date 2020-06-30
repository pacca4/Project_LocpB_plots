#include "TCanvas.h"
#include "TAxis.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;


void DrawPad( const string& data_file,  const string& mc_file, const string& tree, 
              const string& feature,    const string& constraint="" ){
    TFile* dafile = new TFile(data_file.c_str());
    TFile* mcfile = new TFile(  mc_file.c_str());

    TTree* da_tree = (TTree*)dafile->Get(tree.c_str());
    TTree* mc_tree = (TTree*)mcfile->Get(tree.c_str());


    da_tree->Draw(feature.c_str(), constraint.c_str(), "");
    TH1F *da_h =(TH1F*)da_tree->GetHistogram(); 
    mc_tree->Draw(feature.c_str(), constraint.c_str(), "HIST same");
    TH1F *mc_h =(TH1F*)mc_tree->GetHistogram();
    mc_h->SetLineColor(kGreen); da_h->SetLineColor(kRed);
    mc_h->SetLineWidth(3);      da_h->SetLineWidth(3);
    mc_h->Scale(double(da_tree->GetEntries())/mc_tree->GetEntries());

    auto legend = new TLegend(0.5,0.8,0.6,0.9);
    legend->AddEntry(da_h, "data", "l");
    legend->AddEntry(mc_h, "MC",   "l");
    legend->Draw();

    TH1F *hist = (TH1F*)gPad->GetPrimitive("htemp");
    hist->SetTitle((tree+": "+feature).c_str());

    //da_h->Draw();
    //mc_h->Draw();
    
}


void DrawCanvas( const string& data_file,  const string& mc_file,  
                 const string& feature,    const string& constraint="" ){
    
    string feat = feature;
    feat.erase(std::remove(feat.begin(), feat.end(), '['), feat.end());
    feat.erase(std::remove(feat.begin(), feat.end(), ']'), feat.end());

    TCanvas *c=new TCanvas ("c",feature.c_str(),1800,1200);
    c->Divide(3,2);
    c->SetTitle((feature+"TriggerComparison").c_str());
    c->cd(1); DrawPad(data_file, mc_file, "Events",                   feature, constraint );
    c->cd(2); DrawPad(data_file, mc_file, "Events_TriggerIsoMu27",    feature, constraint );
    c->cd(3); DrawPad(data_file, mc_file, "Events_TriggerMu17Mu8Iso", feature, constraint );
    c->cd(4); DrawPad(data_file, mc_file, "Events_TriggerOR",         feature, constraint );
    c->cd(5); DrawPad(data_file, mc_file, "Events_TriggerAND",        feature, constraint );
    
    gStyle->SetLineScalePS(1);
    c->Print((feat+"_Comparison.pdf").c_str());
    c->Print((feat+"_Comparison.png").c_str());

}

//str.erase(std::remove(str.begin(), str.end(), 'a'), str.end());


//root -l -b '../../fitModelRQ_v15.cc (" ", ...); exit(0)'
void FeatureDraw( const string& data_file,  const string& mc_file,  
                  const string& feature,    const string& constraint="" ){          
    DrawCanvas(data_file, mc_file, feature, constraint);
}


void DrawAllFeatures ( const string& data_file,  const string& mc_file){
    FeatureDraw(data_file, mc_file, "Muon_pt[0]",   "Muon_pt[0]<80");
    FeatureDraw(data_file, mc_file, "Muon_pt[1]",   "Muon_pt[1]<60");
    FeatureDraw(data_file, mc_file, "Photon_pt[0]", "Photon_pt[0]<80");
    FeatureDraw(data_file, mc_file, "Jpsi_pt[0]",   "Jpsi_pt[0]<100");
    FeatureDraw(data_file, mc_file, "Muon_eta[0]",  "");
    FeatureDraw(data_file, mc_file, "Muon_eta[1]",  "");
    FeatureDraw(data_file, mc_file, "Photon_eta[0]","");
    FeatureDraw(data_file, mc_file, "Jpsi_eta[0]",  "");
    FeatureDraw(data_file, mc_file, "MuMu_invMass", "");

}





/*
Events
Events_TriggerIsoMu27
Events_TriggerMu17Mu8Iso
Events_TriggerOR
Events_TriggerAND


*/