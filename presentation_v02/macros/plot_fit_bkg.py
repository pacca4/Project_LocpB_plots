import ROOT

from ROOT import TFile, TCanvas
from ROOT import TH1F, TH1
from ROOT import TLegend, TLatex, TString
from ROOT import gRandom, gPad, gROOT, gStyle, gErrorIgnoreLevel
from ROOT import kTRUE, kFALSE, kGreen, kRed, kBlue

from ROOT import RooRealVar, RooArgSet, RooArgList, RooDataSet, RooAbsData, RooDataHist
from ROOT import RooFit
from ROOT import RooExponential, RooGaussian, RooAddPdf, RooCBShape





#global gErrorIgnoreLevel
ROOT.RooMsgService.instance().setGlobalKillBelow(5)
#RooMsgService::instance().setSilentMode(true);
#RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
#RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);

file_bkg = TFile.Open("data/cutted/bkg.root")
tree_bkg = file_bkg.Events

h = TH1F("h", "h", 40, 70, 150)
tree_bkg.Draw("Reco_mass>>h", "")


Reco_mass = RooRealVar("Reco_mass",  "Reconstructed Mass m_{#mu#mu#gamma}", 70, 150, "GeV/c^{2}")
data_unb  = RooDataSet("data_unbin", "Unbinned data", tree_bkg, RooArgSet(Reco_mass))
data      = RooDataHist("data_bin",  "Binned data", RooArgList(Reco_mass), h)
CutData   = data.reduce("Reco_mass>=90 && (Reco_mass<120 || Reco_mass>130)")

Reco_mass.setRange("lowMass" ,  90, 120)
Reco_mass.setRange("highMass", 130, 150)
Reco_mass.setRange("signal",   120, 130)
Reco_mass.setRange("full",      70, 150)

IsoMu_tau = RooRealVar    ("IsoMu_tau", "IsoMu_tau" , -0.0190705, -1, 0., "GeV/c^{2}")
IsoMu_exp = RooExponential("IsoMu_exp", "IsoMu_exp" , Reco_mass, IsoMu_tau)

IsoMu_exp.fitTo(CutData , RooFit.Range("lowMass,highMass"), RooFit.Verbose(ROOT.kFALSE), RooFit.PrintLevel(-1), RooFit.PrintEvalErrors(-1), RooFit.Warnings(kFALSE))


c1 = TCanvas("c1", "c1", 800, 600);
c1.cd();

Reco_mass.setBins( 40 );
h_frame = Reco_mass.frame();
h_frame.SetTitle("Background m_{#mu#mu#gamma} Distribution Fit");

CutData.plotOn(h_frame);
IsoMu_exp.plotOn(h_frame, RooFit.Range("full",kTRUE), RooFit.LineColor(kRed));
h_frame.Draw();
h.Draw("SAME")

h.SetFillColor(kBlue)
h.SetFillStyle(3003)
gStyle.SetOptStat(0)
MuonGang_label = TLatex()
MuonGang_label.SetTextSize(0.03)
MuonGang_label.DrawLatexNDC(0.10, 0.91, "#it{MuonGang} collaboration")
bkg_ev = TLatex()
MuonGang_label.SetTextSize(0.03)
MuonGang_label.DrawLatexNDC(0.775, 0.91, "#bf{#bkg:} "+str(int(h.GetEntries())))

legend = TLegend(0.78,0.78,0.88,0.88)
legend.AddEntry(h,            "  data", "f")
legend.AddEntry("h_data_bin", "  fit data", "ep")
legend.AddEntry("IsoMu_exp_Norm[Reco_mass]_Range[full]_NormRange[full]", "  exp fit", "l")
legend.SetBorderSize(0)
legend.SetMargin(0.3)
legend.Draw()

c1.Draw()
