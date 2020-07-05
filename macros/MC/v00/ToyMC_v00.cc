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



double runToyModel(Int_t nbins=100) {

	RooRealVar MuMuG_invMass("MuMuG_invMass", "Reconstructed Mass", 0, 2, "[a.u.]]");
	RooRealVar Lambda("LBkgComb", "combinatorial Bkg life, Left side", -1, "1/[a.u.]");
	// RooRealVar SigMean("SigMean","Signal mean", 1, "[a.u.]");
	// RooRealVar SigSD("SigSD","Signal s.d.", 0.1, "[a.u.]");

	RooExponential ExpDecay("Exp Decay", "Exponential Decay", MuMuG_invMass, Lambda);

	// RooGaussian Signal("signal","signal PDF", MuMuG_invMass, SigMean, SigSD);


	// --- Construct signal+background PDF ---
	// RooRealVar nsig("nsig","#signal events",200, 0., 20000);
	// RooRealVar nbkg("nbkg","#background events",2000, 0., 20000);
	// RooAddPdf model("model","g+a",RooArgList(Signal,ExpDecay), RooArgList(nsig,nbkg));

	RooDataSet* data = ExpDecay.generate(MuMuG_invMass, 10000);
	TH1* h = data->createHistogram("h", MuMuG_invMass, nbins);


	RooPlot* MuMuG_invMass_frame = MuMuG_invMass.frame();
	data->plotOn(MuMuG_invMass_frame);
	ExpDecay.plotOn(MuMuG_invMass_frame);
	// model.plotOn(MuMuG_invMass_frame, Components(ExpDecay), LineStyle(ELineStyle::kDashed));








	RooRealVar Toy_MuMuG_invMass("Toy_MuMuG_invMass", "Reconstructed Mass", 0, 2, "[a.u.]]");
	RooRealVar Toy_Lambda("Toy_LBkgComb", "combinatorial Bkg life, Left side", -1, "1/[a.u.]");
	// RooRealVar Toy_SigMean("SigMean","Signal mean", 1, "[a.u.]");
	// RooRealVar Toy_SigSD("SigSD","Signal s.d.", 0.1, "[a.u.]");

	RooExponential Toy_ExpDecay("Toy_ExpDecay", "Exponential Decay", Toy_MuMuG_invMass, Toy_Lambda);

	// RooGaussian Toy_Signal("signal","signal PDF", Toy_MuMuG_invMass, Toy_SigMean, Toy_SigSD);


	// --- Construct signal+background PDF ---
	// RooRealVar Toy_nsig("nsig","#signal events", 200, 0., 20000);
	// RooRealVar Toy_nbkg("nbkg","#background events", 2000, 0., 20000);
	// RooAddPdf Toy_model("model","g+a", RooArgList(Toy_Signal,Toy_ExpDecay), RooArgList(Toy_nsig,Toy_nbkg));

	// Toy_ExpDecay.fitTo(*data);
	// Toy_ExpDecay.plotOn(MuMuG_invMass_frame, LineColor(kBlue));


	// MuMuG_invMass_frame->Draw();


	TF1* f_fit  = new TF1("f_fit",  "[0]*exp([1]*x)", 0, 2);
	TF1* f_true = new TF1("f_true", "[0]*exp([1]*x)", 0, 2);

	f_true->SetParameter(0, 200.0/(1.0 - 1.0/exp(2.0)));
	f_true->SetParameter(1, -1.0);

	// h->Draw();
	h->Fit(f_fit, "Q0");


	// n = numerator
	// d = denominator
	long x_o;
	double x_e_n;
	double x_e_d;
	double nll   = 0.;
	// double nll_n = 0.;
	// double nll_d = 0.;
	// double lfsa  = 0.; // log factorial stirling approximation

	for (int i=0; i<nbins; ++i) {
		x_o   = h->GetBinContent(i+1);
		x_e_n = f_fit->Eval(h->GetBinCenter(i+1));
		x_e_d = f_true->Eval(h->GetBinCenter(i+1));

		// lfsa  = 0.5*log(2*M_PI*x_o) + x_o*log(x_o/exp(1.0));

		nll  += (- x_e_n + x_e_d) + x_o*log(x_e_n/x_e_d);

		// nll += - x_e + x_o*log(x_e) - lfsa;
	}

	nll *= -2.0;


	//cout << "Negative Log Likelihood: " << nll << endl;

	delete f_fit;
	delete f_true;
	delete h;

	return nll;
}





void sampleNLL(Int_t nsamples=1000, Int_t nbins1=100, Int_t nbins2=100, int patience=10) {
	vector<double> nlls(0);

	for (int i=0; i<nsamples; ++i) {
		nlls.push_back(runToyModel(nbins2));
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
}
