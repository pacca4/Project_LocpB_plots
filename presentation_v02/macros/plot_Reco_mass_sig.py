import ROOT

from ROOT import TFile
from ROOT import TH1F
from ROOT import TCanvas, TLegend, TLatex
from ROOT import gRandom, gPad, gROOT, gStyle
from ROOT import kTRUE, kGreen, kRed, kBlue

c1 = TCanvas("c1", "c1", 800, 600)

file_sig = TFile.Open("data/cutted/sig.root")
tree_sig = file_sig.Events

h = TH1F("h", "h", 100, 115, 135)
tree_sig.Draw("Reco_mass>>h", "")

h.GetXaxis().SetTitle("Reconstructed mass m_{#mu#mu#gamma} (GeV/c^{2})")
h.GetYaxis().SetTitle("Events/(0.2 GeV/c^{2})")
h.SetTitle("Signal m_{#mu#mu#gamma} distribution")

h.SetLineWidth(1)
h.SetFillColor(kBlue)
h.SetFillStyle(3003)

h.Draw()

gStyle.SetOptStat(0)
MuonGang_label = TLatex()
MuonGang_label.SetTextSize(0.03)
MuonGang_label.DrawLatexNDC(0.10, 0.91, "#it{MuonGang} collaboration")
sig_ev = TLatex()
MuonGang_label.SetTextSize(0.03)
MuonGang_label.DrawLatexNDC(0.775, 0.91, "#bf{#sig:} "+str(int(h.GetEntries())))

c1.Draw()
