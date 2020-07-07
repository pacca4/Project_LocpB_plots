#BSUB -J ToyMC_BatchMu_v02
cd ..
root -l -b -q '../../../macros/MC/v02/ToyMC_BatchMu_v02.cc('50000', '100', '2.00');'
