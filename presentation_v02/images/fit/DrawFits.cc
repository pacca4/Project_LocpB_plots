#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooExtendPdf.h"
#include "RooFitResult.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "RooNLLVar.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>

#include "SigFitIsoMu.h"
#include "SigFitIsoTrk.h"
#include "BkgFitIsoMu.h"
#include "BkgFitIsoTrk.h"
//#include "style.cc"

using namespace RooFit ;


void DrawFits(const string& path="../../data/FinalData/"){
    SigFitIsoMu (path+"Final_SignalIsoMu.root");
    SigFitIsoTrk(path+"Final_SignalIsoTrk.root");
    BkgFitIsoMu (path+"Final_DataIsoMu.root");
    BkgFitIsoTrk(path+"Final_DataIsoTrk.root");
}

