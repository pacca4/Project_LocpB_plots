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
//#include "style.cc"

using namespace RooFit ;


void BkgFitIsoTrk (const string& file,int nbin=160, double xmin=70, double xmax=150 , 
                double mass=126, const string& outname="H_2muG" , 
                double sigma1=1, double sigma2=4, double sigma3=20,
                double signalFrac=0.5 , double bkg_frac=0.5){

    RooRealVar Reco_mass( "Reco_mass", "Reconstructed Mass",    xmin, xmax, "Gev/c^{2}" );
    TFile *f=new TFile(file.c_str(), "READ");
    TTree* EvTree = (TTree*)f->Get("Events");

    RooDataSet data_unb ("data", "data", EvTree, Reco_mass);
    TH1* h = data_unb.createHistogram("h", Reco_mass, Binning(nbin, xmin, xmax));
    RooDataHist data("ToyMC_data", "ToyMC Binned data", Reco_mass, h);

    TString Cut = "Reco_mass<120 ||Reco_mass>130";
    RooAbsData *CutData = data.reduce( Cut );

    Reco_mass.setRange("lowMass" ,  xmin,  120);
    Reco_mass.setRange("highMass",   130, xmax);
    Reco_mass.setRange("signal",     120,  130);

    RooRealVar     IsoTrk_tau ( "IsoTrk_tau", "IsoTrk_tau" , -0.0302445, -1, 0., "Gev/c^{2}");
    RooExponential IsoTrk_exp ( "IsoTrk_exp", "IsoTrk_exp" ,   Reco_mass, IsoTrk_tau);//, ResoGauss, RooDecay::SingleSided );

    IsoTrk_exp.fitTo(*CutData , Range("lowMass,highMass") );


    TCanvas* c1 = new TCanvas( "c1",outname.c_str(), 800, 600 );
    c1->SetLeftMargin( 0.15 );
    c1->cd();
    Reco_mass.setBins( nbin );
    RooPlot* h_frame = Reco_mass.frame();
    h_frame->SetTitle("Background Distribution Fit, Exponential Model");

    CutData->plotOn( h_frame );
    IsoTrk_exp.plotOn( h_frame, LineColor(kRed) );
    h_frame->Draw();
   
     
    TString msigns[] = {
        "data",
        "Model"    
    };

    TLegend* mleg = new TLegend(0.75, 0.8,0.9,0.9);

    for (int i=0; i<2; i++) {
        TString obj_name=h_frame->nameOf(i); if (obj_name=="") continue;
        TObject *obj = h_frame->findObject(obj_name.Data());
        if(msigns[i]=="data") mleg->AddEntry(obj,msigns[i],"lp");
        else mleg->AddEntry(obj,msigns[i],"l");
    }
    mleg->Draw("same");

    c1->Print("BkgFitIsoTrk.png");
    c1->Print("BkgFitIsoTrk.pdf");

    cout<<"Exp slope: "<<IsoTrk_tau.getVal()<<endl;


}