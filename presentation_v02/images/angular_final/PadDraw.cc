#include "TCanvas.h"
#include "TAxis.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;


void PadDraw( const string& data_file,  const string& mc_file, const string& tree, 
              const string& feature,    const string& constraint="", const string& outname="",
              const string& xlab="",  const string& ylab="", const string& title="" ){
    
    TFile* dafile = new TFile(data_file.c_str());
    TFile* mcfile = new TFile(  mc_file.c_str());

    TTree* da_tree = (TTree*)dafile->Get(tree.c_str());
    TTree* mc_tree = (TTree*)mcfile->Get(tree.c_str());

    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    da_tree->Draw(feature.c_str(), constraint.c_str(), "HIST norm");
    TH1F *da_h =(TH1F*)da_tree->GetHistogram(); 
    mc_tree->Draw(feature.c_str(), constraint.c_str(), "HIST same norm");
    TH1F *mc_h =(TH1F*)mc_tree->GetHistogram();

    mc_h->SetLineColor(kGreen); da_h->SetLineColor(kRed);
    mc_h->SetLineWidth(3);      da_h->SetLineWidth(3);
   // mc_h->Scale(double(da_tree->GetEntries())/mc_tree->GetEntries());
   
    //da_h->DrawNormalized();
    //mc_h->DrawNormalized("same");

    if (da_h->GetMaximum() < mc_h->GetMaximum())
    	da_h->GetYaxis()->SetRangeUser(0, mc_h->GetMaximum()*1.1);

    auto legend = new TLegend(0.4,0.8,0.5,0.9);
    legend->AddEntry(da_h, "data", "l");
    legend->AddEntry(mc_h, "MC",   "l");
    legend->Draw();

    TH1F *hist = (TH1F*)gPad->GetPrimitive("htemp");
    hist->SetTitle((title).c_str());
    if (xlab!="")     hist->GetXaxis()->SetTitle((xlab).c_str());
    if (ylab!="")     hist->GetYaxis()->SetTitle((ylab).c_str());
    gStyle->SetTitleYOffset(0.8);

    c1->Print((feature+outname+".png").c_str());
    c1->Print((feature+outname+".pdf").c_str());

    //da_h->Draw();
    //mc_h->Draw();
    
}