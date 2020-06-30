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


void Hfit_2G_CB (const string& file, double xmin=0, double xmax=200 , int nbin=400,
                double mass=126, const string& outname="H_2muG" , 
                double sigma1=1, double sigma2=4, double sigma3=2,
                double signalFrac=0.5 , double bkg_frac=0.5){

    ofstream oo ((outname+"_outParams.txt").c_str());
    const char* list = file.c_str(); 


    RooRealVar m( "m", "Reconstructed Mass",    xmin, xmax, "GeV" );
    RooDataSet*       data = RooDataSet::read( list, RooArgList( m) );

    RooRealVar rmass (  "rmass",   "Invariant  Mass",   mass,   xmin,  xmax, "GeV" );
    RooRealVar widt1( "widt1", "Mass Widt1", sigma1,  0.001, sigma1*10, "GeV" );
    RooRealVar widt2( "widt2", "Mass Widt2", sigma2,  0.001, sigma2*10, "GeV" ); 
    RooRealVar widt3( "widt3", "Mass Widt3", sigma2,  0.001, sigma3*10, "GeV" ); 

    RooRealVar gfrac1   ( "gfrac1", "signal fraction1", signalFrac , 0.0, 1.0 );
    RooRealVar gfrac2   ( "gfrac2", "signal fraction2", signalFrac , 0.0, 1.0 );

    RooGaussian    gauss1( "gauss1",     "Signal Distribution1", m, rmass, widt1 );
    RooGaussian    gauss2( "gauss2",     "Signal Distribution2", m, rmass, widt2 );
    RooGaussian    gauss3( "gauss3",     "Signal Distribution2", m, rmass, widt3 );

    RooAddPdf GaussSum ( "GaussSum", "Signal"  , RooArgList( gauss1, gauss2, gauss3), 
                                                 RooArgList( gfrac1, gfrac1 ));


    RooRealVar sigma ("sigma", "CB sigma", 3, 0.1, 10, "Gev");
    RooRealVar n ("n", "CB n", 2, 0, 10, "Gev");
    RooRealVar alpha ("alpha", "CB alpha", 1, 0, 20, "Gev");
    RooCBShape CBBkg ("CBBkg", "Cristall ball", m , rmass, sigma, n, alpha); 
    
    RooRealVar BFrac( "BFrac", "Bkg fraction"       , bkg_frac, 0.0, 1.0, "." );
    


    int          TotEvents = data->numEntries();
    float        SigEvents = TotEvents * signalFrac;
    float        BkgEvents = TotEvents * ( 1.0 - signalFrac );

    RooRealVar   SigNum ( "SigNum", "Signal number", 0, TotEvents );
    RooRealVar   BkgNum ( "BkgNum", "Backgr number", 0, TotEvents );
 
    RooExtendPdf ExtendedSignal  ( "ExtendedSignal", "ExtendedSignal", GaussSum, SigNum );
    RooExtendPdf ExtendedBackCP  ( "ExtendedBackCP", "ExtendedBackCP", CBBkg, BkgNum );

    RooAddPdf  model12  ( "modell2", "Signal + Background 12", RooArgList(ExtendedBackCP ,ExtendedSignal) );
 

    model12.fitTo( *data, Extended( kTRUE )); 

 
    TCanvas* c1 = new TCanvas( "c1",outname.c_str(), 800, 600 );
    c1->SetLeftMargin( 0.15 );
    c1->cd();
    m.setBins( nbin );
    RooPlot* h_frame = m.frame();
   
    RooArgSet params = RooArgList(rmass, widt1, widt2, widt3, sigma) ;
    model12.paramOn(h_frame,Layout(0.7, 0.95, 0.9), Format("NE", AutoPrecision()), Parameters(params));

    data->plotOn( h_frame );
    model12.plotOn( h_frame );
    model12.plotOn( h_frame, Components( "CBBkg" ),  LineStyle( kDashed ), LineColor( kGreen ) );
    model12.plotOn( h_frame, Components( "GaussSum"),LineStyle( kDashed ), LineColor( csig ) );
   // model12.plotOn( mfrtmp, Components( "gauss1"),LineStyle( kDashed ), LineColor( kOrange ) );
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


  c1->Print("Hfit_2G_CB.pdf");
}