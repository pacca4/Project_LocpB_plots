#BSUB -J ToyMC_BatchMu_IsoTrk_v05
cd ..
root -l -b -q '../../../macros/MC/v05/ToyMC_BatchMu_IsoTrk_v05.cc('100000', '40', '6000', '500');'
