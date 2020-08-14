import ROOT

from ROOT import TFile, TCanvas
from ROOT import TH1F, TH1
from ROOT import TLegend, TLatex, TString
from ROOT import gRandom, gPad, gROOT, gStyle, gErrorIgnoreLevel
from ROOT import kTRUE, kFALSE
from ROOT import kGreen, kRed, kBlue, kOrange
from ROOT import kDashed, kDashDotted

from ROOT import RooRealVar, RooArgSet, RooArgList, RooDataSet, RooAbsData, RooDataHist
from ROOT import RooFit
from ROOT import RooExponential, RooGaussian, RooAddPdf, RooCBShape





#global gErrorIgnoreLevel
ROOT.RooMsgService.instance().setGlobalKillBelow(5)
#RooMsgService::instance().setSilentMode(true);
#RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
#RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);

file_sig = TFile.Open("data/cutted/sig.root")
tree_sig = file_sig.Events

h = TH1F("h", "h", 100, 115, 135)
tree_sig.Draw("Reco_mass>>h", "")


Reco_mass = RooRealVar("Reco_mass",  "Reconstructed Mass m_{#mu#mu#gamma}", 115, 135, "GeV/c^{2}")
data_unb  = RooDataSet("data_unbin", "Unbinned data", tree_sig, RooArgSet(Reco_mass))
data      = RooDataHist("data_bin",  "Binned data", RooArgList(Reco_mass), h)












IsoMu_mean =  RooRealVar( "IsoMu_mean", "IsoMu_mean",   124.793,   120,  130, "Gev/c^{2}" );
IsoMu_sig1 =  RooRealVar( "IsoMu_sig1", "IsoMu_sig1",  0.733419,   0.01,  10, "Gev/c^{2}" );
IsoMu_sig2 =  RooRealVar( "IsoMu_sig2", "IsoMu_sig2",   2.48086,   0.01,  10, "Gev/c^{2}" );
IsoMu_sig3 =  RooRealVar( "IsoMu_sig3", "IsoMu_sig3",   11.2167,      1, 100, "Gev/c^{2}" );

IsoMu_gfrac1 = RooRealVar( "IsoMu_gfrac1", "IsoMu_gfrac1", 0.263134,  0.0, 1.0 );
IsoMu_gfrac2 = RooRealVar( "IsoMu_gfrac2", "IsoMu_gfrac2", 0.698942,  0.0, 1.0 );

IsoMu_gauss1 = RooGaussian( "IsoMu_gauss1", "IsoMu_gauss1", Reco_mass, IsoMu_mean, IsoMu_sig1 );
IsoMu_gauss2 = RooGaussian( "IsoMu_gauss2", "IsoMu_gauss2", Reco_mass, IsoMu_mean, IsoMu_sig2 );
IsoMu_gauss3 = RooGaussian( "IsoMu_gauss3", "IsoMu_gauss3", Reco_mass, IsoMu_mean, IsoMu_sig3 );

IsoMu_GaussSum = RooAddPdf("IsoMu_GaussSum", "IsoMu_GaussSum",
                           RooArgList(IsoMu_gauss1, IsoMu_gauss2, IsoMu_gauss3),
                           RooArgList(IsoMu_gfrac1, IsoMu_gfrac2 ));

IsoMu_CBSig   = RooRealVar("IsoMu_CBSig"  , "IsoMu_CBSig"  , 1.41684, 0.1, 10, "Gev/c^{2}");
IsoMu_CBn     = RooRealVar("IsoMu_CBn"    , "IsoMu_CBn"    , 1.79598, 0, 10, "Gev/c^{2}");
IsoMu_CBalpha = RooRealVar("IsoMu_CBalpha", "IsoMu_CBalpha", 1.66116, 0, 3, "Gev/c^{2}");
IsoMu_CB      = RooCBShape("IsoMu_CB",      "IsoMu_CB", Reco_mass,
                           IsoMu_mean, IsoMu_CBSig,
                           IsoMu_CBn , IsoMu_CBalpha);

IsoMu_Frac = RooRealVar("IsoMu_Frac", "IsoMu_Frac", 0.31048, 0.0, 1.0, ".");

IsoMu_Model = RooAddPdf("IsoMu_Model", "IsoMu_Model ",
                        RooArgList(IsoMu_GaussSum, IsoMu_CB), RooArgList(IsoMu_Frac));


IsoMu_Model.fitTo(data, RooFit.Verbose(ROOT.kFALSE), RooFit.PrintLevel(-1), RooFit.PrintEvalErrors(-1), RooFit.Warnings(kFALSE))


c1 = TCanvas("c1", "c1", 800, 600);
c1.cd();

Reco_mass.setBins(100);
h_frame = Reco_mass.frame();
h_frame.SetTitle("Signal m_{#mu#mu#gamma} Distribution Fit");

data.plotOn(h_frame)
IsoMu_Model.plotOn(h_frame, RooFit.LineColor(kRed))
IsoMu_Model.plotOn(h_frame, RooFit.Components("IsoMu_GaussSum"), RooFit.LineStyle(kDashed), RooFit.LineColor(kGreen))
IsoMu_Model.plotOn(h_frame, RooFit.Components("IsoMu_CB"), RooFit.LineStyle(kDashDotted), RooFit.LineColor(kOrange))
h_frame.Draw()
h.Draw("SAME")

h.SetFillColor(kBlue)
h.SetFillStyle(3003)
gStyle.SetOptStat(0)
MuonGang_label = TLatex()
MuonGang_label.SetTextSize(0.03)
MuonGang_label.DrawLatexNDC(0.10, 0.91, "#it{MuonGang} collaboration")
sig_ev = TLatex()
MuonGang_label.SetTextSize(0.03)
MuonGang_label.DrawLatexNDC(0.775, 0.91, "#bf{#sig:} "+str(int(h.GetEntries())))

legend = TLegend(0.78,0.73,0.88,0.88)
legend.AddEntry(h,            "  data", "f")
legend.AddEntry("h_data_bin", "  fit data", "ep")
legend.AddEntry("IsoMu_Model_Norm[Reco_mass]", "  sig fit", "l")
legend.AddEntry("IsoMu_Model_Norm[Reco_mass]_Comp[IsoMu_GaussSum]", "  Gauss", "l")
legend.AddEntry("IsoMu_Model_Norm[Reco_mass]_Comp[IsoMu_CB]", "  CB", "l")
legend.SetBorderSize(0)
legend.SetMargin(0.3)
legend.Draw()

c1.Draw()
