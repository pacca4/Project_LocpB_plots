import ROOT
from ROOT.Math import VectorUtil as vUtil
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
        #self.out.branch("MuMu_invMass", "F");
        #self.out.branch("MuMuG_invMass", "F")

        self.out.branch("JPsi_mass", "F")
        self.out.branch("JPsi_pt"  , "F")
        self.out.branch("JPsi_eta" , "F")
        self.out.branch("JPsi_phi" , "F")
        self.out.branch("Reco_mass", "F")
        self.out.branch("Reco_pt"  , "F")
        self.out.branch("Reco_eta" , "F")
        self.out.branch("Reco_phi" , "F")

        self.out.branch("DeltaR_Mu0Mu1", "F")
        self.out.branch("DeltaR_Mu0Pht", "F")
        self.out.branch("DeltaR_Mu1Pht", "F")
        self.out.branch("DeltaR_JpsiPht", "F")
        self.out.branch("DeltaR_RecoMu0", "F")
        self.out.branch("DeltaR_RecoMu1", "F")
        self.out.branch("DeltaR_RecoPht", "F")
        self.out.branch("DeltaR_RecoJPsi", "F") 
        
        self.out.branch("AngleCM_Mu0Mu1", "F")
        self.out.branch("AngleCM_Mu0Pht", "F")
        self.out.branch("AngleCM_Mu1Pht", "F")
        self.out.branch("AngleCM_JpsiPht", "F")
        self.out.branch("AngleCM_RecoMu0", "F")
        self.out.branch("AngleCM_RecoMu1", "F")
        self.out.branch("AngleCM_RecoPht", "F")
        self.out.branch("AngleCM_RecoJPsi", "F")

        self.out.branch("DeltaPhiCM_Mu0Mu1", "F")
        self.out.branch("DeltaPhiCM_Mu0Pht", "F")
        self.out.branch("DeltaPhiCM_Mu1Pht", "F")
        self.out.branch("DeltaPhiCM_JpsiPht", "F")
        self.out.branch("DeltaPhiCM_RecoMu0", "F")
        self.out.branch("DeltaPhiCM_RecoMu1", "F")
        self.out.branch("DeltaPhiCM_RecoPht", "F")
        self.out.branch("DeltaPhiCM_RecoJPsi", "F")
                        
        self.out.branch("DeltaRCM_Mu0Mu1", "F")
        self.out.branch("DeltaRCM_Mu0Pht", "F")
        self.out.branch("DeltaRCM_Mu1Pht", "F")
        self.out.branch("DeltaRCM_JpsiPht", "F")
        self.out.branch("DeltaRCM_RecoMu0", "F")
        self.out.branch("DeltaRCM_RecoMu1", "F")
        self.out.branch("DeltaRCM_RecoPht", "F")
        self.out.branch("DeltaRCM_RecoJPsi", "F") 

               
    def endFile(self, inputFile, outputFile, inputTree, wrappedOutputTree):
        pass
    def analyze(self, event):
        """process event, return True (go to next module) or False (fail, go to next event)"""
        #electrons = Collection(event, "Electron")
        if event.nMuon <2:
            return False
        if event.nPhoton <1:
            return False
        if event.Muon_charge[0] == event.Muon_charge[1] :
            return False
        

        #introduce some light cuts to reduce size of skimmed files
        #we have already done this this of cuts in the previous dataset 
        #these will be lighter but will eliminate lot's of not interesting events


        #if ( mumuG.M() < 70  or mumuG.M() > 150 ): return False
        #if (  mumu.M() < 2.8 or  mumu.M() > 3.3 ): return False
        if event.Muon_pt[0]         <  15 : return False
        if event.Muon_pt[1]         <   2 : return False
        if event.Photon_pt[0]       <  25 : return False
        if abs(event.Photon_eta[0]) > 2.9 : return False


        
        
       # Mu = Collection(event, "Muon")
       # Ph = Collection(event, "Photon")
       # mumu  = ROOT.TLorentzVector()
       # mumu  = Mu[0].p4() + Mu[1].p4()
       # mumuG = ROOT.TLorentzVector()
       # mumuG = Mu[0].p4() + Mu[1].p4() + Ph[0].p4()


        Muons = Collection(event, "Muon")
        Ph =Collection(event, "Photon")

        mu0=ROOT.Math.PtEtaPhiMVector( event.Muon_pt[0]   , event.Muon_eta[0]    , 
                                       event.Muon_phi[0]  , event.Muon_mass[0]   )
        mu1=ROOT.Math.PtEtaPhiMVector( event.Muon_pt[1]   , event.Muon_eta[1]    , 
                                       event.Muon_phi[1]  , event.Muon_mass[1]   )
        pht=ROOT.Math.PtEtaPhiMVector( event.Photon_pt[0] , event.Photon_eta[0]  , 
                                       event.Photon_phi[0], event.Photon_mass[0] )

        jpsi=mu0+mu1
        reco=mu0+mu1+pht 

        mu0_cart = ROOT.Math.PxPyPzEVector(mu0.Px(), mu0.Py(), mu0.Pz(), mu0.E())
        mu1_cart = ROOT.Math.PxPyPzEVector(mu1.Px(), mu1.Py(), mu1.Pz(), mu1.E())
        pht_cart = ROOT.Math.PxPyPzEVector(pht.Px(), pht.Py(), pht.Pz(), pht.E())
        jpsi_cart = mu0_cart+mu1_cart
        reco_cart = mu0_cart+mu1_cart+pht_cart

        betaCM  = reco_cart.BoostToCM()
        boostCM = ROOT.Math.Boost(betaCM.X(), betaCM.Y(), betaCM.Z())

        mu0_cartCM  = boostCM(mu0_cart)
        mu1_cartCM  = boostCM(mu1_cart)
        pht_cartCM  = boostCM(pht_cart)
        jpsi_cartCM = boostCM(jpsi_cart)
        reco_cartCM = boostCM(reco_cart)

        
                    
        self.out.fillBranch("JPsi_mass", jpsi.M())
        self.out.fillBranch("JPsi_pt"  , jpsi.Pt())
        self.out.fillBranch("JPsi_eta" , jpsi.Eta())
        self.out.fillBranch("JPsi_phi" , jpsi.Phi())
        self.out.fillBranch("Reco_mass", reco.M())
        self.out.fillBranch("Reco_pt"  , reco.Pt())
        self.out.fillBranch("Reco_eta" , reco.Eta())
        self.out.fillBranch("Reco_phi" , reco.Phi()) 


