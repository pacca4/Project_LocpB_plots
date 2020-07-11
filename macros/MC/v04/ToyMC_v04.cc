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
	// Observable
	RooRealVar Reco_mass("Reco_mass", "m_{#mu#mu#gamma}", 70.0, 150.0, "GeV");
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

	Reco_mass.setBins(nbins);

	RooDataSet* data = Model.generate(Reco_mass, nev);
	TH1* h = data->createHistogram("h", Reco_mass, nbins);
	// ************************************************************************** //

	// ************************************************************************** //
	// Toy distribution
	// ************************************************************************** //
	// Observable
	RooRealVar ToyMC_Reco_mass("Reco_mass", "m_{#mu#mu#gamma}", 70.0, 150.0, "GeV");
	// Background RooRealVar's
	RooRealVar ToyMC_BkgGaussMean1 ("ToyMC_BkgGaussMean1",  "ToyMC Bkg Gauss Mean 1",      80.1798,   70., 150., "GeV");
	RooRealVar ToyMC_BkgGaussSigma1("ToyMC_BkgGaussSigma1", "ToyMC Bkg Gauss Sigma 1",     20.1929,   10.,  50., "GeV");
	RooRealVar ToyMC_BkgFrac       ("ToyMC_BkgFrac",        "ToyMC Bkg Gauss Fraction",     0.828563,  0.,   1.       );
	RooRealVar ToyMC_BkgTau        ("ToyMC_BkgTau" ,        "ToyMC Bkg ExpDecay Lifetime", 66.3033,   10., 100., "GeV");
	RooRealVar ToyMC_BkgGaussMean2 ("ToyMC_BkgGaussMean2" , "ToyMC Bkg Gauss Mean 2"  ,    70.0241,   50., 100., "Gev");
	RooRealVar ToyMC_BkgGaussSigma2("ToyMC_BkgGaussSigma2", "ToyMC Bkg Gauss Sigma 2",     19.8339,   10.,  50., "GeV");
	// Signal RooRealVar's
	RooRealVar ToyMC_SigRMass      ("ToyMC_SigRMass",       "ToyMC Sig Resonant Mass",    124.742,    110.,   140., "GeV");
	RooRealVar ToyMC_SigGaussSigma1("ToyMC_SigGaussSigma1", "ToyMC Sig Gauss Sigma 1",      0.778463,   0.01,  40., "GeV");
	RooRealVar ToyMC_SigGaussSigma2("ToyMC_SigGaussSigma2", "ToyMC Sig Gauss Sigma 2",      2.95579,    0.01,  40., "GeV");
	RooRealVar ToyMC_SigGaussSigma3("ToyMC_SigGaussSigma3", "ToyMC Sig Gauss Sigma 3",     21.2928,     0.01,  40., "GeV");
	RooRealVar ToyMC_SigGaussFrac1 ("ToyMC_SigGaussFrac1",  "ToyMC Sig Gauss Fraction 1",   0.323915,   0.,     1.       );
	RooRealVar ToyMC_SigGaussFrac2 ("ToyMC_SigGaussFrac2",  "ToyMC Sig Gauss Fraction 2",   0.615781,   0.,     1.       );
	RooRealVar ToyMC_SigCBSigma    ("ToyMC_SigCBSigma",     "ToyMC Sig CB Sigma",           1.57503,    0.1,   10., "Gev");
	RooRealVar ToyMC_SigCBN        ("ToyMC_SigCBN",         "ToyMC Sig CB N",               1.54091,    0.1,   10., "Gev");
	RooRealVar ToyMC_SigCBAlpha    ("ToyMC_SigCBAlpha",     "ToyMC Sig CB Alpha",           2.03489,    0.,     5., "Gev");
	RooRealVar ToyMC_SigCBFrac     ("ToyMC_SigCBFrac",      "ToyMC Sig CB Fraction",        0.282173,   0.,     1.       );
	// Background PDF
	RooGaussian   ToyMC_BkgGauss1("ToyMC_BkgGauss1", "ToyMC Bkg Gauss Distribution 1",        ToyMC_Reco_mass, ToyMC_BkgGaussMean1, ToyMC_BkgGaussSigma1);
	RooGaussModel ToyMC_BkgGauss2("ToyMC_BkgGauss2", "ToyMC Bkg Gauss Distribution 2",        ToyMC_Reco_mass, ToyMC_BkgGaussMean2, ToyMC_BkgGaussSigma2);
	RooDecay      ToyMC_BkgExpo  ("ToyMC_BkgExpo",   "ToyMC Bkg Exponential Convolved Decay", ToyMC_Reco_mass, ToyMC_BkgTau,        ToyMC_BkgGauss2, RooDecay::SingleSided);
	RooAddPdf     ToyMC_BkgModel ("ToyMC_BkgModel",  "ToyMC Bkg Gauss + ConvExp PDF",         RooArgList(ToyMC_BkgExpo, ToyMC_BkgGauss1), ToyMC_BkgFrac);
	// Signal PDF
	RooGaussian ToyMC_SigGauss1  ("ToyMC_SigGauss1",   "ToyMC Sig Gauss Distribution 1", ToyMC_Reco_mass, ToyMC_SigRMass, ToyMC_SigGaussSigma1);
	RooGaussian ToyMC_SigGauss2  ("ToyMC_SigGauss2",   "ToyMC Sig Gauss Distribution 2", ToyMC_Reco_mass, ToyMC_SigRMass, ToyMC_SigGaussSigma2);
	RooGaussian ToyMC_SigGauss3  ("ToyMC_SigGauss3",   "ToyMC Sig Gauss Distribution 3", ToyMC_Reco_mass, ToyMC_SigRMass, ToyMC_SigGaussSigma3);
	RooCBShape  ToyMC_SigCB      ("ToyMC_SigCB",       "ToyMC Sig Cristall Ball",        ToyMC_Reco_mass, ToyMC_SigRMass, ToyMC_SigCBSigma, ToyMC_SigCBN, ToyMC_SigCBAlpha);
	RooAddPdf   ToyMC_SigGaussSum("ToyMC_SigGaussSum", "ToyMC Sig Gauss Sum PDF",        RooArgList(ToyMC_SigGauss1, ToyMC_SigGauss2, ToyMC_SigGauss3), RooArgList(ToyMC_SigGaussFrac1, ToyMC_SigGaussFrac2));
	RooAddPdf   ToyMC_SigModel   ("ToyMC_SigModel",    "ToyMC Sig Gauss+CB PDF",         RooArgList(ToyMC_SigGaussSum, ToyMC_SigCB), ToyMC_SigCBFrac);


	// --- Construct signal+background PDF --- //
	RooRealVar ToyMC_NSig ("ToyMC_NSig",  "#Signal events",     10*mu);
	RooRealVar ToyMC_NBkg ("ToyMC_NBkg",  "#Background events",  2000);
	RooAddPdf  ToyMC_Model("ToyMC_Model", "B+mu*S", RooArgList(ToyMC_SigModel,ToyMC_BkgModel), RooArgList(ToyMC_NSig,ToyMC_NBkg));

	RooDataHist ToyMC_data("ToyMC_data", "ToyMC Binned data", ToyMC_Reco_mass, h);

	ToyMC_Model.fitTo(ToyMC_data, Verbose(kFALSE), PrintLevel(-1), PrintEvalErrors(-1), Warnings(kFALSE));

	ToyMC_NSig.setVal(10*mu);

	/*
	RooPlot* ToyMC_Reco_mass_frame = ToyMC_Reco_mass.frame();
	data->plotOn(ToyMC_Reco_mass_frame);
	ToyMC_Model.plotOn(ToyMC_Reco_mass_frame);
	ToyMC_Model.plotOn(ToyMC_Reco_mass_frame, Components(ToyMC_BkgModel), LineStyle(ELineStyle::kDashed));
	ToyMC_Model.plotOn(ToyMC_Reco_mass_frame, Components(ToyMC_SigModel), LineStyle(ELineStyle::kDashed), LineColor(kRed));

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
	RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);
	vector<double> nlls(0);

	for (int i=0; i<nsamples; ++i) {
		nlls.push_back(runToyModel(nbins2, nev, mu));
		if(i%patience==0){
			std::cout << "Progress " << int(i*100.0/nsamples) << " %\r";
			std::cout.flush();
		}
	}
	std::cout << endl;

	double xmin = *min_element(nlls.begin(), nlls.end());
	double xmax = *max_element(nlls.begin(), nlls.end());

	TH1D* h = new TH1D("h", "h", nbins1, xmin, xmax);

	for (int i=0; i<nsamples; ++i) {
		h->Fill(nlls[i]);
	}

	h->Draw();
	TLatex MuonGang_label;
	MuonGang_label.SetTextSize(0.03);
	MuonGang_label.DrawLatexNDC(0.10, 0.91, "#it{MuonGang} collaboration");
}
