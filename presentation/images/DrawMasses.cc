#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooExtendPdf.h"
#include "RooFitResult.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "RooNLLVar.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include "style.cc"

using namespace std;

void DrawMasses(const string& path="./"){
    
    //setTDRStyle();

    TFile *f=new TFile((path+"Final_DataIsoMu.root").c_str(), "READ");
    TTree* EvTree = (TTree*)f->Get("Events_IsoMu");

    
    TCanvas* c1 = new TCanvas( "c1","c1", 800, 600 );
   
    TH1F *h = new TH1F("h", "h", 160, 70, 150);
    EvTree->Draw("Reco_mass>>h", "", "goff");
    h->GetXaxis()->SetTitle("Reconstructed mass (Gev)");
    h->GetYaxis()->SetTitle("Events/(0.5 GeV)");
    h->SetTitle("Background distribution, HLT_IsoMu27 Selection");
    
    h->Draw();

   // gStyle->SetOptStat(11);  
    //tdrStyle->SetOptStat(11);  

    c1->Print("BkgMassIsoMu.png");
    c1->Print("BkgMassIsoMu.pdf");


    f=new TFile((path+"Final_DataIsoTrk.root").c_str(), "READ");
    EvTree = (TTree*)f->Get("Events_IsoTrk");

    h = new TH1F("h", "h", 160, 70, 150);
    EvTree->Draw("Reco_mass>>h", "", "goff");
    c1 = new TCanvas( "c1","c1", 800, 600 );
    h->GetXaxis()->SetTitle("Reconstructed mass (Gev)");
    h->GetYaxis()->SetTitle("Events/(0.5 GeV)");
    h->SetTitle("Background distribution, HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ Selection");
    
    h->Draw();    c1->Print("BkgMassIsoTrk.png");
    c1->Print("BkgMassIsoTrk.pdf");


    f=new TFile((path+"Final_SignalIsoMu.root").c_str(), "READ");
    EvTree = (TTree*)f->Get("Events_IsoMu");

    h = new TH1F("h", "h", 100, 115, 135);
    EvTree->Draw("Reco_mass>>h", "", "goff");
    c1 = new TCanvas( "c1","c1", 800, 600 );
    h->GetXaxis()->SetTitle("Reconstructed mass (Gev)");
    h->GetYaxis()->SetTitle("Events/(0.2 GeV)");
    h->SetTitle("Signal distribution, HLT_IsoMu27 Selection");
    
    h->Draw();    
    c1->Print("SigMassIsoMu.png");
    c1->Print("SigMassIsoMu.pdf");



    f=new TFile((path+"Final_SignalIsoTrk.root").c_str(), "READ");
    EvTree = (TTree*)f->Get("Events_IsoTrk");

    h = new TH1F("h", "h", 100, 115, 135);
    EvTree->Draw("Reco_mass>>h", "", "goff");
    c1 = new TCanvas( "c1","c1", 800, 600 );
    h->GetXaxis()->SetTitle("Reconstructed mass (Gev)");
    h->GetYaxis()->SetTitle("Events/(0.2 GeV)");
    h->SetTitle("Signal distribution, HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ Selection");
    
    h->Draw();    
    c1->Print("SigMassIsoTrk.png");
    c1->Print("SigMassIsoTrk.pdf");




}