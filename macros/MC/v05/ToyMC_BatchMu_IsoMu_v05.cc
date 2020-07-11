#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "TH1.h"
#include "TF1.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>



using namespace RooFit ;





double runToyModel(Int_t nbins=100, Int_t nev=500000, Double_t mu=1.0) {
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

	Reco_mass.setBins(nbins);

	RooDataSet* data = Model.generate(Reco_mass, nev);
	TH1* h = data->createHistogram("h", Reco_mass, nbins);
	// ************************************************************************** //

	// ************************************************************************** //
	// Toy distribution
	// ************************************************************************** //
	// Background RooRealVar's
	RooRealVar ToyMC_BkgPolyNCoef1("ToyMC_BkgPolyNCoef1", "ToyMC Bkg IsoMu PolyN Coef 1", -0.0104791,   -0.1,   0.1,  "GeV^-1");
	RooRealVar ToyMC_BkgPolyNCoef2("ToyMC_BkgPolyNCoef2", "ToyMC Bkg IsoMu PolyN Coef 2",  2.92647e-05, -0.001, 0.01, "GeV^-2");
	// Background PDF
	RooPolynomial ToyMC_Model("ToyMC_BkgModel", "ToyMC Bkg IsoMu PolyN PDF", Reco_mass, RooArgList(ToyMC_BkgPolyNCoef1, ToyMC_BkgPolyNCoef2));

	RooDataHist ToyMC_data("ToyMC_data", "ToyMC Binned data", Reco_mass, h);

	ToyMC_Model.fitTo(ToyMC_data, Verbose(kFALSE), PrintLevel(-1), PrintEvalErrors(-1), Warnings(kFALSE));


	// ************************************************************************** //
	// Toy test
	// ************************************************************************** //
	// n = numerator
	// d = denominator
	double binw = h->GetBinWidth(1);
	double x_o;
	double N_o;
	double N_e_n;
	double nll = 0.;

	RooArgSet obs(Reco_mass);

	for (int i=0; i<nbins; ++i) {
		x_o   = h->GetBinCenter(i+1);
		N_o   = h->GetBinContent(i+1);

		Reco_mass.setVal(x_o);

		N_e_n = ToyMC_Model.getVal(&obs) * nev * binw;

		// from: https://books.google.it/books?id=5-45DwAAQBAJ&pg=PA120&lpg=PA120&dq=toy+montecarlo+loglikelihood+ratio&source=bl&ots=4Xawe9iGBx&sig=ACfU3U1wmi1002ijZ9Teu2cBvS7rn46eKg&hl=it&sa=X&ved=2ahUKEwiE0Yz_uazqAhUVuHEKHd5cCGIQ6AEwA3oECAkQAQ#v=onepage&q=toy%20montecarlo%20loglikelihood%20ratio&f=false
		nll  += (- N_e_n + N_o) + N_o*log(N_e_n/N_o);
	}

	nll *= -2.0;

	delete data;
	delete h;
	// ************************************************************************** //

	return nll;
}





void ToyMC_BatchMu_IsoMu_v05(Int_t nsamples=1000, Int_t nbins=100, Int_t nev=100000, Double_t mu=1.0) {

	RooMsgService::instance().setSilentMode(true);
	RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
	RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);

	ofstream ofile(Form("../../../macros/MC/sampling_tests/v05/BatchMu/IsoMu/ToyMC_BatchMu_IsoMu__nsamples_%d__nbins_%d__nev_%d__mu_%1.1f.txt", nsamples, nbins, nev, mu));

	for (int i=0; i<nsamples; ++i) {
		ofile << runToyModel(nbins, nev, mu) << endl;
	}
}
