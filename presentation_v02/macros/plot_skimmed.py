import ROOT

from ROOT import TFile
from ROOT import TCanvas, TLegend, TLatex
from ROOT import gRandom, gPad, gROOT, gStyle
from ROOT import kTRUE, kGreen, kRed, kBlue





file_bkg = TFile.Open("data/skimmed/bkg.root")
file_sig = TFile.Open("data/skimmed/sig.root")
tree_bkg = file_bkg.Events
tree_sig = file_sig.Events



def draw_feature(feature_name, plot_id, feature_condition="", title="", xlab="", lw=3, lp=[0.78,0.78,0.88,0.88], fill=False):
    tree_bkg.Draw(feature_name, feature_condition, "HIST norm")
    tree_sig.Draw(feature_name, feature_condition, "HIST same norm")
    hist_bkg = tree_bkg.GetHistogram()
    hist_sig = tree_sig.GetHistogram()

    hist_bkg.SetLineColor(kGreen)
    hist_sig.SetLineColor(kRed)
    hist_bkg.SetLineWidth(lw)
    hist_sig.SetLineWidth(lw)

    if fill==True:
        hist_bkg.SetFillColor(kGreen)
        hist_bkg.SetFillStyle(3003)
        hist_sig.SetFillColor(kRed)
        hist_sig.SetFillStyle(3003)

    if hist_bkg.GetMaximum()<hist_sig.GetMaximum():
        hist_bkg.GetYaxis().SetRangeUser(0, hist_sig.GetMaximum()*1.1)
    else:
        hist_bkg.GetYaxis().SetRangeUser(0, hist_bkg.GetMaximum()*1.1)


    legend[plot_id] = TLegend(*lp)
    legend[plot_id].AddEntry(hist_bkg, "  bkg", "f")
    legend[plot_id].AddEntry(hist_sig, "  sig", "f")
    legend[plot_id].SetBorderSize(0)
    legend[plot_id].SetMargin(0.5)
    legend[plot_id].Draw()


    hist_bkg.GetXaxis().SetTitle(xlab if xlab!="" else feature_name) #"Reconstructed mass (Gev/c^{2})")
    hist_bkg.SetTitle(title if title!="" else feature_name+" distribution")

    gStyle.SetOptStat(0)
    MuonGang_label = TLatex()
    MuonGang_label.SetTextSize(0.03)
    MuonGang_label.DrawLatexNDC(0.10, 0.91, "#it{MuonGang} collaboration")
    bkg_ev = TLatex()
    MuonGang_label.SetTextSize(0.03)
    MuonGang_label.DrawLatexNDC(0.775, 0.91, "#bf{#bkg:} "+str(int(hist_bkg.GetEntries())))



feature_list = [
    "Muon_pt[0]",
    "Muon_pt[1]",
    "Photon_pt[0]",
    "Muon_eta[0]",
    "Muon_eta[1]",
    "Photon_eta[0]",
    "JPsi_mass",
    "JPsi_pt",
    "JPsi_eta",
    "AngleCM_Mu0Mu1",
    "DeltaEtaCM_Mu0Mu1",
    "DeltaPhiCM_Mu0Mu1",
    "AngleCM_JpsiPht",
    "DeltaEtaCM_JpsiPht",
    "DeltaPhiCM_JpsiPht",
	"Muon_pfRelIso03_all[0]",
	"Muon_pfRelIso03_all[1]",
	"Photon_pfRelIso03_all[0]",
]
feature_condition_list = [
    "Muon_pt[0] < 150",
    "Muon_pt[1] < 60",
    "Photon_pt[0] < 200",
    "", #"Muon_eta[0]"
    "", #"Muon_eta[1]"
    "", #"Photon_eta[0]"
    "", #"JPsi_mass",
    "JPsi_pt < 250",
    "", #"JPsi_eta",
    "AngleCM_Mu0Mu1 > 0.96", #"AngleCM_Mu0Mu1",
    "abs(DeltaEtaCM_Mu0Mu1) < 0.5", #"DeltaEtaCM_Mu0Mu1",
    "abs(DeltaPhiCM_Mu0Mu1) < 0.5", #"DeltaPhiCM_Mu0Mu1",
    "", #"AngleCM_JpsiPht",
    "abs(DeltaEtaCM_JpsiPht) < 5", #"DeltaEtaCM_JpsiPht",
    "", #"DeltaPhiCM_JpsiPht",
	"Muon_pfRelIso03_all[0] > 0.001   && Muon_pfRelIso03_all[0] < 0.25  ", #"Muon_pfRelIso03_all[0]",
	"Muon_pfRelIso03_all[1] > 0.001   && Muon_pfRelIso03_all[1] < 0.25  ", #"Muon_pfRelIso03_all[1]",
	"Photon_pfRelIso03_all[0] > 0.001 && Photon_pfRelIso03_all[0] < 0.25", #"Photon_pfRelIso03_all[0]",
]
feature_title_list = [
    "Leading muon p_{T} distributions",
    "Subleading muon p_{T} distributions",
    "Photon p_{T} distributions",
    "Leading muon #eta distributions",
    "Subleading muon #eta distributions",
    "Photon #eta distributions",
    "#mu#mu invariant mass distributions",
    "#mu#mu p_{T} distributions",
    "#mu#mu #eta distributions",
    "cos #theta_{#mu#mu} CM distributions",
    "#Delta#eta_{#mu#mu} CM distributions",
    "#Delta#phi_{#mu#mu} CM distributions",
    "cos #theta_{J/#psi#gamma} CM distributions",
    "#Delta#eta_{J/#psi#gamma} CM distributions",
    "#Delta#phi_{J/#psi#gamma} CM distributions",
	"Leading muon isolatation distributions (>0.001)",
    "Subleading muon isolatation distributions (>0.001)",
    "Photon isolatation distributions (>0.001)",
]
feature_xlab_list = [
    "p_{T,#mu[0]} (GeV/c)",
    "p_{T,#mu[1]} (GeV/c)",
    "p_{T,#gamma} (GeV/c)",
    "#eta_{#mu[0]}",
    "#eta_{#mu[1]}",
    "#eta_{#gamma}",
    "m_{#mu#mu} (GeV/c^{2})",
    "p_{T,#mu#mu} (GeV/c)",
    "#eta_{#mu#mu}",
    "cos #theta_{#mu#mu}^{*}",
    "#Delta#eta_{#mu#mu}^{*}",
    "#Delta#phi_{#mu#mu}^{*}",
    "cos #theta_{J/#psi#gamma}^{*}",
    "#Delta#eta_{J/#psi#gamma}^{*}",
    "#Delta#phi_{J/#psi#gamma}^{*}",
	"Isolation #mu[0]",
	"Isolation #mu[1]",
	"Isolation #gamma",
]



nr = 6
nc = 3

c1 = TCanvas("c1", "c1", 320*nc, 240*nr)
c1.Divide(nc,nr)

legend = [0] * nr * nc
for i in range(len(feature_list)):
    c1.cd(i+1)
    draw_feature(feature_list[i], i,
                 feature_condition=feature_condition_list[i],
                 title=feature_title_list[i],
                 xlab=feature_xlab_list[i],
                 lw=2,
                 fill=True)
c1.Draw()