#we can use vUtil.Angle or vUtil.cosTheta, it the sama
        self.out.fillBranch("AngleCM_Mu0Mu1",   vUtil.CosTheta( mu0_cartCM.Vect() , mu1_cartCM.Vect()))
        self.out.fillBranch("AngleCM_Mu0Pht",   vUtil.CosTheta( mu0_cartCM.Vect() , pht_cartCM.Vect()))
        self.out.fillBranch("AngleCM_Mu1Pht",   vUtil.CosTheta( mu1_cartCM.Vect() , pht_cartCM.Vect()))
        self.out.fillBranch("AngleCM_JpsiPht",  vUtil.CosTheta(jpsi_cartCM.Vect() , pht_cartCM.Vect()))
        self.out.fillBranch("AngleCM_RecoMu0",  vUtil.CosTheta(reco_cartCM.Vect() , mu0_cartCM.Vect()))
        self.out.fillBranch("AngleCM_RecoMu1",  vUtil.CosTheta(reco_cartCM.Vect() , mu1_cartCM.Vect()))
        self.out.fillBranch("AngleCM_RecoPht",  vUtil.CosTheta(reco_cartCM.Vect() , pht_cartCM.Vect()))
        self.out.fillBranch("AngleCM_RecoJPsi", vUtil.CosTheta(reco_cartCM.Vect() ,jpsi_cartCM.Vect()))

        
        self.out.fillBranch("DeltaR_Mu0Mu1",   vUtil.DeltaR( mu0,  mu1 ))
        self.out.fillBranch("DeltaR_Mu0Pht",   vUtil.DeltaR( mu0,  pht ))
        self.out.fillBranch("DeltaR_Mu1Pht",   vUtil.DeltaR( mu1,  pht ))
        self.out.fillBranch("DeltaR_JpsiPht",  vUtil.DeltaR(jpsi,  pht ))
        self.out.fillBranch("DeltaR_RecoMu0",  vUtil.DeltaR(reco,  mu0 ))
        self.out.fillBranch("DeltaR_RecoMu1",  vUtil.DeltaR(reco,  mu1 ))
        self.out.fillBranch("DeltaR_RecoPht",  vUtil.DeltaR(reco,  pht ))
        self.out.fillBranch("DeltaR_RecoJPsi", vUtil.DeltaR(reco, jpsi )) 

        #self.out.fillBranch("DeltaPhiCM_Mu0Mu1",  vUtil.DeltaPhi( mu0_cartCM,  mu1_cartCM ))
        #self.out.fillBranch("DeltaPhiCM_Mu0Pht",  vUtil.DeltaPhi( mu0_cartCM,  pht_cartCM ))
        #self.out.fillBranch("DeltaPhiCM_Mu1Pht",  vUtil.DeltaPhi( mu1_cartCM,  pht_cartCM ))
        #self.out.fillBranch("DeltaPhiCM_JpsiPht", vUtil.DeltaPhi(jpsi_cartCM,  pht_cartCM ))
        #self.out.fillBranch("DeltaPhiCM_RecoMu0", vUtil.DeltaPhi(reco_cartCM,  mu0_cartCM ))
        #self.out.fillBranch("DeltaPhiCM_RecoMu1", vUtil.DeltaPhi(reco_cartCM,  mu1_cartCM ))
        #self.out.fillBranch("DeltaPhiCM_RecoPht", vUtil.DeltaPhi(reco_cartCM,  pht_cartCM ))
        #self.out.fillBranch("DeltaPhiCM_RecoJPsi",vUtil.DeltaPhi(reco_cartCM, jpsi_cartCM ))

        self.out.fillBranch("DeltaPhiCM_Mu0Mu1",   mu0_cartCM.Phi() -  mu1_cartCM.Phi())
        self.out.fillBranch("DeltaPhiCM_Mu0Pht",   mu0_cartCM.Phi() -  pht_cartCM.Phi())
        self.out.fillBranch("DeltaPhiCM_Mu1Pht",   mu1_cartCM.Phi() -  pht_cartCM.Phi())
        self.out.fillBranch("DeltaPhiCM_JpsiPht", jpsi_cartCM.Phi() -  pht_cartCM.Phi())
        self.out.fillBranch("DeltaPhiCM_RecoMu0", reco_cartCM.Phi() -  mu0_cartCM.Phi())
        self.out.fillBranch("DeltaPhiCM_RecoMu1", reco_cartCM.Phi() -  mu1_cartCM.Phi())
        self.out.fillBranch("DeltaPhiCM_RecoPht", reco_cartCM.Phi() -  pht_cartCM.Phi())
        self.out.fillBranch("DeltaPhiCM_RecoJPsi",reco_cartCM.Phi() - jpsi_cartCM.Phi())



        self.out.fillBranch("DeltaRCM_Mu0Mu1",  vUtil.DeltaR( mu0_cartCM,  mu1_cartCM ))
        self.out.fillBranch("DeltaRCM_Mu0Pht",  vUtil.DeltaR( mu0_cartCM,  pht_cartCM ))
        self.out.fillBranch("DeltaRCM_Mu1Pht",  vUtil.DeltaR( mu1_cartCM,  pht_cartCM ))
        self.out.fillBranch("DeltaRCM_JpsiPht", vUtil.DeltaR(jpsi_cartCM,  pht_cartCM ))
        self.out.fillBranch("DeltaRCM_RecoMu0", vUtil.DeltaR(reco_cartCM,  mu0_cartCM ))
        self.out.fillBranch("DeltaRCM_RecoMu1", vUtil.DeltaR(reco_cartCM,  mu1_cartCM ))
        self.out.fillBranch("DeltaRCM_RecoPht", vUtil.DeltaR(reco_cartCM,  pht_cartCM ))
        self.out.fillBranch("DeltaRCM_RecoJPsi",vUtil.DeltaR(reco_cartCM, jpsi_cartCM ))

                

        
        #self.out.fillBranch( "MuMu_invMass",  mumu.M())
        #self.out.fillBranch("MuMuG_invMass", mumuG.M())        

        return True


# define modules using the syntax 'name = lambda : constructor' to avoid having them loaded when not needed

MuMuGModuleConstr = lambda : MuMuGProducer()#jetSelection= lambda j : j.pt >= 0) 
 
