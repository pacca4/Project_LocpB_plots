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



//root -l -b '../../fitModelRQ_v15.cc (" ", ...); exit(0)'
void compareDrawNormalized( const string& data_file,  const string& mc_file,
                  bool save=false){

    TFile* dafile = new TFile(data_file.c_str());
    TFile* mcfile = new TFile(  mc_file.c_str());

    TTree* da_tree = (TTree*)dafile->Get("Events");
    TTree* mc_tree = (TTree*)mcfile->Get("Events");

    mc_tree->SetLineColor(kGreen); da_tree->SetLineColor(kRed);
    mc_tree->SetLineWidth(3);      da_tree->SetLineWidth(3);

    int da_nevs = da_tree->GetEntries();
    int mc_nevs = mc_tree->GetEntries();

    //set_TH1F
    TH1F *da_hist = new TH1F("da_hist","MUont_pt[0]", 80,20,60);
    TH1F *mc_hist = new TH1F("mc_hist","MUont_pt[0]", 80,20,60);

    //sewt_color

    mc_hist->SetLineColor(kGreen); da_hist->SetLineColor(kRed);
    mc_hist->SetLineWidth(3);      da_hist->SetLineWidth(3);

    TCanvas* c1 = new TCanvas( "c1","c1", 800, 600 );
    c1->cd();
    da_tree->Draw("Muon_pt[0]>>da_hist", "Muon_pt[0]<60", "goff");
    mc_tree->Draw("Muon_pt[0]>>mc_hist", "Muon_pt[0]<60", "goff");
    mc_hist->Scale(double(da_nevs)/mc_nevs);
    da_hist -> Draw();
    mc_hist -> Draw("HIST same");
    auto legend = new TLegend(0.5,0.8,0.6,0.9);
    legend->AddEntry(da_tree, "data", "l");
    legend->AddEntry(mc_tree, "MC",   "l");
    legend->Draw();
    if(save) c1->Print("MuonPt0.pdf");



    da_hist = new TH1F("da_hist","Photon_pt[0]", 100,30,80);
    mc_hist = new TH1F("mc_hist","Photon_pt[0]", 100,30,80);
    mc_hist->SetLineColor(kGreen); da_hist->SetLineColor(kRed);
    mc_hist->SetLineWidth(3);      da_hist->SetLineWidth(3);

    TCanvas* c2 = new TCanvas( "c2","c2", 800, 600 );
    c2->cd();
    da_tree->Draw("Photon_pt[0]>>da_hist", "Photon_pt[0]<80", "goff");
    mc_tree->Draw("Photon_pt[0]>>mc_hist", "Photon_pt[0]<80", "goff");
    mc_hist->Scale(double(da_nevs)/mc_nevs);
    da_hist -> Draw();
    mc_hist -> Draw("HIST same");
    legend->Draw();
    if(save) c2->Print("photonPt.pdf");




    da_hist = new TH1F("da_hist","Muon_pt[1]", 100,0,60);
    mc_hist = new TH1F("mc_hist","Muont_pt[1]", 100,0,60);
    mc_hist->SetLineColor(kGreen); da_hist->SetLineColor(kRed);
    mc_hist->SetLineWidth(3);      da_hist->SetLineWidth(3);

    TCanvas* c3 = new TCanvas( "c3","c3", 800, 600 );
    c3->cd();
    da_tree->Draw("Muon_pt[1]>>da_hist", "Muon_pt[1]<60", "goff");
    mc_tree->Draw("Muon_pt[1]>>mc_hist", "Muon_pt[1]<60", "goff");
    mc_hist->Scale(double(da_nevs)/mc_nevs);
    da_hist -> Draw();
    mc_hist -> Draw("HIST same");
    legend->Draw();
    if(save) c3->Print("MuonPt1.pdf");



    
    TCanvas* c4 = new TCanvas( "c4","c4", 800, 600 );
    c4->cd();
    TH1F* da_hJ= DrawAngleJPsiG(da_tree, kRed,   3 , "da_Angle"); da_hJ->Draw();
    TH1F* mc_hJ= DrawAngleJPsiG(mc_tree, kGreen, 3 , "mc_Angle"); mc_hJ->Draw("same");
    mc_hJ->Scale(double(da_nevs)/mc_nevs);
    da_hJ ->Draw();
    mc_hJ ->Draw("HIST same");
    legend->Draw();
    //mc_h->SetLineColor(kGreen); da_h->SetLineColor(kRed);
    //mc_h->SetLineWidth(3);      da_h->SetLineWidth(3);
    if(save) c4->Print("JpsiGAngle.pdf");

    TCanvas* c5 = new TCanvas( "c5","c5", 800, 600 );
    c5->cd();
    TH1F* da_hm= DrawAngleMuMu(da_tree, kRed,   3 , "da_Angle"); da_hm->Draw();
    TH1F* mc_hm= DrawAngleMuMu(mc_tree, kGreen, 3 , "mc_Angle"); mc_hm->Draw("same");
    mc_hm->Scale(double(da_nevs)/mc_nevs);
    da_hm ->Draw();
    mc_hm ->Draw("HIST same");
    legend->Draw();
    //mc_h->SetLineColor(kGreen); da_h->SetLineColor(kRed);
    //mc_h->SetLineWidth(3);      da_h->SetLineWidth(3);
    if(save) c5->Print("MuMuAngle.pdf");




    da_hist = new TH1F("da_hist","Muon_eta[0]", 100,-3,3);
    mc_hist = new TH1F("mc_hist","Muon_eta[0]", 100,-3,3);
    mc_hist->SetLineColor(kGreen); da_hist->SetLineColor(kRed);
    mc_hist->SetLineWidth(3);      da_hist->SetLineWidth(3);

    TCanvas* c6 = new TCanvas( "c6","c6", 800, 600 );
    c6->cd();
    da_tree->Draw("Muon_eta[0]>>da_hist", "", "goff" );
    mc_tree->Draw("Muon_eta[0]>>mc_hist","", "goff" );
    mc_hist->Scale(double(da_nevs)/mc_nevs);
    da_hist -> Draw();
    mc_hist -> Draw("HIST same");
    legend->Draw();
    if(save) c6->Print("MuonEta0.pdf");


    da_hist = new TH1F("da_hist","Muon_eta[1]", 100,-3,3);
    mc_hist = new TH1F("mc_hist","Muon_eta[1]", 100,-3,3);
    mc_hist->SetLineColor(kGreen); da_hist->SetLineColor(kRed);
    mc_hist->SetLineWidth(3);      da_hist->SetLineWidth(3);

    TCanvas* c7 = new TCanvas( "c7","c7", 800, 600 );
    c7->cd();
    da_tree->Draw("Muon_eta[1]>>da_hist", "", "goff");
    mc_tree->Draw("Muon_eta[1]>>mc_hist", "", "goff");
    mc_hist->Scale(double(da_nevs)/mc_nevs);
    da_hist -> Draw();
    mc_hist -> Draw("HIST same");
    legend->Draw();
    if(save) c7->Print("MuonEta1.pdf");




    da_hist = new TH1F("da_hist","Photon_eta[0]", 100,-3,3);
    mc_hist = new TH1F("mc_hist","Photon_eta[0]", 100,-3,3);
    mc_hist->SetLineColor(kGreen); da_hist->SetLineColor(kRed);
    mc_hist->SetLineWidth(3);      da_hist->SetLineWidth(3);

    TCanvas* c8 = new TCanvas( "c8","c8", 800, 600 );
    c8->cd();
    da_tree->Draw("Photon_eta[0]>>da_hist", "","goff");
    mc_tree->Draw("Photon_eta[0]>>mc_hist", "", "goff" );
    mc_hist->Scale(double(da_nevs)/mc_nevs);
    da_hist -> Draw();
    mc_hist -> Draw("HIST same");
    legend->Draw();
    if(save) c8->Print("PhotonEta.pdf");

}