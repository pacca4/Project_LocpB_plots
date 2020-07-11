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
// Draw theoretical distribution - IsoMu
// ************************************************************************** //
void drawSigBkgIsoMuPDF(Int_t nbins=100, Int_t nev=100000, Double_t mu=1.0) {
	// ************************************************************************** //
	// True distribution
	// ************************************************************************** //
	// Observable
	RooRealVar Reco_mass("Reco_mass", "m_{#mu#mu#gamma}", 70.0, 150.0, "GeV");
	// Background RooRealVar's
	RooRealVar BkgPolyNCoef1("BkgPolyNCoef1", "Bkg IsoMu PolyN Coef 1", -0.0104791,   "GeV^-1");
	RooRealVar BkgPolyNCoef2("BkgPolyNCoef2", "Bkg IsoMu PolyN Coef 2",  2.92647e-05, "GeV^-2");
	// Signal RooRealVar's
	RooRealVar SigRMass      ("SigRMass",       "Sig IsoMu Resonant Mass",    124.793,    "GeV");
	RooRealVar SigGaussSigma1("SigGaussSigma1", "Sig IsoMu Gauss Sigma 1",      0.733419, "GeV");
	RooRealVar SigGaussSigma2("SigGaussSigma2", "Sig IsoMu Gauss Sigma 2",      2.48086,  "GeV");
	RooRealVar SigGaussSigma3("SigGaussSigma3", "Sig IsoMu Gauss Sigma 3",     11.2167,   "GeV");
	RooRealVar SigGaussFrac1 ("SigGaussFrac1",  "Sig IsoMu Gauss Fraction 1",   0.263134       );
	RooRealVar SigGaussFrac2 ("SigGaussFrac2",  "Sig IsoMu Gauss Fraction 2",   0.698942       );
	RooRealVar SigCBSigma    ("SigCBSigma",     "Sig IsoMu CB Sigma",           1.41684,  "Gev");
	RooRealVar SigCBN        ("SigCBN",         "Sig IsoMu CB N",               1.79598,  "Gev");
	RooRealVar SigCBAlpha    ("SigCBAlpha",     "Sig IsoMu CB Alpha",           1.66116,  "Gev");
	RooRealVar SigCBFrac     ("SigCBFrac",      "Sig IsoMu CB Fraction",        0.31048        );
	// Background PDF
	RooPolynomial BkgModel("BkgModel", "Bkg IsoMu PolyN PDF", Reco_mass, RooArgList(BkgPolyNCoef1, BkgPolyNCoef2));
	// Signal PDF
	RooGaussian SigGauss1  ("SigGauss1",   "Sig IsoMu Gauss Distribution 1", Reco_mass, SigRMass, SigGaussSigma1);
	RooGaussian SigGauss2  ("SigGauss2",   "Sig IsoMu Gauss Distribution 2", Reco_mass, SigRMass, SigGaussSigma2);
	RooGaussian SigGauss3  ("SigGauss3",   "Sig IsoMu Gauss Distribution 3", Reco_mass, SigRMass, SigGaussSigma3);
	RooCBShape  SigCB      ("SigCB",       "Sig IsoMu Cristall Ball",        Reco_mass, SigRMass, SigCBSigma, SigCBN, SigCBAlpha);
	RooAddPdf   SigGaussSum("SigGaussSum", "Sig IsoMu Gauss Sum PDF",        RooArgList(SigGauss1, SigGauss2, SigGauss3), RooArgList( SigGaussFrac1, SigGaussFrac2));
	RooAddPdf   SigModel   ("SigModel",    "Sig IsoMu Gauss+CB PDF",         RooArgList(SigGaussSum, SigCB), SigCBFrac);


	// --- Construct signal+background PDF --- //
	RooRealVar NSig("NSig",  "#Signal events",         1.224*0.208*mu);
	RooRealVar NBkg("NBkg",  "#Background events", 20918.);
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





// ************************************************************************** //
// Draw theoretical distribution - IsoTrk
// ************************************************************************** //
void drawSigBkgIsoTrkPDF(Int_t nbins=100, Int_t nev=100000, Double_t mu=1.0) {
	// ************************************************************************** //
	// True distribution
	// ************************************************************************** //
	// Observable
	RooRealVar Reco_mass("Reco_mass", "m_{#mu#mu#gamma}", 70.0, 150.0, "GeV");
	// Background RooRealVar's
	RooRealVar BkgTau("BkgTau", "Bkg IsoTrk ExpDecay Lifetime", -0.0302445,  "GeV");
	// Signal RooRealVar's
	RooRealVar SigRMass      ("SigRMass",       "Sig IsoMu Resonant Mass",    124.795,    "GeV");
	RooRealVar SigGaussSigma1("SigGaussSigma1", "Sig IsoMu Gauss Sigma 1",      0.776242, "GeV");
	RooRealVar SigGaussSigma2("SigGaussSigma2", "Sig IsoMu Gauss Sigma 2",      2.45992,  "GeV");
	RooRealVar SigGaussSigma3("SigGaussSigma3", "Sig IsoMu Gauss Sigma 3",     12.1068,   "GeV");
	RooRealVar SigGaussFrac1 ("SigGaussFrac1",  "Sig IsoMu Gauss Fraction 1",   0.336607       );
	RooRealVar SigGaussFrac2 ("SigGaussFrac2",  "Sig IsoMu Gauss Fraction 2",   0.632098       );
	RooRealVar SigCBSigma    ("SigCBSigma",     "Sig IsoMu CB Sigma",           1.39755,  "Gev");
	RooRealVar SigCBN        ("SigCBN",         "Sig IsoMu CB N",               1.81178,  "Gev");
	RooRealVar SigCBAlpha    ("SigCBAlpha",     "Sig IsoMu CB Alpha",           1.59337,  "Gev");
	RooRealVar SigCBFrac     ("SigCBFrac",      "Sig IsoMu CB Fraction",        0.325142       );
	// Background PDF
	RooExponential BkgModel( "BkgModel", "Bkg IsoTrk ExpDecay PDF", Reco_mass, BkgTau);
	// Signal PDF
	RooGaussian SigGauss1  ("SigGauss1",   "Sig IsoMu Gauss Distribution 1", Reco_mass, SigRMass, SigGaussSigma1);
	RooGaussian SigGauss2  ("SigGauss2",   "Sig IsoMu Gauss Distribution 2", Reco_mass, SigRMass, SigGaussSigma2);
	RooGaussian SigGauss3  ("SigGauss3",   "Sig IsoMu Gauss Distribution 3", Reco_mass, SigRMass, SigGaussSigma3);
	RooCBShape  SigCB      ("SigCB",       "Sig IsoMu Cristall Ball",        Reco_mass, SigRMass, SigCBSigma, SigCBN, SigCBAlpha);
	RooAddPdf   SigGaussSum("SigGaussSum", "Sig IsoMu Gauss Sum PDF",        RooArgList(SigGauss1, SigGauss2, SigGauss3), RooArgList( SigGaussFrac1, SigGaussFrac2));
	RooAddPdf   SigModel   ("SigModel",    "Sig IsoMu Gauss+CB PDF",         RooArgList(SigGaussSum, SigCB), SigCBFrac);


	// --- Construct signal+background PDF --- //
	RooRealVar NSig("NSig",  "#Signal events",        1.224*0.189*mu);
	RooRealVar NBkg("NBkg",  "#Background events", 5901.);
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
