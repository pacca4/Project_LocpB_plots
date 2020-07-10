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
#include "RooBukinPdf.h"

using namespace RooFit ;



EColor csig = kRed;
EColor csi2 = kViolet;
EColor cbkg = kGreen;
EColor cmis = kOrange;


void DataFitIsoMu ( const string& file, int nbin=80 ){

    //define the Reconstructed mass variable
    RooRealVar Reco_mass( "Reco_mass", "Reconstructed Mass", 70, 150, "GeV" );
    Reco_mass.setBins( nbin );

    //open file and get events from tree
    TFile *f=new TFile(file.c_str(), "READ");
    TTree* EvTree = (TTree*)f->Get("Events_IsoMu");
    //unbinned dataset
    RooDataSet data_unb ("data", "data", EvTree, Reco_mass);
    //binned data set
    TH1* h = data_unb.createHistogram("h", Reco_mass, nbin);
    RooDataHist data("Reconstructed mass", "Reconstructed mass", Reco_mass, h);

    //cut data around signal region and define mass ranges (for the fit)
    TString Cut = "Reco_mass<120 ||Reco_mass>130";
    RooAbsData *CutData = data.reduce( Cut );
    Reco_mass.setRange("lowMass" ,  70, 120);
    Reco_mass.setRange("highMass", 130, 150);
    Reco_mass.setRange("signal",   120, 130);

    //define model for fit: 2nd degree polynomial
    RooRealVar Bkg_IsoMu_coef1( "Bkg_IsoMu_coef1", "Bkg_IsoMu_coef1"  , -0.0104791, -0.1, 0.1, "GeV^-1" );
    RooRealVar Bkg_IsoMu_coef2( "Bkg_IsoMu_coef2", "Bkg_IsoMu_coef2"  , 2.92647e-05,  -0.001,  0.01, "GeV^-2" ); 
    RooPolynomial Bkg_IsoMu_polyn ( "Bkg_IsoMu_polyn", "Bkg_IsoMu_polyn",
                 Reco_mass, RooArgList( Bkg_IsoMu_coef1, Bkg_IsoMu_coef2 ) );
    //fit the model on the sidebands
    Bkg_IsoMu_polyn.fitTo( *CutData , Range("lowMass,highMass") );
    
    TCanvas* c1 = new TCanvas( "Reconstructed mass", "Reconstructed mass", 800, 600 );
    c1->SetLeftMargin( 0.15 ); c1->cd();
    RooPlot* h_frame = Reco_mass.frame();
    CutData->plotOn( h_frame );
    Bkg_IsoMu_polyn.plotOn( h_frame , LineColor( kRed ));
    h_frame->Draw();

    c1->Print("RecoMassSidebands_IsoMu.pdf");
}


void DataFitIsoTrk ( const string& file, int nbin=80 ){

    //define the Reconstructed mass variable
    RooRealVar Reco_mass( "Reco_mass", "Reconstructed Mass", 70, 150, "GeV" );
    Reco_mass.setBins( nbin );

    //open file and get events from tree
    TFile *f=new TFile(file.c_str(), "READ");
    TTree* EvTree = (TTree*)f->Get("Events_IsoTrk");
    //unbinned dataset
    RooDataSet data_unb ("data", "data", EvTree, Reco_mass);
    //binned data set
    TH1* h = data_unb.createHistogram("h", Reco_mass, nbin);
    RooDataHist data("Reconstructed mass", "Reconstructed mass", Reco_mass, h);

    //cut data around signal region and define mass ranges (for the fit)
    TString Cut = "Reco_mass<120 ||Reco_mass>130";
    RooAbsData *CutData = data.reduce( Cut );
    Reco_mass.setRange("lowMass" ,  70, 120);
    Reco_mass.setRange("highMass", 130, 150);
    Reco_mass.setRange("signal",   120, 130);

    //define model for fit: exponential (simple not convoluted :( )
    RooRealVar     Bkg_IsoTrk_tau ( "Bkg_IsoTrk_tau", "Bkg_IsoTrk_tau" , -0.0302445, -1, 0., "GeV");
    RooExponential Bkg_IsoTrk_exp ( "Bkg_IsoTrk_exp", "Bkg_IsoTrk_exp" ,   Reco_mass, Bkg_IsoTrk_tau);//, ResoGauss, RooDecay::SingleSided );
 
 //fit the model on the sidebands
    Bkg_IsoTrk_exp.fitTo( *CutData , Range("lowMass,highMass") );
    
    TCanvas* c2 = new TCanvas( "Reconstructed mass", "Reconstructed mass", 800, 600 );
    c2->SetLeftMargin( 0.15 ); c2->cd();
    RooPlot* h_frame = Reco_mass.frame();
    CutData->plotOn( h_frame );
    Bkg_IsoTrk_exp.plotOn( h_frame , LineColor( kRed ));
    h_frame->Draw();

    c2->Print("RecoMassSidebands_IsoTrk.pdf");
}

