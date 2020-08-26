#BSUB -J ToyMC_BatchMu_IsoTrk_v06_SingleMuon
#BSUB -o treeAna.log
cd /lustre/cmswork/ardino/HToJPsiG_ToMuMuG/LOCP_modB_Project/scripts/MC/v06_SingleMuon
root -l -b -q '../../../macros/MC/v06_SingleMuon/ToyMC_BatchMu_SingleMuon_v06.cc('100000', '30', '1000', '300');'
