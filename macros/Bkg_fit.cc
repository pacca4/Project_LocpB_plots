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


//void fitModelRQ_v15  (const string& file, const string& outname,
//		     int nbin, double xmin, double xmax,
//		     double xfac, double yfac,
//		     double mass, double sigma1, double sigma2, 
//		     double expc, double signalFrac,
//			   double ap, double bp, double cp, double norm,
//         int tr , bool setReso, bool setMean, bool setFrac ) 
//         //tr indica il trigger in uso 0 nessuno 1 il primo 2 entrambi
//          //i boolenai impostano le cose da fissare
//{


void Bkg_fit(const string& file, const string& outname, 
             float xmin=105, float xmax=150,
             float xlow=120, float xhig=130, int nbin=180 ){



    //ofstream oo ((outname+"outParams.txt").c_str());

    //data from txt
    //const char* list = file.c_str(); //lista dei dati
    // RooDataSet* data = RooDataSet::read( list, RooArgList( m) );

    // data from ttree
 
    RooRealVar MuMuG_invMass( "MuMuG_invMass", "Reconstructed Mass",    xmin, xmax, "GeV" );
    TFile *f=new TFile(file.c_str(), "READ");
    TTree* EvTree = (TTree*)f->Get("Events");
    RooDataSet data ("data", "data", EvTree, MuMuG_invMass);

 //   RooRealVar m( "m", "Reconstructed Mass",    xmin, xmax, "GeV" );
 // RooRealVar d( "d",    "Flight Distance",    dmin, dmax,  "cm" );
 // RooRealVar e( "e",     "Distance Error", 0.00001,  0.2,  "cm" );

  //  RooRealVar rmass (  "rmass",   "Resonance  Mass",   mass,   xmin,  xmax, "GeV" );
  //  RooRealVar widt1( "widt1", "Resonance Widt1", sigma1,  0.001, sigma1*10, "GeV" );
  //  RooRealVar widt2( "widt2", "Resonance Widt2", sigma2,  0.001, sigma2*10, "GeV" ); 
  //  RooRealVar gfrac    ( "gfrac", "signal fraction1", signalFrac , 0.0, 1.0 );
  
// create the fit model components: Gaussian and exponential PDFs
  //  RooGaussian    gauss1( "gauss1",     "Signal Distribution1", m, rmass, widt1 );
  //  RooGaussian    gauss2( "gauss2",     "Signal Distribution2", m, rmass, widt2 );
 
    RooRealVar coef1( "coef1", "Coefficicent 1"  , -0.0025, -0.10, 0.0, "GeV^-1" );
 //   RooRealVar coef2( "coef2", "Coefficicent 2"  , 0.03,  -10.0,  10.0, "GeV^-2" );
 
   // RooPolynomial polyn( "polyn", "Polynomial Distribution", MuMuG_invMass, RooArgList( coef1, coef2 ) );
    RooPolyVar polyn( "polyn", "Polynomial Distribution", MuMuG_invMass, RooArgList( coef1 ) );
  // RooChebychev  polyc( "polyc", "Chebychev Polynomial Distribution", m, RooArgList( coef1, coef2) );

   // RooAddPdf GaussSum ( "GaussSum", "Signal"  , RooArgList( gauss1, gauss2   ), gfrac   );
 
 //taglio un pezzo dei dati per fittare il fondo
    TString Cut = "MuMuG_invMass<"+std::to_string(xlow)+
              " || MuMuG_invMass>"+std::to_string(xhig);
//  TString Cut = "m>5.5";

 // RooAbsData* CutData = wdata.reduce( Cut );
    RooAbsData* CutData = data.reduce( Cut );

    MuMuG_invMass.setRange("lowMass" , xmin, xlow);
    MuMuG_invMass.setRange("highMass", xhig, xmax);
    MuMuG_invMass.setRange("signal",   xlow, xhig);


    float BkgEvents = EvTree->GetEntries();

    RooRealVar   BkgNum ( "BkgNum", "Backgr number", BkgEvents,  BkgEvents/10.0, BkgEvents*10.0 );
    RooExtendPdf ExtendedBackCP  ( "ExtendedBackCP", "ExtendedBackCP",  polyn , BkgNum );

 
    RooFitResult* r = polyn.fitTo( *CutData , Range("lowMass,highMass"),
                     Extended (kFALSE), Save(kTRUE));
    //polyn.fitTo(data);
   // polyn.fitTo( *CutData , Extended (kFALSE) );

    TCanvas* tbg = new TCanvas( "tbg",outname.c_str(), 800, 600 );
    tbg->SetLeftMargin( 0.15 );
    tbg->cd();
    MuMuG_invMass.setBins( nbin );
    RooPlot* mfrtbg = MuMuG_invMass.frame();
    CutData->plotOn( mfrtbg );
    
    polyn.plotOn( mfrtbg );
    mfrtbg->Draw();


    tbg->Print( ( outname + "MassBkg.pdf" ).c_str() );
    tbg->Print( ( outname + "MassBkg.png" ).c_str() );

    r->Print();
    
    cout << "coef1: " << coef1.getVal() << " +/- " << coef1.getError() << endl;
    RooArgSet mset(MuMuG_invMass);
    cout << "Norm:" << (float)data.sumEntries()*(xmax-xmin)/nbin<<endl;
    cout << "Norm:" << polyn.getNorm(&mset)<<"\t"<<polyn.getNorm()<<endl;

    RooRealVar myTemp("myTemp","myTemp",xmin, xmin, xmax, "."); //interested in height/value at 500
    RooArgSet nset(myTemp);
    double normExpVal = ExtendedBackCP.getVal(&nset);
    cout<<normExpVal<<endl;
    cout<<BkgNum.getVal()<<endl;

  //  cout << "coef2: " << coef2.getVal() << " +/- " << coef2.getError() << endl;
  //  cout << "BkgNum: " << BkgNum.getVal() << " +/- " << BkgNum.getError() << endl;
//  return;
 /*
    coef1.setConstant( kTRUE );
      coef2.setConstant( kTRUE );

  // model12.fitTo( wdata, Extended( kTRUE ),SumW2Error(kTRUE)  );
     model12.fitTo( *data, Extended( kTRUE )); 

  // print results
     cout << "results" << endl;
     cout << "rmass: " << rmass.getVal() << " +/- " << rmass.getError() << endl;
     cout << "widt1: " << widt1.getVal() << " +/- " << widt1.getError() << endl;
     cout << "widt2: " << widt2.getVal() << " +/- " << widt2.getError() << endl;
     cout << "gfrac: " << gfrac.getVal() << " +/- " << gfrac.getError() << endl;
 

     float f1 = gfrac.getVal();
     float sigma = sqrt( ( pow( widt1.getVal(), 2 ) *         signalFrac ) +
                      ( pow( widt2.getVal(), 2 ) * ( 1.0 - signalFrac ) ) );
     cout << " sigma = " << sigma << endl;


    TCanvas* tmp = new TCanvas( "tmp",outname.c_str(), 800, 600 );
     tmp->SetLeftMargin( 0.15 );
    tmp->cd();
     m.setBins( nbin );
    RooPlot* mfrtmp = m.frame();
  //  mfrtmp->SetTitle( ptit.c_str() );
  // mfrtmp->SetXTitle( atit.c_str() );
 // wdata.plotOn( mfrtmp );
     data->plotOn( mfrtmp );
  
    model12.plotOn( mfrtmp, Components( "polyn" ),LineStyle( kDashed ), LineColor( kViolet ) );
    model12.plotOn( mfrtmp, Components( "GaussSum"),LineStyle( kDashed ), LineColor( csig ) );
    model12.plotOn( mfrtmp, Components( "gauss1"),LineStyle( kDashed ), LineColor( kOrange ) );
    model12.plotOn( mfrtmp, Components( "gauss2"),LineStyle( kDashed ), LineColor( kGreen ) );
    model12.plotOn( mfrtmp );
    mfrtmp->Draw();
    tmp->Print( ( outname + "Mass.pdf" ).c_str() );
    tmp->Print( ( outname + "Mass.png" ).c_str() );
    cout << "dframe objects:\n";
    for (int i=0; i<mfrtmp->numItems(); i++) {
      TString obj_name=mfrtmp->nameOf(i); if (obj_name=="") continue;
      cout << Form("%d. '%s'\n",i,obj_name.Data());
    }

TString mnames[] = {
    "h_dataset",
    "modell2_Norm[m]_Comp[polyn]",
    "modell2_Norm[m]_Comp[GaussSum]",
    "modell2_Norm[m]",
    ""
  };

  TString msigns[] = {
    "data",
    "Background",
    "Signal",
    "Model"
  };

  TLegend* mleg = new TLegend(0.6,0.9,0.60,0.90);
  Int_t im=-1;
  while ( mnames[++im] != "" ) {
    TObject *obj = mfrtmp->findObject(mnames[im].Data());
    if (!obj) {
     // Warning("fitBi4",Form("Can't find item='%s' in the frame2!\n",names[i].Data()));
      cout<<"non trovato\n";
      continue;
    }
    mleg->AddEntry(obj,msigns[im],"l");
  }
  mleg->Draw("same");
*/
}






