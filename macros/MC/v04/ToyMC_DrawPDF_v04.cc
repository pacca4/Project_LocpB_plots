#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooNLLVar.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "TH1.h"
#include "TF1.h"
#include "TF1Convolution.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>



using namespace RooFit ;





// ************************************************************************** //
// Draw theoretical distribution
// ************************************************************************** //
void drawSigBkgPDF(Int_t nbins=100, Int_t nev=100000, Double_t mu=1.0) {
	// ************************************************************************** //
	// True distribution
	// ************************************************************************** //
	// Observable
	RooRealVar    Reco_mass("Reco_mass", "m_{#mu#mu#gamma}", 70.0, 150.0, "GeV");
	// Background RooRealVar's
	RooRealVar BkgGaussMean1 ("BkgGaussMean1",  "Bkg Gauss Mean 1",      80.1798,  "GeV");
	RooRealVar BkgGaussSigma1("BkgGaussSigma1", "Bkg Gauss Sigma 1",     20.1929,  "GeV");
	RooRealVar BkgFrac       ("BkgFrac",        "Bkg Gauss Fraction",     0.828563      );
	RooRealVar BkgTau        ("BkgTau" ,        "Bkg ExpDecay Lifetime", 66.3033,  "GeV");
	RooRealVar BkgGaussMean2 ("BkgGaussMean2" , "Bkg Gauss Mean 2"  ,    70.0241,  "Gev");
	RooRealVar BkgGaussSigma2("BkgGaussSigma2", "Bkg Gauss Sigma 2",     19.8339,  "GeV");
	// Signal RooRealVar's
	RooRealVar SigRMass      ("SigRMass",       "Sig Resonant Mass",    124.742,    "GeV");
	RooRealVar SigGaussSigma1("SigGaussSigma1", "Sig Gauss Sigma 1",      0.778463, "GeV");
	RooRealVar SigGaussSigma2("SigGaussSigma2", "Sig Gauss Sigma 2",      2.95579,  "GeV");
	RooRealVar SigGaussSigma3("SigGaussSigma3", "Sig Gauss Sigma 3",     21.2928,   "GeV");
	RooRealVar SigGaussFrac1 ("SigGaussFrac1",  "Sig Gauss Fraction 1",   0.323915       );
	RooRealVar SigGaussFrac2 ("SigGaussFrac2",  "Sig Gauss Fraction 2",   0.615781       );
	RooRealVar SigCBSigma    ("SigCBSigma",     "Sig CB Sigma",           1.57503,  "Gev");
	RooRealVar SigCBN        ("SigCBN",         "Sig CB N",               1.54091,  "Gev");
	RooRealVar SigCBAlpha    ("SigCBAlpha",     "Sig CB Alpha",           2.03489,  "Gev");
	RooRealVar SigCBFrac     ("SigCBFrac",      "Sig CB Fraction",        0.282173       );
	// Background PDF
	RooGaussian   BkgGauss1("BkgGauss1", "Bkg Gauss Distribution 1",        Reco_mass, BkgGaussMean1, BkgGaussSigma1);
	RooGaussModel BkgGauss2("BkgGauss2", "Bkg Gauss Distribution 2",        Reco_mass, BkgGaussMean2, BkgGaussSigma2);
	RooDecay      BkgExpo  ("BkgExpo",   "Bkg Exponential Convolved Decay", Reco_mass, BkgTau,        BkgGauss2, RooDecay::SingleSided );
	RooAddPdf     BkgModel ("BkgModel",  "Bkg Gauss + ConvExp PDF",         RooArgList(BkgExpo, BkgGauss1), BkgFrac);
	// Signal PDF
	RooGaussian SigGauss1  ("SigGauss1",   "Sig Gauss Distribution 1", Reco_mass, SigRMass, SigGaussSigma1);
	RooGaussian SigGauss2  ("SigGauss2",   "Sig Gauss Distribution 2", Reco_mass, SigRMass, SigGaussSigma2);
	RooGaussian SigGauss3  ("SigGauss3",   "Sig Gauss Distribution 3", Reco_mass, SigRMass, SigGaussSigma3);
	RooCBShape  SigCB      ("SigCB",       "Sig Cristall Ball",        Reco_mass, SigRMass, SigCBSigma, SigCBN, SigCBAlpha);
	RooAddPdf   SigGaussSum("SigGaussSum", "Sig Gauss Sum PDF",        RooArgList(SigGauss1, SigGauss2, SigGauss3), RooArgList( SigGaussFrac1, SigGaussFrac2));
	RooAddPdf   SigModel   ("SigModel",    "Sig Gauss+CB PDF",         RooArgList(SigGaussSum, SigCB), SigCBFrac);


	// --- Construct signal+background PDF --- //
	RooRealVar NSig("NSig",  "#Signal events",     10*mu);
	RooRealVar NBkg("NBkg",  "#Background events",  2000);
	RooAddPdf Model("Model", "B+mu*S", RooArgList(SigModel,BkgModel), RooArgList(NSig,NBkg));

	RooDataSet* data = Model.generate(Reco_mass, nev);

	Reco_mass.setBins(nbins);

	RooPlot* Reco_mass_frame = Reco_mass.frame();
	data->plotOn(Reco_mass_frame);
	Model.plotOn(Reco_mass_frame);
	Model.plotOn(Reco_mass_frame, Components(BkgModel), LineStyle(ELineStyle::kDashed));
	Model.plotOn(Reco_mass_frame, Components(SigModel), LineStyle(ELineStyle::kDashed), LineColor(kRed));

	Reco_mass_frame->Draw();
	// ************************************************************************** //
}
// ************************************************************************** //
