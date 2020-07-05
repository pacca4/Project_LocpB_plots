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




// ************************************************************************** //
// Draw -2*loglikelihood ratio distribution (sampled from another macro)
// ************************************************************************** //
void ToyMC_DrawPDF_v00(Int_t nbins=100) {

	vector<double> nlls(0);

	ifstream ifile("batch_test_2/ToyMC_Batch_all.txt");

	double t;
	while (ifile >> t) {
		nlls.push_back(t);
	}

	double xmin = *min_element(nlls.begin(), nlls.end());
	double xmax = *max_element(nlls.begin(), nlls.end());

	TH1D* h = new TH1D("h", "h", nbins, xmin, xmax);
	TF1*  f = new TF1("f","[0]*[1]*ROOT::Math::chisquared_pdf(x,[2],0)", xmin, xmax);
	f->SetParameter(0, nlls.size());
	f->SetParameter(1, h->GetBinWidth(1));
	f->SetParameter(2, 95);

	for (int i=0; i<nlls.size(); ++i) {
		h->Fill(nlls[i]);
	}

	h->Draw();
	f->Draw("SAME");
}
// ************************************************************************** //





// ************************************************************************** //
// Draw theoretical distribution
// ************************************************************************** //
void drawSigBkgPDF(Int_t nbins=100) {
	// ************************************************************************** //
	// True distribution
	// ************************************************************************** //
	RooRealVar MuMuG_invMass("MuMuG_invMass", "m_{#mu#mu#gamma}", 0, 2, "[a.u.]]");
	RooRealVar Lambda("Bkg Lambda", "Bkg life", -1, "1/[a.u.]");
	RooRealVar SigMean("SigMean", "Signal mean", 1, "[a.u.]");
	RooRealVar SigSD("SigSD", "Signal s.d.", 0.1, "[a.u.]");

	RooExponential ExpDecay("Exp Decay", "Exponential Decay", MuMuG_invMass, Lambda);

	RooGaussian Signal("Signal","Signal PDF", MuMuG_invMass, SigMean, SigSD);


	// --- Construct signal+background PDF --- //
	RooRealVar nsig("nsig","#signal events", 200, 0., 20000);
	RooRealVar nbkg("nbkg","#background events", 2000, 0., 20000);
	RooAddPdf model("model","g+a",RooArgList(Signal,ExpDecay), RooArgList(nsig,nbkg));

	RooDataSet* data = model.generate(MuMuG_invMass, 10000);

	RooPlot* MuMuG_invMass_frame = MuMuG_invMass.frame();
	data->plotOn(MuMuG_invMass_frame);
	model.plotOn(MuMuG_invMass_frame);
	model.plotOn(MuMuG_invMass_frame, Components(ExpDecay), LineStyle(ELineStyle::kDashed));
	model.plotOn(MuMuG_invMass_frame, Components(Signal),   LineStyle(ELineStyle::kDashed), LineColor(kRed));

	MuMuG_invMass_frame->Draw();
	// ************************************************************************** //
}
// ************************************************************************** //





