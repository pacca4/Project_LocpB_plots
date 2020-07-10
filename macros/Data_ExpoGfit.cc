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


void Data_ExpoGfit (const string& file, double xmin=70, double xmax=150 , int nbin=80,
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
       Reco_mass.setBins( nbin );

   
    TFile *f=new TFile(file.c_str(), "READ");
    TTree* EvTree = (TTree*)f->Get("Events_2MuPh");
 // RooDataSet* data = RooDataSet::read( list, RooArgList( m) );
    RooDataSet data_unb ("data", "data", EvTree, Reco_mass);

    TH1* h = data_unb.createHistogram("h", Reco_mass, nbin);
    RooDataHist data("ToyMC_data", "ToyMC Binned data", Reco_mass, h);


   TString Cut = "Reco_mass<120 ||Reco_mass>130";
//  TString Cut = "m>5.5";

 // RooAbsData* CutData = wdata.reduce( Cut );
    RooAbsData *CutData = data.reduce( Cut );

    Reco_mass.setRange("lowMass" ,  70, 120);
    Reco_mass.setRange("highMass", 130, 150);
    Reco_mass.setRange("signal",   120, 130);

 

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


    RooRealVar LBkgComb  ("LBkgComb", "combinatorial Bkg life, Left side"  , 0, -100, 100, "GeV");
    RooRealVar ResoMean  ( "ResoMean",  "Mass Res Mean"  ,   80,   xmin, xmax, "Gev" );
    RooRealVar Resolution( "Resolution","Mass Resolution", 40,  0.001, 100, "GeV" );
    //ResoMean.setConstant( kTRUE );
  //  RooRealVar BFrac( "BFrac", "Bkg fraction"       , bkg_frac, 0.0, 1.0, "." );
    //RooGaussian    gauss1( "gauss1",     "Signal Distribution1", Reco_mass, ResoMean, widt1 );
  
    RooGaussModel ResoGauss( "ResoGauss", "Mass Resolution Gaussian", Reco_mass, ResoMean, Resolution );
  //  RooDecay LBkgExpo( "LBkgExpo", "Left Bk Convolved Decay",   Reco_mass, LBkgComb, ResoGauss, RooDecay::SingleSided );
    RooExponential LBkgExpo( "LBkgExpo", "Left Bk Convolved Decay",   Reco_mass, LBkgComb);//, ResoGauss, RooDecay::SingleSided );
    //RooAddPdf GGexp ("GGexp", )

    RooRealVar coef1( "coef1", "Coefficicent 1"  , -0.0025, -0.10, 0.0, "GeV^-1" );
    RooRealVar coef2( "coef2", "Coefficicent 2"  , 0.03,  -10.0,  10.0, "GeV^-2" );
 
    RooPolynomial polyn( "polyn", "Polynomial Distribution", Reco_mass, RooArgList( coef1, coef2 ) );
   // RooPolyVar polyn( "polyn", "Polynomial Distribution", MuMuG_invMass, RooArgList( coef1 ) );




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
    LBkgExpo.fitTo( *CutData , Range("lowMass,highMass") ); 
    polyn.fitTo( *CutData , Range("lowMass,highMass") );
 //   model12.fitTo(data);

    RooNLLVar nll("nll", "nll", LBkgExpo, data);
    cout<<nll.getVal()<<endl;

    TCanvas* c1 = new TCanvas( "c1",outname.c_str(), 800, 600 );
    c1->SetLeftMargin( 0.15 );
    c1->cd();
    Reco_mass.setBins( nbin );
    RooPlot* h_frame = Reco_mass.frame();
    CutData->plotOn( h_frame );
   // model12.plotOn( h_frame );
  //  polyn.plotOn( h_frame , LineColor( kGreen ));
    LBkgExpo.plotOn( h_frame );
   // model12.plotOn( h_frame );
   // model12.plotOn( h_frame, Components( "gauss1"),LineStyle( kDashed ), LineColor( kGreen ) );
   // model12.plotOn( h_frame, Components( "LBkgExpo"),LineStyle( kDashed ), LineColor( kRed ) );
//
    h_frame->Draw();

  for (int i=0; i<h_frame->numItems(); i++) {
    TString obj_name=h_frame->nameOf(i); if (obj_name=="") continue;
    cout << Form("%d. '%s'\n",i,obj_name.Data());
  }
     
     
 //    RooHist* hresid = h_frame->residHist("h_ToyMC_data", "LBkgExpo_Norm[Reco_mass]", true) ;
  //   RooHist* hresid = h_frame->residHist("h_ToyMC_data", "model12_Norm[Reco_mass]", true) ;
 
    // Construct a histogram with the pulls of the data w.r.t the curve
 
    // Create a new frame to draw the residual distribution and add the distribution to the frame
 //   RooPlot* frame2 = Reco_mass.frame(Title("Residual Distribution")) ;
 //   frame2->addPlotable(hresid,"P") ;
 //TCanvas* c2 = new TCanvas( "c2",outname.c_str(), 800, 600 );
 //  c2->cd();
 // frame2->Draw();

    cout<<"rmass "<<   rmass.getVal() <<endl;  
    cout<<"widt1 "<<   widt1.getVal() <<endl;      
    cout<<"gfrac1 "<< gfrac1.getVal() <<endl;  
    cout<<"  ResoMean "<<  ResoMean.getVal()  <<endl;
    cout<<"Resolution "<<Resolution.getVal()<<endl;
    cout<<"  LBkgComb "<<  LBkgComb.getVal()  <<endl;

}