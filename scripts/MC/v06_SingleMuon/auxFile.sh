#BSUB -J ToyMC_BatchMu_IsoMu_v05
cd ..
root -l -b -q '../../../macros/MC/v05/ToyMC_BatchMu_IsoMu_v05.cc('100000', '40', '21000', '1000');'
