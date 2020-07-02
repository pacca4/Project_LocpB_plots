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
	// ************************************************************************** //
	// True distribution
	// ************************************************************************** //
	RooRealVar MuMuG_invMass("MuMuG_invMass", "Reconstructed Mass", 0, 2, "[a.u.]]");
	RooRealVar Lambda("LBkgComb", "combinatorial Bkg life, Left side", -1, "1/[a.u.]");
	RooRealVar SigMean("SigMean","Signal mean", 1, "[a.u.]");
	RooRealVar SigSD("SigSD","Signal s.d.", 0.1, "[a.u.]");

	RooExponential ExpDecay("Exp Decay", "Exponential Decay", MuMuG_invMass, Lambda);

	RooGaussian Signal("signal","signal PDF", MuMuG_invMass, SigMean, SigSD);


	// --- Construct signal+background PDF ---
	RooRealVar nsig("nsig","#signal events",200, 0., 20000);
	RooRealVar nbkg("nbkg","#background events",2000, 0., 20000);
	RooAddPdf model("model","g+a",RooArgList(Signal,ExpDecay), RooArgList(nsig,nbkg));

	RooDataSet* data = model.generate(MuMuG_invMass, 10000);
	TH1* h = data->createHistogram("h", MuMuG_invMass, nbins);


	// ************************************************************************** //
	// Toy test
	// ************************************************************************** //
	// Normalizations:
	//		(#bkg/(#sig+#bkg))*((#events*binWidth)/(1-e^{-2}))
	//		(#sig/(#sig+#bkg))*((#events*binWidth)/(~1))
	TF1* f_num = new TF1("f_fit",  "[0]*[1]*exp([2]*x) + [3]*[4]*gaus(5)", 0, 2);
	TF1* f_den = new TF1("f_true", "[0]*[1]*exp([2]*x) + [3]*[4]*gaus(5)", 0, 2);

	double N_sig = nsig.getVal();
	double N_bkg = nbkg.getVal();
	double N_tot = h->GetEntries();
	double bin_w = h->GetBinWidth(1);

	double norm_0 = (N_bkg/(N_bkg+N_sig)) * N_tot * bin_w;
	double norm_1 = (N_sig/(N_bkg+N_sig)) * N_tot * bin_w;

	// fixed parameters
	f_num->FixParameter(0, norm_0);
	f_num->FixParameter(3, 1.0);    // SM signal strength
	f_num->FixParameter(4, norm_1);
	// parameters initialization
	f_num->SetParameter(1,  1.0);
	f_num->SetParameter(2, -1.0);
	f_num->SetParameter(5,  4.0);
	f_num->SetParameter(6,  1.0);
	f_num->SetParameter(7,  0.1);

	// fixed parameters
	f_den->FixParameter(0, norm_0);
	f_den->FixParameter(4, norm_1);
	// parameters initialization
	f_den->SetParameter(1,  1.0);
	f_den->SetParameter(2, -1.0);
	f_den->SetParameter(3,  1.0); // SM signal strength
	f_den->SetParameter(5,  4.0);
	f_den->SetParameter(6,  1.0);
	f_den->SetParameter(7,  0.1);

	h->Fit(f_num, "LIQ0", "", 0.0, 2.0);
	h->Fit(f_den, "LIQ0", "", 0.0, 2.0);

	// n = numerator
	// d = denominator
	long x_o;
	double x_e_n;
	double x_e_d;
	double nll = 0.;

	for (int i=0; i<nbins; ++i) {
		x_o   = h->GetBinContent(i+1);
		x_e_n = f_num->Eval(h->GetBinCenter(i+1));
		x_e_d = f_den->Eval(h->GetBinCenter(i+1));

		// first approach
		// nll  += (- x_e_n + x_e_d) + x_o*log(x_e_n/x_e_d);
		// from: https://books.google.it/books?id=5-45DwAAQBAJ&pg=PA120&lpg=PA120&dq=toy+montecarlo+loglikelihood+ratio&source=bl&ots=4Xawe9iGBx&sig=ACfU3U1wmi1002ijZ9Teu2cBvS7rn46eKg&hl=it&sa=X&ved=2ahUKEwiE0Yz_uazqAhUVuHEKHd5cCGIQ6AEwA3oECAkQAQ#v=onepage&q=toy%20montecarlo%20loglikelihood%20ratio&f=false
		nll  += (- x_e_n + x_o) + x_o*log(x_e_n/x_o);
	}

	nll *= -2.0;

	delete f_num;
	delete f_den;
	delete h;

	return nll;
	// ************************************************************************** //
}



void ToyMC_Batch_v00(Int_t batch_id, Int_t nsamples=1000, Int_t nbins=100) {

	ofstream ofile(Form("/lustre/cmswork/ardino/LOCP/LOCP_modB_Project/macros/MC/batch_test/ToyMC_batch_%d.txt", batch_id));

	for (int i=0; i<nsamples; ++i) {
		ofile << runToyModel(nbins) << endl;
	}
}
