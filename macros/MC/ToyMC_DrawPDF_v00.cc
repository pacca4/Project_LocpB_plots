#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "TH1.h"
#include "TF1.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>



void ToyMC_DrawPDF_v00(Int_t nbins=100) {

	vector<double> nlls(0);

	ifstream ifile("batch_test_1/ToyMC_Batch_all.txt");

	double t;
	while (ifile >> t) {
		nlls.push_back(t);
	}

	double xmin = *min_element(nlls.begin(), nlls.end());
	double xmax = *max_element(nlls.begin(), nlls.end());

	TH1D* h = new TH1D("h", "h", nbins, xmin, xmax);
	TF1*  f = new TF1("f","500000*[0]*ROOT::Math::chisquared_pdf(x,[1],0)", xmin, xmax);
	f->SetParameter(0, h->GetBinWidth(1));
	f->SetParameter(1, 95);

	for (int i=0; i<nlls.size(); ++i) {
		h->Fill(nlls[i]);
	}

	h->Draw();
	f->Draw("SAME");
}