// praticament e ora faacio copia incolla delle varie macro e metto tutto assieme
// mettendo dei nomi decenti

void SigFitIsoMu  ( const string& file, int nbin=80){
    //define the Reconstructed mass variable
    RooRealVar Reco_mass( "Reco_mass", "Reconstructed Mass", 70, 150, "GeV" );
    Reco_mass.setBins( nbin );

    //open file and get events from tree
    TFile *f=new TFile(file.c_str(), "READ");
    TTree* EvTree = (TTree*)f->Get("Events_IsoMu");
    //unbinned dataset
    RooDataSet data_unb ("data", "data", EvTree, Reco_mass);
    //binned data set
    TH1* h = data_unb.createHistogram("h", Reco_mass, nbin);
    RooDataHist data("Reconstructed mass", "Reconstructed mass", Reco_mass, h);

    //model for signal: Cristall ball with a 3variate gaussian
    //diatribution mean and gaussian variances
    RooRealVar  Sig_IsoMu_mean   ( "Sig_IsoMu_mean", "Sig_IsoMu_mean",   124.793,   120,  130, "GeV" );
    RooRealVar  Sig_IsoMu_sig1   ( "Sig_IsoMu_sig1", "Sig_IsoMu_sig1",  0.733419,   0.01,  10, "GeV" );
    RooRealVar  Sig_IsoMu_sig2   ( "Sig_IsoMu_sig2", "Sig_IsoMu_sig2",   2.48086,   0.01,  10, "GeV" ); 
    RooRealVar  Sig_IsoMu_sig3   ( "Sig_IsoMu_sig3", "Sig_IsoMu_sig3",   11.2167,      1, 100, "GeV" ); 
    //fraction of pdf for the gaussian sum
    RooRealVar  Sig_IsoMu_gfrac1 ( "Sig_IsoMu_gfrac1", "Sig_IsoMu_gfrac1", 0.263134,  0.0, 1.0 );
    RooRealVar  Sig_IsoMu_gfrac2 ( "Sig_IsoMu_gfrac2", "Sig_IsoMu_gfrac2", 0.698942,  0.0, 1.0 );

    //gaussian definition ad sum
    RooGaussian Sig_IsoMu_gauss1 ( "Sig_IsoMu_gauss1", "Sig_IsoMu_gauss1", Reco_mass, Sig_IsoMu_mean, Sig_IsoMu_sig1 );
    RooGaussian Sig_IsoMu_gauss2 ( "Sig_IsoMu_gauss2", "Sig_IsoMu_gauss2", Reco_mass, Sig_IsoMu_mean, Sig_IsoMu_sig2 );
    RooGaussian Sig_IsoMu_gauss3 ( "Sig_IsoMu_gauss3", "Sig_IsoMu_gauss3", Reco_mass, Sig_IsoMu_mean, Sig_IsoMu_sig3 );
    RooAddPdf Sig_IsoMu_GaussSum ( "Sig_IsoMu_GaussSum", "Sig_IsoMu_GaussSum"  , 
                                    RooArgList( Sig_IsoMu_gauss1, Sig_IsoMu_gauss2, Sig_IsoMu_gauss3), 
                                    RooArgList( Sig_IsoMu_gfrac1, Sig_IsoMu_gfrac2 ));
    //cristall ball paramenters and definition
    RooRealVar Sig_IsoMu_CBSig   ("Sig_IsoMu_CBSig"  , "Sig_IsoMu_CBSig"  , 1.41684, 0.1, 10, "Gev");
    RooRealVar Sig_IsoMu_CBn     ("Sig_IsoMu_CBn"    , "Sig_IsoMu_CBn"    , 1.79598, 0, 10, "Gev");
    RooRealVar Sig_IsoMu_CBalpha ("Sig_IsoMu_CBalpha", "Sig_IsoMu_CBalpha", 1.66116, 0, 3, "Gev");
    RooCBShape Sig_IsoMu_CB ("Sig_IsoMu_CB", "Sig_IsoMu_CB", Reco_mass ,
                             Sig_IsoMu_mean, Sig_IsoMu_CBSig  , 
                             Sig_IsoMu_CBn , Sig_IsoMu_CBalpha); 
    //gaussian and cb sum for the final model
    RooRealVar Sig_IsoMu_Frac( "Sig_IsoMu_Frac", "Sig_IsoMu_Frac" , 0.31048, 0.0, 1.0, "." );
    RooAddPdf  Sig_IsoMu_Model  ( "Sig_IsoMu_Model", "Sig_IsoMu_Model ", 
                                RooArgList(Sig_IsoMu_GaussSum, Sig_IsoMu_CB), Sig_IsoMu_Frac );

    Sig_IsoMu_Model.fitTo( data ); 
    TCanvas* c1 = new TCanvas( "Reconstructed mass", "Reconstructed mass", 800, 600 );
    c1->SetLeftMargin( 0.15 ); c1->cd();
    RooPlot* h_frame = Reco_mass.frame();
   
   // RooArgSet params = RooArgList(rmass, widt1, widt2, widt3, sigma) ;
   // model12.paramOn(h_frame,Layout(0.7, 0.95, 0.9), Format("NE", AutoPrecision()), Parameters(params));

    data.plotOn( h_frame );
    Sig_IsoMu_Model.plotOn( h_frame );
    Sig_IsoMu_Model.plotOn( h_frame, Components( "Sig_IsoMu_GaussSum"),LineStyle( kDashed ), LineColor( kOrange ) );
    Sig_IsoMu_Model.plotOn( h_frame, Components( "Sig_IsoMu_CB"),LineStyle( kDashed ), LineColor( kGreen ) );
    h_frame->Draw();
   
    c1->Print("RecoMassSignal_IsoMu.pdf");
}
void SigFitIsoTrk  ( const string& file, int nbin=80){
    //define the Reconstructed mass variable
    RooRealVar Reco_mass( "Reco_mass", "Reconstructed Mass", 70, 150, "GeV" );
    Reco_mass.setBins( nbin );

    //open file and get events from tree
    TFile *f=new TFile(file.c_str(), "READ");
    TTree* EvTree = (TTree*)f->Get("Events_IsoTrk");
    //unbinned dataset
    RooDataSet data_unb ("data", "data", EvTree, Reco_mass);
    //binned data set
    TH1* h = data_unb.createHistogram("h", Reco_mass, nbin);
    RooDataHist data("Reconstructed mass", "Reconstructed mass", Reco_mass, h);

    //model for signal: Cristall ball with a 3variate gaussian
    //diatribution mean and gaussian variances
    RooRealVar  Sig_IsoTrk_mean   ( "Sig_IsoTrk_mean", "Sig_IsoTrk_mean",   124.793,   120,  130, "GeV" );
    RooRealVar  Sig_IsoTrk_sig1   ( "Sig_IsoTrk_sig1", "Sig_IsoTrk_sig1",  0.733419,   0.01,  10, "GeV" );
    RooRealVar  Sig_IsoTrk_sig2   ( "Sig_IsoTrk_sig2", "Sig_IsoTrk_sig2",   2.48086,   0.01,  10, "GeV" ); 
    RooRealVar  Sig_IsoTrk_sig3   ( "Sig_IsoTrk_sig3", "Sig_IsoTrk_sig3",   11.2167,      1, 100, "GeV" ); 
    //fraction of pdf for the gaussian sum
    RooRealVar  Sig_IsoTrk_gfrac1 ( "Sig_IsoTrk_gfrac1", "Sig_IsoTrk_gfrac1", 0.263134,  0.0, 1.0 );
    RooRealVar  Sig_IsoTrk_gfrac2 ( "Sig_IsoTrk_gfrac2", "Sig_IsoTrk_gfrac2", 0.698942,  0.0, 1.0 );

    //gaussian definition ad sum
    RooGaussian Sig_IsoTrk_gauss1 ( "Sig_IsoTrk_gauss1", "Sig_IsoTrk_gauss1", Reco_mass, Sig_IsoTrk_mean, Sig_IsoTrk_sig1 );
    RooGaussian Sig_IsoTrk_gauss2 ( "Sig_IsoTrk_gauss2", "Sig_IsoTrk_gauss2", Reco_mass, Sig_IsoTrk_mean, Sig_IsoTrk_sig2 );
    RooGaussian Sig_IsoTrk_gauss3 ( "Sig_IsoTrk_gauss3", "Sig_IsoTrk_gauss3", Reco_mass, Sig_IsoTrk_mean, Sig_IsoTrk_sig3 );
    RooAddPdf Sig_IsoTrk_GaussSum ( "Sig_IsoTrk_GaussSum", "Sig_IsoTrk_GaussSum"  , 
                                    RooArgList( Sig_IsoTrk_gauss1, Sig_IsoTrk_gauss2, Sig_IsoTrk_gauss3), 
                                    RooArgList( Sig_IsoTrk_gfrac1, Sig_IsoTrk_gfrac2 ));
    //cristall ball paramenters and definition
    RooRealVar Sig_IsoTrk_CBSig   ("Sig_IsoTrk_CBSig"  , "Sig_IsoTrk_CBSig"  , 1.41684, 0.1, 10, "Gev");
    RooRealVar Sig_IsoTrk_CBn     ("Sig_IsoTrk_CBn"    , "Sig_IsoTrk_CBn"    , 1.79598, 0, 10, "Gev");
    RooRealVar Sig_IsoTrk_CBalpha ("Sig_IsoTrk_CBalpha", "Sig_IsoTrk_CBalpha", 1.66116, 0, 3, "Gev");
    RooCBShape Sig_IsoTrk_CB ("Sig_IsoTrk_CB", "Sig_IsoTrk_CB", Reco_mass ,
                             Sig_IsoTrk_mean, Sig_IsoTrk_CBSig  , 
                             Sig_IsoTrk_CBn , Sig_IsoTrk_CBalpha); 
    //gaussian and cb sum for the final model
    RooRealVar Sig_IsoTrk_Frac( "Sig_IsoTrk_Frac", "Sig_IsoTrk_Frac" , 0.31048, 0.0, 1.0, "." );
    RooAddPdf  Sig_IsoTrk_Model  ( "Sig_IsoTrk_Model", "Sig_IsoTrk_Model ", 
                                RooArgList(Sig_IsoTrk_GaussSum, Sig_IsoTrk_CB), Sig_IsoTrk_Frac );

    Sig_IsoTrk_Model.fitTo( data ); 
    TCanvas* c1 = new TCanvas( "Reconstructed mass", "Reconstructed mass", 800, 600 );
    c1->SetLeftMargin( 0.15 ); c1->cd();    
    RooPlot* h_frame = Reco_mass.frame();
   
   // RooArgSet params = RooArgList(rmass, widt1, widt2, widt3, sigma) ;
   // model12.paramOn(h_frame,Layout(0.7, 0.95, 0.9), Format("NE", AutoPrecision()), Parameters(params));

    data.plotOn( h_frame );
    Sig_IsoTrk_Model.plotOn( h_frame );
    Sig_IsoTrk_Model.plotOn( h_frame, Components( "Sig_IsoTrk_GaussSum"),LineStyle( kDashed ), LineColor( kOrange ) );
    Sig_IsoTrk_Model.plotOn( h_frame, Components( "Sig_IsoTrk_CB"),LineStyle( kDashed ), LineColor( kGreen ) );
    h_frame->Draw();
   
    c1->Print("RecoMassSignal_IsoTrk.pdf");
    
}

void FitAll(int nbin=80){
    DataFitIsoMu ("Final_DataIsoMu.root"   , nbin);
    DataFitIsoTrk("Final_DataIsoTrk.root"  , nbin);
    SigFitIsoMu  ("Final_SignalIsoMu.root" , nbin);
    SigFitIsoTrk ("Final_SignalIsoTrk.root", nbin);
}

