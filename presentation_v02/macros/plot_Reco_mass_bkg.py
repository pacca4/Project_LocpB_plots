import ROOT

from ROOT import TFile
from ROOT import TH1F
from ROOT import TCanvas, TLegend, TLatex
from ROOT import gRandom, gPad, gROOT, gStyle
from ROOT import kTRUE, kGreen, kRed, kBlue





nbins = 60


c1 = TCanvas("c1", "c1", 800, 600)

file_bkg = TFile.Open("data/cutted/bkg.root")
tree_bkg = file_bkg.Events

h = TH1F("h", "h", nbins, 70, 150)
tree_bkg.Draw("Reco_mass>>h", "")

h.GetXaxis().SetTitle("Reconstructed mass m_{#mu#mu#gamma} (GeV/c^{2})")
h.GetYaxis().SetTitle("Events/(1.33333 GeV/c^{2})")
h.SetTitle("Background m_{#mu#mu#gamma} distribution")

h.SetLineWidth(1)
h.SetFillColor(kBlue)
h.SetFillStyle(3003)

h.Draw()

gStyle.SetOptStat(0)
MuonGang_label = TLatex()
MuonGang_label.SetTextSize(0.03)
MuonGang_label.DrawLatexNDC(0.10, 0.91, "#it{MuonGang} collaboration")
bkg_ev = TLatex()
MuonGang_label.SetTextSize(0.03)
MuonGang_label.DrawLatexNDC(0.775, 0.91, "#bf{#bkg:} "+str(int(h.GetEntries())))

c1.Draw()
