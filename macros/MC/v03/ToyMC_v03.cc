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
#include "TLatex.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>



using namespace RooFit ;





// P.S. : mu logic has to be modified
double runToyModel(Int_t nbins=100, Int_t nev=100000, Double_t mu=1.0) {
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
	RooRealVar NSig("NSig",  "#Signal events",       10.);
	RooRealVar NBkg("NBkg",  "#Background events", 2000.);
	RooAddPdf Model("Model", "B+mu*S", RooArgList(Signal,LBkgExpo), RooArgList(NSig,NBkg));

	Reco_mass.setBins(nbins);

	RooDataSet* data = Model.generate(Reco_mass, nev);
	TH1* h = data->createHistogram("h", Reco_mass, nbins);
	// ************************************************************************** //

	// ************************************************************************** //
	// Toy distribution
	// ************************************************************************** //
	// RooRealVar    Reco_mass("Reco_mass", "m_{#mu#mu#gamma}",         70.0, 150.0,        "GeV");
	// RooRealVar    BkgTau   ("BkgTau",    "Bkg Lifetime",              1.0,   0.0, 100.0, "GeV"); // 6.33972e+01
	// RooRealVar    ResoMean ("ResoMean",  "Mass Resolution Mean",     50.0,  20.0, 150.0, "GeV"); // 7.52686e+01
	// RooRealVar    ResoSigma("ResoSigma", "Mass Resolution Sigma",    10.0,   0.1, 100.0, "GeV"); // 1.04042e+01
	RooRealVar    ToyMC_Reco_mass("Reco_mass", "m_{#mu#mu#gamma}",       70.0, 150.0,               "GeV");
	RooRealVar    ToyMC_LBkgTau  ("LBkgTau",   "Left Bkg Lifetime",       6.33972e+01,  50.,  100., "GeV"); // 6.33972e+01
	RooRealVar    ToyMC_ResoMean ("ResoMean",  "Mass Resolution Mean",    7.52686e+01,  50.,  100., "GeV"); // 7.52686e+01
	RooRealVar    ToyMC_ResoSigma("ResoSigma", "Mass Resolution Sigma",   1.04042e+01,   1.,   20., "GeV"); // 1.04042e+01
	RooRealVar    ToyMC_SigMean  ("SigMean",   "Signal Mean",           125.0,         110.,  140., "GeV");
	RooRealVar    ToyMC_SigSigma ("SigSigma",  "Signal Sigma",            1.5,           0.1,  10., "GeV");
	RooGaussModel ToyMC_ResoGauss("ResoGauss", "Mass Resolution Gaussian", ToyMC_Reco_mass, ToyMC_ResoMean, ToyMC_ResoSigma);
	RooDecay      ToyMC_LBkgExpo ("LBkgExpo",  "Left Bkg Convolved Decay", ToyMC_Reco_mass, ToyMC_LBkgTau,  ToyMC_ResoGauss, RooDecay::SingleSided);
	RooGaussian   ToyMC_Signal   ("SigGauss",  "Signal Gaussian",          ToyMC_Reco_mass, ToyMC_SigMean,  ToyMC_SigSigma);

	// --- Construct signal+background PDF --- //
	RooRealVar ToyMC_NSig ("NSig",  "#Signal events",        10.);
	RooRealVar ToyMC_NBkg ("NBkg",  "#Background events",  2000.);
	RooAddPdf  ToyMC_Model("Model", "B+mu*S", RooArgList(ToyMC_Signal,ToyMC_LBkgExpo), RooArgList(ToyMC_NSig,ToyMC_NBkg));

	RooDataHist ToyMC_data("ToyMC_data", "ToyMC Binned data", Reco_mass, h);

	ToyMC_Model.fitTo(ToyMC_data);

	ToyMC_NSig.setVal(10*mu);

	/*
	RooPlot* ToyMC_Reco_mass_frame = ToyMC_Reco_mass.frame();
	data->plotOn(ToyMC_Reco_mass_frame);
	ToyMC_Model.plotOn(ToyMC_Reco_mass_frame);
	ToyMC_Model.plotOn(ToyMC_Reco_mass_frame, Components(ToyMC_LBkgExpo), LineStyle(ELineStyle::kDashed));
	ToyMC_Model.plotOn(ToyMC_Reco_mass_frame, Components(ToyMC_Signal),   LineStyle(ELineStyle::kDashed), LineColor(kRed));

	ToyMC_Reco_mass_frame->Draw();
	*/
	// ************************************************************************** //


	// ************************************************************************** //
	// Toy test
	// ************************************************************************** //
	// Normalizations:
	//		(#bkg/(#sig+#bkg))*((#events*binWidth)/(1-e^{-2}))
	//		(#sig/(#sig+#bkg))*((#events*binWidth)/(~1))

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

		N_e_n = Model.getVal(&obs) * nev * binw;

		// from: https://books.google.it/books?id=5-45DwAAQBAJ&pg=PA120&lpg=PA120&dq=toy+montecarlo+loglikelihood+ratio&source=bl&ots=4Xawe9iGBx&sig=ACfU3U1wmi1002ijZ9Teu2cBvS7rn46eKg&hl=it&sa=X&ved=2ahUKEwiE0Yz_uazqAhUVuHEKHd5cCGIQ6AEwA3oECAkQAQ#v=onepage&q=toy%20montecarlo%20loglikelihood%20ratio&f=false
		nll  += (- N_e_n + N_o) + N_o*log(N_e_n/N_o);
	}

	nll *= -2.0;

	delete data;
	delete h;
	// ************************************************************************** //


	return nll;
}





void sampleNLL(Int_t nsamples=1000, Int_t nbins1=100, Int_t nbins2=100, Int_t nev=100000, Int_t patience=10, Double_t mu=1.0) {

	RooMsgService::instance().setSilentMode(true);
	RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
	vector<double> nlls(0);

	for (int i=0; i<nsamples; ++i) {
		nlls.push_back(runToyModel(nbins2, nev, mu));
		if(i%patience==0){
			std::cout << "Progress " << int(i*100.0/nsamples) << " %\r";
			std::cout.flush();
		}
	}

	double xmin = *min_element(nlls.begin(), nlls.end());
	double xmax = *max_element(nlls.begin(), nlls.end());

	TH1D* h = new TH1D("h", "h", nbins1, xmin, xmax);

	for (int i=0; i<nsamples; ++i) {
		h->Fill(nlls[i]);
	}

	h->Draw();
	TLatex MuonGang_label;
	MuonGang_label.SetTextSize(0.03);
	MuonGang_label.DrawLatexNDC(0.10, 0.91, "#it{MuonGang} collaboration^{#void1}");
}
