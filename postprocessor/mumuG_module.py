import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

from PhysicsTools.NanoAODTools.postprocessing.framework.datamodel import Collection 
from PhysicsTools.NanoAODTools.postprocessing.framework.eventloop import Module

class MuMuGProducer(Module):
    def __init__(self):#, jetSelection):
        #self.jetSel = jetSelection
        pass
    def beginJob(self):
        pass
    def endJob(self):
        pass
    def beginFile(self, inputFile, outputFile, inputTree, wrappedOutputTree):
        self.out = wrappedOutputTree
        #self.out.branch("EventMass",  "F");
        self.out.branch("MuMu_invMass", "F");
        self.out.branch("MuMuG_invMass", "F")
    def endFile(self, inputFile, outputFile, inputTree, wrappedOutputTree):
        pass
    def analyze(self, event):
        """process event, return True (go to next module) or False (fail, go to next event)"""
        #electrons = Collection(event, "Electron")
        if event.nMuon <2:
            return False
        if event.nPhoton <1:
            return False
        #muons = Collection(event, "Muon")
        #jets = Collection(event, "Jet")
        #eventSum = ROOT.TLorentzVector()
        #for lep in muons :
        #    eventSum += lep.p4()
        #for lep in electrons :
        #    eventSum += lep.p4()
        
        #self.out.fillBranch("EventMass",eventSum.M())
        Mu = Collection(event, "Muon")
        Ph = Collection(event, "Photon")
        mumu  = ROOT.TLorentzVector()
        mumu  = Mu[0].p4() + Mu[1].p4()
        mumuG = ROOT.TLorentzVector()
        mumuG = Mu[0].p4() + Mu[1].p4() + Ph[0].p4()

        #introduce some light cuts to reduce size of skimmed files
        #we have already done this this of cuts in the previous dataset 
        #these will be lighter but will eliminate lot's of not interesting events

        if ( mumuG.M() < 70  or mumuG.M() > 150 ): return False
        if (  mumu.M() < 2.8 or  mumu.M() > 3.3 ): return False

        if event.Muon_pt[0]   < 10 : return False
        if event.Photon_pt[0] < 20 : return False
        
        self.out.fillBranch( "MuMu_invMass",  mumu.M())
        self.out.fillBranch("MuMuG_invMass", mumuG.M())        

        return True


# define modules using the syntax 'name = lambda : constructor' to avoid having them loaded when not needed

MuMuGModuleConstr = lambda : MuMuGProducer()#jetSelection= lambda j : j.pt >= 0) 
 
