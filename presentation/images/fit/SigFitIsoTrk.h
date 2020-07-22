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


void SigFitIsoTrk (const string& file, double xmin=115, double xmax=135 , int nbin=160,
                double mass=126, const string& outname="H_2muG" , 
                double sigma1=1, double sigma2=4, double sigma3=20,
                double signalFrac=0.5 , double bkg_frac=0.5){

    RooRealVar Reco_mass( "Reco_mass", "Reconstructed Mass",    xmin, xmax, "GeV" );
    TFile *f=new TFile(file.c_str(), "READ");
    TTree* EvTree = (TTree*)f->Get("Events_IsoTrk");

    RooDataSet data_unb ("data", "data", EvTree, Reco_mass);
    TH1* h = data_unb.createHistogram("h", Reco_mass, nbin);
    RooDataHist data("ToyMC_data", "ToyMC Binned data", Reco_mass, h);


    RooRealVar  IsoTrk_mean   ( "IsoTrk_mean", "IsoTrk_mean",   124.793,   120,  130, "GeV" );
    RooRealVar  IsoTrk_sig1   ( "IsoTrk_sig1", "IsoTrk_sig1",  0.733419,   0.01,  10, "GeV" );
    RooRealVar  IsoTrk_sig2   ( "IsoTrk_sig2", "IsoTrk_sig2",   2.48086,   0.01,  10, "GeV" );
    RooRealVar  IsoTrk_sig3   ( "IsoTrk_sig3", "IsoTrk_sig3",   11.2167,      1, 100, "GeV" );
    
    RooRealVar  IsoTrk_gfrac1 ( "IsoTrk_gfrac1", "IsoTrk_gfrac1", 0.263134,  0.0, 1.0 );
    RooRealVar  IsoTrk_gfrac2 ( "IsoTrk_gfrac2", "IsoTrk_gfrac2", 0.698942,  0.0, 1.0 );
    
    RooGaussian IsoTrk_gauss1 ( "IsoTrk_gauss1", "IsoTrk_gauss1", Reco_mass, IsoTrk_mean, IsoTrk_sig1 );
    RooGaussian IsoTrk_gauss2 ( "IsoTrk_gauss2", "IsoTrk_gauss2", Reco_mass, IsoTrk_mean, IsoTrk_sig2 );
    RooGaussian IsoTrk_gauss3 ( "IsoTrk_gauss3", "IsoTrk_gauss3", Reco_mass, IsoTrk_mean, IsoTrk_sig3 );
    
    RooAddPdf IsoTrk_GaussSum ( "IsoTrk_GaussSum", "IsoTrk_GaussSum"  ,
                                    RooArgList( IsoTrk_gauss1, IsoTrk_gauss2, IsoTrk_gauss3),
                                    RooArgList( IsoTrk_gfrac1, IsoTrk_gfrac2 ));
    
    RooRealVar IsoTrk_CBSig   ("IsoTrk_CBSig"  , "IsoTrk_CBSig"  , 1.41684, 0.1, 10, "Gev");
    RooRealVar IsoTrk_CBn     ("IsoTrk_CBn"    , "IsoTrk_CBn"    , 1.79598, 0, 10, "Gev");
    RooRealVar IsoTrk_CBalpha ("IsoTrk_CBalpha", "IsoTrk_CBalpha", 1.66116, 0, 3, "Gev");
    RooCBShape IsoTrk_CB ("IsoTrk_CB", "IsoTrk_CB", Reco_mass ,
                             IsoTrk_mean, IsoTrk_CBSig  ,
                             IsoTrk_CBn , IsoTrk_CBalpha); 
    
    RooRealVar IsoTrk_Frac( "IsoTrk_Frac", "IsoTrk_Frac" , 0.31048, 0.0, 1.0, "." );
    
    RooAddPdf  IsoTrk_Model  ( "IsoTrk_Model", "IsoTrk_Model ",
                                RooArgList(IsoTrk_GaussSum, IsoTrk_CB), IsoTrk_Frac );

    
    IsoTrk_Model.fitTo( data ); 
   // CBBkg.fitTo(data);
   // CBBkg.fitTo(data);
    

 
    TCanvas* c1 = new TCanvas( "c1",outname.c_str(), 800, 600 );
    c1->SetLeftMargin( 0.15 );
    c1->cd();
    Reco_mass.setBins( nbin );
    RooPlot* h_frame = Reco_mass.frame();
    h_frame->SetTitle("Signal Fit, HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ Selection");
    //RooArgSet params = RooArgList(rmass, widt1, widt2, widt3, sigma) ;
    //IsoTrk_Model.paramOn(h_frame,Layout(0.7, 0.95, 0.9), Format("NE", AutoPrecision()), Parameters(params));

    data.plotOn( h_frame );
    IsoTrk_Model.plotOn( h_frame );
    IsoTrk_Model.plotOn( h_frame, Components( "IsoTrk_GaussSum"),LineStyle( kDashed ), LineColor( kGreen ) );
    IsoTrk_Model.plotOn( h_frame, Components("IsoTrk_CB"),LineStyle( kDashed ), LineColor( kRed ) );
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

    c1->Print("SignalFitIsoTrk.png");
    c1->Print("SignalFitIsoTrk.pdf");


}