/* ******* OLD MODELS ***********

********** DA RIDEFINIRE ESPONENZILE DOPPIO IN CASO ************

  RooDecay DBkgExpo( "DBkgExpo", "Double Bk Convolved Decay", d, DBkgComb, ResoGauss, RooDecay::DoubleSided );


   RooAddPdf RLBkgExpo  ("RLBkgExpo", "Convolved Right and left Bkg",  RooArgList( RBkgExpo  , LBkgExpo), RFrac );
  RooAddPdf AlBkgExpo  ("AlBkgExpo", "Convolved All Bkg",             RooArgList( RLBkgExpo , DBkgExpo), DFrac );
  RooAddPdf AQBkgExpo  ("AQBkgExpo", "Convolved All Bkg + q tail",    RooArgList( RBkgExpo , QBkgExpo,  LBkgExpo, DBkgExpo), 
			                                              RooArgList( RFrac , QFrac , LFrac));
  RooAddPdf RQBkgExpo  ("RQBkgExpo", "Convolved Right 2 and left Bkg", RooArgList( RBkgExpo , QBkgExpo,  LBkgExpo), 
                                                                       RooArgList( RFrac , QFrac));

  RooAddPdf RDBkgExpo  ("RDBkgExpo", "Convolved Right and double Bkg", RooArgList( RBkgExpo  , DBkgExpo), RFrac );
  RooAddPdf R2BkgExpo  ("R2BkgExpo", "Convolved Right 2 Bkg"         , RooArgList( RBkgExpo  , QBkgExpo), RFrac );

  RooAddPdf Expo1GBkg  ("Expo1GBkg", "1 expo+ gauss bkg"   ,    RooArgList(RBkgExpo, BkgGauss), GFrac );

  
  RooAddPdf DSTotDecay ("DSTotDecay", "Combin Total Decay double side" , RooArgList( DBkgExpo  , PromptGauss), BFrac );
  // RooAddPdf RLTotDecay ("RLTotDecay", "Combin Total Decay right left"  , RooArgList( RLBkgExpo , PromptGauss), BFrac );
  RooAddPdf RLTotDecay ("RLTotDecay", "Combin Total Decay right left"  , RooArgList( RLBkgExpo ,  CoreGauss ), BFrac );
  RooAddPdf RQTotDecay ("RQTotDecay", "Combin Total Decay 2 right left", RooArgList( RQBkgExpo ,  CoreGauss ), BFrac );
  RooAddPdf RDTotDecay ("RDTotDecay", "Combin Total Decay right double", RooArgList( RDBkgExpo ,  CoreGauss ), BFrac );
   RooAddPdf R2TotDecay ("R2TotDecay", "Combin Total Decay right 2     ", RooArgList( R2BkgExpo ,  CoreGauss ), BFrac );
 //RooAddPdf R2TotDecay ("R2TotDecay", "Combin Total Decay right 2     ", RooArgList( R2BkgExpo , PromptGauss  ), BFrac );

  RooAddPdf AlTotDecay ("AlTotDecay", "Combin Total Decay all"         , RooArgList( AlBkgExpo , PromptGauss), BFrac );
  
//RooAddPdf AQTotDecay ("AQTotDecay", "Combin Total Decay all + q tail", RooArgList( AQBkgExpo , PromptGauss), BFrac );
   RooAddPdf AQTotDecay ("AQTotDecay", "Combin Total Decay all + q tail", RooArgList( AQBkgExpo , CoreGauss ), BFrac );


  RooAddPdf E1GTotDecay ("E1GTotDecay", "Combin expo gauss and prompt ", RooArgList( Expo1GBkg , PromptGauss), BFrac );

  RooProdPdf EffExp( "EffExp", "EffExp", RooArgSet( SigExpo, EffPol) );
  RooProdPdf Signal( "Signal", "Signal 2D", RooArgSet( GaussSum,EffExp ) );


  //costruisco il modello per fit 2d
  //RooProdPdf Signal( "Signal", "Signal 2D", RooArgSet( GaussSum, SigExpo, EffPol) );
  
  RooProdPdf DSBkg ( "DSBkg", "2D bkg double side" , RooArgSet( polyn  , DSTotDecay ) );
  RooProdPdf RLBkg ( "RLBkg", "2D bkg right left"  , RooArgSet( polyn  , RLTotDecay ) );
  RooProdPdf RQBkg ( "RQBkg", "2D bkg right 2 left", RooArgSet( polyn  , RQTotDecay ) );
  RooProdPdf RDBkg ( "RDBkg", "2D bkg right double", RooArgSet( polyn  , RDTotDecay ) );
  RooProdPdf R2Bkg ( "R2Bkg", "2D bkg right 2"     , RooArgSet( polyn  , R2TotDecay ) );
  RooProdPdf AlBkg ( "AlBkg", "2D bkg all"         , RooArgSet( polyn  , AlTotDecay ) );
  RooProdPdf AQBkg ( "AQBkg", "2D bkg all + q tail", RooArgSet( polyn  , AQTotDecay ) );
  RooProdPdf E1GBkg( "AQBkg",  "2D bkg expo gauss ", RooArgSet( polyn  , E1GTotDecay) );

  
  RooExtendPdf ExtendedSignal2D ( "ExtendedSignal2D", "ExtendedSignal2D", Signal, SigNum);
  RooExtendPdf DSExtendedBackgr ( "DSExtendedBackgr", "DSExtendedBackgr", DSBkg , BkgNum);
  RooExtendPdf RLExtendedBackgr ( "RLExtendedBackgr", "RLExtendedBackgr", RLBkg , BkgNum);
  RooExtendPdf RQExtendedBackgr ( "RQExtendedBackgr", "RQExtendedBackgr", RQBkg , BkgNum);
  RooExtendPdf RDExtendedBackgr ( "RDExtendedBackgr", "RDExtendedBackgr", RDBkg , BkgNum);
  RooExtendPdf R2ExtendedBackgr ( "R2ExtendedBackgr", "R2ExtendedBackgr", R2Bkg , BkgNum);
  RooExtendPdf AlExtendedBackgr ( "AlExtendedBackgr", "AlExtendedBackgr", AlBkg , BkgNum);
  RooExtendPdf AQExtendedBackgr ( "AQExtendedBackgr", "AQExtendedBackgr", AQBkg , BkgNum);
  RooExtendPdf E1GExtendedBackgr( "E1GExtendedBackgr","E1GExtendedBackgr", E1GBkg , BkgNum);

  RooAddPdf ModelDS ("ModelDS", "Signal + bkg double side" , RooArgList( ExtendedSignal2D , DSExtendedBackgr ));
  RooAddPdf ModelRL ("ModelRL", "Signal + bkg right left"  , RooArgList( ExtendedSignal2D , RLExtendedBackgr ));
  RooAddPdf ModelAl ("ModelAL", "Signal + bkg all"         , RooArgList( ExtendedSignal2D , AlExtendedBackgr ));
  RooAddPdf ModelAQ ("ModelAQ", "Signal + bkg all + q tail", RooArgList( ExtendedSignal2D , AQExtendedBackgr ));
  RooAddPdf ModelE1G("ModelE1G", "Signal + bkg expo gauss" , RooArgList( ExtendedSignal2D , E1GExtendedBackgr));
  RooAddPdf ModelRQ ("ModelRQ", "Signal + 2 right left "   , RooArgList( ExtendedSignal2D , RQExtendedBackgr ));
  RooAddPdf ModelRD ("ModelRD", "Signal + right double "   , RooArgList( ExtendedSignal2D , RDExtendedBackgr ));
  RooAddPdf ModelR2 ("ModelR2", "Signal + right 2 "        , RooArgList( ExtendedSignal2D , R2ExtendedBackgr ));

*/
