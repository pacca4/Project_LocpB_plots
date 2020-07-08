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

using namespace RooFit ;


EColor csig = kRed;
EColor csi2 = kViolet;
EColor cbkg = kGreen;
EColor cmis = kOrange;


void bkg_fit (const string& file, double xmin=70, double xmax=150 , int nbin=80,
                double mass=95, const string& outname="bkg" ,
                double sigma1=10, bool append=false){//, 
                //, double sigma2=4, double sigma3=2,
                //double signalFrac=0.5 , double bkg_frac=0.5){

    //if(append) ofstream oo ((outname+"_outParams.txt").c_str(), std::ios_base::app);
    //else ofstream oo ((outname+"_outParams.txt").c_str());
    //ofstream oo ((outname+"_outParams.txt").c_str(), std::ios_base::app);
    //const char* list = file.c_str(); 

   ofstream oo ((outname+"_outParams.txt").c_str(), std::ios_base::app);
    RooRealVar Reco_mass( "Reco_mass", "Reconstructed Mass",    xmin, xmax, "GeV" );
    TFile *f=new TFile(file.c_str(), "READ");
    TTree* EvTree = (TTree*)f->Get("Events_IsoMu");
 // RooDataSet* data = RooDataSet::read( list, RooArgList( m) );
    RooDataSet data_unb ("data", "data", EvTree, Reco_mass);

    TH1* h = data_unb.createHistogram("h", Reco_mass, nbin);
    RooDataHist data("ToyMC_data", "ToyMC Binned data", Reco_mass, h);
 

    RooRealVar rmass (  "rmass",   "Invariant  Mass",   80,   xmin,  xmax, "GeV" );
    RooRealVar widt1( "widt1", "Mass Widt1", sigma1,  0.001, 100, "GeV" );
   // RooRealVar widt2( "widt2", "Mass Widt2", sigma2,  0.001, sigma2*10, "GeV" ); 
   // RooRealVar widt3( "widt3", "Mass Widt3", sigma2,  0.001, sigma3*10, "GeV" ); 

   RooRealVar gfrac1   ( "gfrac1", "signal fraction1", 0.5 , 0.0, 1.0 );
    //RooRealVar gfrac2   ( "gfrac2", "signal fraction2", signalFrac , 0.0, 1.0 );

   RooGaussian    gauss1( "gauss1",     "Signal Distribution1", Reco_mass, rmass, widt1 );
   // RooGaussian    gauss2( "gauss2",     "Signal Distribution2", m, rmass, widt2 );
   // RooGaussian    gauss3( "gauss3",     "Signal Distribution2", m, rmass, widt3 );

   // RooAddPdf GaussSum ( "GaussSum", "Signal"  , RooArgList( gauss1, gauss2, gauss3), 
    //                                             RooArgList( gfrac1, gfrac1 ));


    RooRealVar LBkgComb  ("LBkgComb", "combinatorial Bkg life, Left side"  , 20, 0.0, 100, "GeV");
    RooRealVar ResoMean  ( "ResoMean",  "Mass Res Mean"  ,   80,   xmin, xmax, "Gev" );
    RooRealVar Resolution( "Resolution","Mass Resolution", 40,  0.001, 100, "GeV" );
    //ResoMean.setConstant( kTRUE );
  //  RooRealVar BFrac( "BFrac", "Bkg fraction"       , bkg_frac, 0.0, 1.0, "." );
    //RooGaussian    gauss1( "gauss1",     "Signal Distribution1", Reco_mass, ResoMean, widt1 );
  
    RooGaussModel ResoGauss( "ResoGauss", "Mass Resolution Gaussian", Reco_mass, ResoMean, Resolution );
    RooDecay LBkgExpo( "LBkgExpo", "Left Bk Convolved Decay",   Reco_mass, LBkgComb, ResoGauss, RooDecay::SingleSided );
    //RooAddPdf GGexp ("GGexp", )
/*

    int          TotEvents = data->numEntries();
    float        SigEvents = TotEvents * signalFrac;
    float        BkgEvents = TotEvents * ( 1.0 - signalFrac );

    RooRealVar   SigNum ( "SigNum", "Signal number", 0, TotEvents );
    RooRealVar   BkgNum ( "BkgNum", "Backgr number", 0, TotEvents );
 
    RooExtendPdf ExtendedSignal  ( "ExtendedSignal", "ExtendedSignal", GaussSum, SigNum );
    RooExtendPdf ExtendedBackCP  ( "ExtendedBackCP", "ExtendedBackCP", LBkgExpo, BkgNum );

    RooAddPdf  model12  ( "modell2", "Signal + Background 12", RooArgList(ExtendedBackCP ,ExtendedSignal) );
 */

  RooAddPdf model12 ("model12", "model12", RooArgList(LBkgExpo, gauss1), gfrac1);

   // model12.fitTo( *data, Extended( kTRUE )); 
    LBkgExpo.fitTo( data ); 
    model12.fitTo(data);

    RooNLLVar nll("nll", "nll", LBkgExpo, data);
    cout<<nll.getVal()<<endl;

    TCanvas* c1 = new TCanvas( "c1",outname.c_str(), 800, 600 );
    c1->SetLeftMargin( 0.15 );
    c1->cd();
    Reco_mass.setBins( nbin );
    RooPlot* h_frame = Reco_mass.frame();
    data.plotOn( h_frame );
   // model12.plotOn( h_frame );
    model12.plotOn( h_frame );
    model12.plotOn( h_frame, Components( "gauss1"),LineStyle( kDashed ), LineColor( kGreen ) );
    model12.plotOn( h_frame, Components( "LBkgExpo"),LineStyle( kDashed ), LineColor( kRed ) );

    h_frame->Draw();

    cout<<"rmass "<<   rmass.getVal() <<endl;  
    cout<<"widt1 "<<   widt1.getVal() <<endl;      
    cout<<"gfrac1 "<< gfrac1.getVal() <<endl;  
    cout<<"  ResoMean "<<  ResoMean.getVal()  <<endl;
    cout<<"Resolution "<<Resolution.getVal()<<endl;
    cout<<"  LBkgComb "<<  LBkgComb.getVal()  <<endl;

}