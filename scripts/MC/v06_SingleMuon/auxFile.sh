#BSUB -J ToyMC_BatchMu_IsoTrk_v06_SingleMuon
cd ..
root -l -b -q '../../../macros/MC/v06_SingleMuon/ToyMC_BatchMu_IsoTrk_v06.cc('10', '100', '1600', '2');'
