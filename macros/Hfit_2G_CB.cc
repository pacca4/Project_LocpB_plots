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

using namespace RooFit ;


EColor csig = kRed;
EColor csi2 = kViolet;
EColor cbkg = kGreen;
EColor cmis = kOrange;


void Hfit_2G_CB (const string& file, double xmin=70, double xmax=150 , int nbin=160,
                double mass=126, const string& outname="H_2muG" , 
                double sigma1=1, double sigma2=4, double sigma3=20,
                double signalFrac=0.5 , double bkg_frac=0.5){

    ofstream oo ((outname+"_outParams.txt").c_str());
    const char* list = file.c_str(); 


    //RooRealVar m( "m", "Reconstructed Mass",    xmin, xmax, "GeV" );
    //RooDataSet*       data = RooDataSet::read( list, RooArgList( m) );

    RooRealVar Reco_mass( "Reco_mass", "Reconstructed Mass",    xmin, xmax, "GeV" );
    TFile *f=new TFile(file.c_str(), "READ");
    TTree* EvTree = (TTree*)f->Get("Events_IsoMu");
 // RooDataSet* data = RooDataSet::read( list, RooArgList( m) );
    RooDataSet data_unb ("data", "data", EvTree, Reco_mass);

    TH1* h = data_unb.createHistogram("h", Reco_mass, nbin);
    RooDataHist data("ToyMC_data", "ToyMC Binned data", Reco_mass, h);

    
    RooRealVar rmass (  "rmass",   "Invariant  Mass",   mass,   xmin,  xmax, "GeV" );
    RooRealVar widt1( "widt1", "Mass Widt1",  2,  0.001, sigma1*10, "GeV" );
    RooRealVar widt2( "widt2", "Mass Widt2",  4,  0.001, sigma2*10, "GeV" ); 
    RooRealVar widt3( "widt3", "Mass Widt3", 24,  0.001, sigma3*10, "GeV" ); 

    RooRealVar gfrac1   ( "gfrac1", "signal fraction1", signalFrac , 0.0, 1.0 );
    RooRealVar gfrac2   ( "gfrac2", "signal fraction2", signalFrac , 0.0, 1.0 );

    RooGaussian    gauss1( "gauss1",     "Signal Distribution1", Reco_mass, rmass, widt1 );
    RooGaussian    gauss2( "gauss2",     "Signal Distribution2", Reco_mass, rmass, widt2 );
    RooGaussian    gauss3( "gauss3",     "Signal Distribution2", Reco_mass, rmass, widt3 );

    RooAddPdf GaussSum ( "GaussSum", "Signal"  , RooArgList( gauss1, gauss2, gauss3), 
                                                 RooArgList( gfrac1, gfrac2 ));
    //RooAddPdf GaussSum ( "GaussSum", "Signal"  , RooArgList( gauss1, gauss2), 
    //                                             RooArgList( gfrac1 ));



    RooRealVar sigma ("sigma", "CB sigma", 2.25, 0.1, 10, "Gev");
    RooRealVar n ("n", "CB n", 2, 0, 10, "Gev");
    RooRealVar alpha ("alpha", "CB alpha", 1.5, 0, 3, "Gev");
    RooCBShape CBBkg ("CBBkg", "Cristall ball", Reco_mass , rmass, sigma, n, alpha); 
    
    RooRealVar BFrac( "BFrac", "Bkg fraction"       , bkg_frac, 0.0, 1.0, "." );
    


    int          TotEvents = data.numEntries();
    float        SigEvents = TotEvents * signalFrac;
    float        BkgEvents = TotEvents * ( 1.0 - signalFrac );

    RooRealVar   SigNum ( "SigNum", "Signal number", 0, TotEvents );
    RooRealVar   BkgNum ( "BkgNum", "Backgr number", 0, TotEvents );
 
   // RooExtendPdf ExtendedSignal  ( "ExtendedSignal", "ExtendedSignal", GaussSum, SigNum );
    //RooExtendPdf ExtendedSignal  ( "ExtendedSignal", "ExtendedSignal", gauss1, SigNum );
   // RooExtendPdf ExtendedBackCP  ( "ExtendedBackCP", "ExtendedBackCP", CBBkg, BkgNum );

    RooAddPdf  model12  ( "model12", "Signal ", RooArgList(GaussSum, CBBkg), BFrac );
 
    cout<<"ok\n";

    model12.fitTo( data ); 
   // CBBkg.fitTo(data);
   // CBBkg.fitTo(data);
    

 
    TCanvas* c1 = new TCanvas( "c1",outname.c_str(), 800, 600 );
    c1->SetLeftMargin( 0.15 );
    c1->cd();
    Reco_mass.setBins( nbin );
    RooPlot* h_frame = Reco_mass.frame();
   
    RooArgSet params = RooArgList(rmass, widt1, widt2, widt3, sigma) ;
    model12.paramOn(h_frame,Layout(0.7, 0.95, 0.9), Format("NE", AutoPrecision()), Parameters(params));

    data.plotOn( h_frame );
    model12.plotOn( h_frame );
    model12.plotOn( h_frame, Components( "CBBkg" ),  LineStyle( kDashed ), LineColor( kGreen ) );
    model12.plotOn( h_frame, Components( "GaussSum"),LineStyle( kDashed ), LineColor( csig ) );
   // model12.plotOn( h_frame, Components( "gauss1"),LineStyle( kDashed ), LineColor( kOrange ) );
   // model12.plotOn( mfrtmp, Components( "gauss2"),LineStyle( kDashed ), LineColor( kGreen ) );
  h_frame->Draw();
    cout << "dframe objects:\n";
  for (int i=0; i<h_frame->numItems(); i++) {
    TString obj_name=h_frame->nameOf(i); if (obj_name=="") continue;
    cout << Form("%d. '%s'\n",i,obj_name.Data());
  }
TString mnames[] = {
  "h_dataset", "modell2_Norm[m]", "modell2_Norm[m]_Comp[CBBkg]",
   "modell2_Norm[m]_Comp[GaussSum]" };

  TString msigns[] = {
    "data",
    "Model",
    "CB",
    "Gauss"    
  };
  /*

  TLegend* mleg = new TLegend(0.1,0.9,0.10,0.90);
  Int_t im=-1;
  while ( mnames[++im] != "" ) {
    TObject *obj = h_frame->findObject(mnames[im].Data());
    if (!obj) {
     // Warning("fitBi4",Form("Can't find item='%s' in the frame2!\n",names[i].Data()));
      cout<<"non trovato\n";
      continue;
    }
    mleg->AddEntry(obj,msigns[im],"l");
  }
  mleg->Draw("same");
*/

  c1->Print("Hfit_2G_CB.pdf");
}