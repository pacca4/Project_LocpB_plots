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

	Reco_mass.setBins(nbins);

	RooDataSet* data = Model.generate(Reco_mass, nev);
	TH1* h = data->createHistogram("h", Reco_mass, nbins);
	// ************************************************************************** //

	// ************************************************************************** //
	// Toy distribution
	// ************************************************************************** //
	// Background RooRealVar's
	RooRealVar ToyMC_BkgTau("ToyMC_BkgTau", "ToyMC Bkg IsoTrk ExpDecay Lifetime", -0.0302445, -1., 0., "GeV");
	// Background PDF
	RooExponential ToyMC_Model("ToyMC_BkgModel", "ToyMC Bkg IsoTrk ExpDecay PDF", Reco_mass, ToyMC_BkgTau);


	RooDataHist ToyMC_data("ToyMC_data", "ToyMC Binned data", Reco_mass, h);

	ToyMC_Model.fitTo(ToyMC_data, Verbose(kFALSE), PrintLevel(-1), PrintEvalErrors(-1), Warnings(kFALSE));
	// ************************************************************************** //


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





void ToyMC_Batch_IsoTrk_v05(Int_t batch_id, Int_t nsamples=1000, Int_t nbins=100, Int_t nev=500000, Double_t mu=1.0) {

	RooMsgService::instance().setSilentMode(true);
	RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
	RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);

	ofstream ofile(Form("../../../macros/MC/sampling_tests/v05/Batch/IsoTrk/ToyMC_Batch_IsoTrk__nsamples_%d__nbins_%d__nev_%d__mu_%1.1f__%d.txt", nsamples, nbins, nev, mu, batch_id));

	for (int i=0; i<nsamples; ++i) {
		ofile << runToyModel(nbins, nev, mu) << endl;
	}
}
