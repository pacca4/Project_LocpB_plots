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




void SigFitIsoMu (const string& file, double xmin=115, double xmax=135 , int nbin=160,
                double mass=126, const string& outname="H_2muG" , 
                double sigma1=1, double sigma2=4, double sigma3=20,
                double signalFrac=0.5 , double bkg_frac=0.5){

    RooRealVar Reco_mass( "Reco_mass", "Reconstructed Mass",    xmin, xmax, "Gev/c^{2}" );
    TFile *f=new TFile(file.c_str(), "READ");
    TTree* EvTree = (TTree*)f->Get("Events_IsoMu");

    RooDataSet data_unb ("data", "data", EvTree, Reco_mass);
    TH1* h = data_unb.createHistogram("h", Reco_mass, nbin);
    RooDataHist data("ToyMC_data", "ToyMC Binned data", Reco_mass, h);


    RooRealVar  IsoMu_mean   ( "IsoMu_mean", "IsoMu_mean",   124.793,   120,  130, "Gev/c^{2}" );
    RooRealVar  IsoMu_sig1   ( "IsoMu_sig1", "IsoMu_sig1",  0.733419,   0.01,  10, "Gev/c^{2}" );
    RooRealVar  IsoMu_sig2   ( "IsoMu_sig2", "IsoMu_sig2",   2.48086,   0.01,  10, "Gev/c^{2}" );
    RooRealVar  IsoMu_sig3   ( "IsoMu_sig3", "IsoMu_sig3",   11.2167,      1, 100, "Gev/c^{2}" );
    
    RooRealVar  IsoMu_gfrac1 ( "IsoMu_gfrac1", "IsoMu_gfrac1", 0.263134,  0.0, 1.0 );
    RooRealVar  IsoMu_gfrac2 ( "IsoMu_gfrac2", "IsoMu_gfrac2", 0.698942,  0.0, 1.0 );
    
    RooGaussian IsoMu_gauss1 ( "IsoMu_gauss1", "IsoMu_gauss1", Reco_mass, IsoMu_mean, IsoMu_sig1 );
    RooGaussian IsoMu_gauss2 ( "IsoMu_gauss2", "IsoMu_gauss2", Reco_mass, IsoMu_mean, IsoMu_sig2 );
    RooGaussian IsoMu_gauss3 ( "IsoMu_gauss3", "IsoMu_gauss3", Reco_mass, IsoMu_mean, IsoMu_sig3 );
    
    RooAddPdf IsoMu_GaussSum ( "IsoMu_GaussSum", "IsoMu_GaussSum"  ,
                                    RooArgList( IsoMu_gauss1, IsoMu_gauss2, IsoMu_gauss3),
                                    RooArgList( IsoMu_gfrac1, IsoMu_gfrac2 ));
    
    RooRealVar IsoMu_CBSig   ("IsoMu_CBSig"  , "IsoMu_CBSig"  , 1.41684, 0.1, 10, "Gev/c^{2}");
    RooRealVar IsoMu_CBn     ("IsoMu_CBn"    , "IsoMu_CBn"    , 1.79598, 0, 10, "Gev/c^{2}");
    RooRealVar IsoMu_CBalpha ("IsoMu_CBalpha", "IsoMu_CBalpha", 1.66116, 0, 3, "Gev/c^{2}");
    RooCBShape IsoMu_CB ("IsoMu_CB", "IsoMu_CB", Reco_mass ,
                             IsoMu_mean, IsoMu_CBSig  ,
                             IsoMu_CBn , IsoMu_CBalpha); 
    
    RooRealVar IsoMu_Frac( "IsoMu_Frac", "IsoMu_Frac" , 0.31048, 0.0, 1.0, "." );
    
    RooAddPdf  IsoMu_Model  ( "IsoMu_Model", "IsoMu_Model ",
                                RooArgList(IsoMu_GaussSum, IsoMu_CB), IsoMu_Frac );

    
    IsoMu_Model.fitTo( data ); 
   // CBBkg.fitTo(data);
   // CBBkg.fitTo(data);
    

 
    TCanvas* c1 = new TCanvas( "c1",outname.c_str(), 800, 600 );
    c1->SetLeftMargin( 0.15 );
    c1->cd();
    Reco_mass.setBins( nbin );
    RooPlot* h_frame = Reco_mass.frame();
    h_frame->SetTitle("Signal Distribution Fit :: IsoMu");
    //RooArgSet params = RooArgList(rmass, widt1, widt2, widt3, sigma) ;
    //IsoMu_Model.paramOn(h_frame,Layout(0.7, 0.95, 0.9), Format("NE", AutoPrecision()), Parameters(params));

    data.plotOn( h_frame );
    IsoMu_Model.plotOn( h_frame);
    IsoMu_Model.plotOn( h_frame, Components( "IsoMu_GaussSum"),LineStyle( kDashed ), LineColor( kGreen ) );
    IsoMu_Model.plotOn( h_frame, Components("IsoMu_CB"),LineStyle( kDashed ), LineColor( kRed ) );
    h_frame->Draw();
   
     
    TString msigns[] = {
        "data",
        "Model",
        "CB",
        "Gauss"    
    };

    TLegend* mleg = new TLegend(0.7, 0.7,0.9,0.9);

    for (int i=0; i<h_frame->numItems(); i++) {
        TString obj_name=h_frame->nameOf(i); if (obj_name=="") continue;
        TObject *obj = h_frame->findObject(obj_name.Data());
        if(msigns[i]=="data") mleg->AddEntry(obj,msigns[i],"lp");
        else mleg->AddEntry(obj,msigns[i],"l");
    }
    mleg->Draw("same");

    c1->Print("SignalFitIsoMu.png");
    c1->Print("SignalFitIsoMu.pdf");


}