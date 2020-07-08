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
	// RooRealVar    Reco_mass("Reco_mass", "m_{#mu#mu#gamma}",         70.0, 150.0,        "GeV");
	// RooRealVar    BkgTau   ("BkgTau",    "Bkg Lifetime",              1.0,   0.0, 100.0, "GeV"); // 6.33972e+01
	// RooRealVar    ResoMean ("ResoMean",  "Mass Resolution Mean",     50.0,  20.0, 150.0, "GeV"); // 7.52686e+01
	// RooRealVar    ResoSigma("ResoSigma", "Mass Resolution Sigma",    10.0,   0.1, 100.0, "GeV"); // 1.04042e+01
	RooRealVar    Reco_mass("Reco_mass", "m_{#mu#mu#gamma}",       70.0, 150.0,  "GeV");
	RooRealVar    LBkgTau  ("LBkgTau",   "Left Bkg Lifetime",       6.33972e+01, "GeV"); // 6.33972e+01
	RooRealVar    ResoMean ("ResoMean",  "Mass Resolution Mean",    7.52686e+01, "GeV"); // 7.52686e+01
	RooRealVar    ResoSigma("ResoSigma", "Mass Resolution Sigma",   1.04042e+01, "GeV"); // 1.04042e+01
	RooRealVar    SigMean  ("SigMean",   "Signal Mean",           125.0,         "GeV");
	RooRealVar    SigSigma ("SigSigma",  "Signal Sigma",            1.5,         "GeV");
	RooGaussModel ResoGauss("ResoGauss", "Mass Resolution Gaussian", Reco_mass, ResoMean, ResoSigma);
	RooDecay      LBkgExpo ("LBkgExpo",  "Left Bkg Convolved Decay", Reco_mass, LBkgTau,  ResoGauss, RooDecay::SingleSided);
	RooGaussian   Signal   ("SigGauss",  "Signal Gaussian",          Reco_mass, SigMean,  SigSigma);

	// --- Construct signal+background PDF --- //
	RooRealVar NSig("NSig",  "#Signal events",     10*mu);
	RooRealVar NBkg("NBkg",  "#Background events",  2000);
	RooAddPdf Model("Model", "B+mu*S", RooArgList(Signal,LBkgExpo), RooArgList(NSig,NBkg));

	RooDataSet* data = Model.generate(Reco_mass, nev);

	Reco_mass.setBins(nbins);

	RooPlot* Reco_mass_frame = Reco_mass.frame();
	data->plotOn(Reco_mass_frame);
	Model.plotOn(Reco_mass_frame);
	Model.plotOn(Reco_mass_frame, Components(LBkgExpo), LineStyle(ELineStyle::kDashed));
	Model.plotOn(Reco_mass_frame, Components(Signal),   LineStyle(ELineStyle::kDashed), LineColor(kRed));

	Reco_mass_frame->Draw();
	// ************************************************************************** //
}
// ************************************************************************** //