// ************************************************************************** //
// B + mu*S distribution, depending on mu
// First approach:
// Fit with mu=1, then fix gaussian constant and change mu, then fit again
// ************************************************************************** //
void drawMuSigBkgPDF_1(Int_t nbins=100, Double_t mu=1.0) {
	// ************************************************************************** //
	// True distribution
	// ************************************************************************** //
	RooRealVar MuMuG_invMass("MuMuG_invMass", "m_{#mu#mu#gamma}", 0, 2, "[a.u.]]");
	RooRealVar Lambda("Bkg Lambda", "Bkg life", -1, "1/[a.u.]");
	RooRealVar SigMean("SigMean", "Signal mean", 1, "[a.u.]");
	RooRealVar SigSD("SigSD", "Signal s.d.", 0.1, "[a.u.]");

	RooExponential ExpDecay("Exp Decay", "Exponential Decay", MuMuG_invMass, Lambda);

	RooGaussian Signal("Signal","Signal PDF", MuMuG_invMass, SigMean, SigSD);


	// --- Construct signal+background PDF --- //
	RooRealVar nsig("nsig","#signal events", 200, 0., 20000);
	RooRealVar nbkg("nbkg","#background events", 2000, 0., 20000);
	RooAddPdf model("model","g+a",RooArgList(Signal,ExpDecay), RooArgList(nsig,nbkg));

	RooDataSet* data = model.generate(MuMuG_invMass, 10000);
	TH1* h = data->createHistogram("h", MuMuG_invMass, nbins);
	// ************************************************************************** //


	TF1* f_num = new TF1("f_fit",  "[0]*[1]*exp([2]*x) + [3]*[4]*gaus(5)", 0, 2);
	double N_sig = nsig.getVal();
	double N_bkg = nbkg.getVal();
	double N_tot = h->GetEntries();
	double bin_w = h->GetBinWidth(1);

	double norm_0 = (N_bkg/(N_bkg+N_sig)) * N_tot * bin_w;
	double norm_1 = (N_sig/(N_bkg+N_sig)) * N_tot * bin_w;

	// fixed parameters
	f_num->FixParameter(0, norm_0);
	f_num->FixParameter(3, 1.0);     // SM signal strength
	f_num->FixParameter(4, norm_1);
	// parameters initialization
	f_num->SetParameter(1,  1.0);
	f_num->SetParameter(2, -1.0);
	f_num->SetParameter(5,  4.0);
	f_num->SetParameter(6,  1.0);
	f_num->SetParameter(7,  0.1);

	h->Draw();
	// fit to find the best-fit of theoretical distribution (preliminary operation)
	h->Fit(f_num, "LQ", "", 0.0, 2.0);
	// now fix the gaussian constant (to fix the normalization)
	f_num->FixParameter(5, f_num->GetParameter(5));
	// fix the signal strength
	f_num->FixParameter(3, mu);
	// fit again, with fixed parameters
	h->Fit(f_num, "LQ", "", 0.0, 2.0);
}
// ************************************************************************** //





// ************************************************************************** //
// B + mu*S distribution, depending on mu
// Second approach:
// Fit with mu=1, then change mu
// ************************************************************************** //
void drawMuSigBkgPDF_2(Int_t nbins=100, Double_t mu=1.0) {
	// ************************************************************************** //
	// True distribution
	// ************************************************************************** //
	RooRealVar MuMuG_invMass("MuMuG_invMass", "m_{#mu#mu#gamma}", 0, 2, "[a.u.]]");
	RooRealVar Lambda("Bkg Lambda", "Bkg life", -1, "1/[a.u.]");
	RooRealVar SigMean("SigMean", "Signal mean", 1, "[a.u.]");
	RooRealVar SigSD("SigSD", "Signal s.d.", 0.1, "[a.u.]");

	RooExponential ExpDecay("Exp Decay", "Exponential Decay", MuMuG_invMass, Lambda);

	RooGaussian Signal("Signal","Signal PDF", MuMuG_invMass, SigMean, SigSD);


	// --- Construct signal+background PDF --- //
	RooRealVar nsig("nsig","#signal events", 200, 0., 20000);
	RooRealVar nbkg("nbkg","#background events", 2000, 0., 20000);
	RooAddPdf model("model","g+a",RooArgList(Signal,ExpDecay), RooArgList(nsig,nbkg));

	RooDataSet* data = model.generate(MuMuG_invMass, 10000);
	TH1* h = data->createHistogram("h", MuMuG_invMass, nbins);
	// ************************************************************************** //


	TF1* f_num = new TF1("f_fit",  "[0]*[1]*exp([2]*x) + [3]*[4]*gaus(5)", 0, 2);
	double N_sig = nsig.getVal();
	double N_bkg = nbkg.getVal();
	double N_tot = h->GetEntries();
	double bin_w = h->GetBinWidth(1);

	double norm_0 = (N_bkg/(N_bkg+N_sig)) * N_tot * bin_w;
	double norm_1 = (N_sig/(N_bkg+N_sig)) * N_tot * bin_w;

	// fixed parameters
	f_num->FixParameter(0, norm_0);
	f_num->FixParameter(3, 1.0);     // SM signal strength
	f_num->FixParameter(4, norm_1);
	// parameters initialization
	f_num->SetParameter(1,  1.0);
	f_num->SetParameter(2, -1.0);
	f_num->SetParameter(5,  4.0);
	f_num->SetParameter(6,  1.0);
	f_num->SetParameter(7,  0.1);

	h->Draw();
	// fit to find the best-fit of theoretical distribution (preliminary operation)
	h->Fit(f_num, "LQ", "", 0.0, 2.0);
	// fix the signal strength
	f_num->FixParameter(3, mu);
	// draw
	f_num->Draw("SAME");
}
// ************************************************************************** //
