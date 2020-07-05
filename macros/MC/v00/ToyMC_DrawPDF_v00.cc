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
