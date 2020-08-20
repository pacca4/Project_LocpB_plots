#BSUB -J ToyMC_BatchMu_IsoTrk_v06_SingleMuon
#BSUB -o treeAna.log
cd /lustre/cmswork/aavalent/CMSSW_10_2_6/src/PhysicsTools/NanoAODTools/python/postprocessing/SingleMuon_analysis/Project_LocpB_plots/scripts/MC/v06_SingleMuon
root -l -b -q '../../../macros/MC/v06_SingleMuon/ToyMC_BatchMu_IsoTrk_v06.cc('250000', '40', '1000', '400');'
