#!/bin/bash
# $1 = number of batches (one for every mu)
# $2 = number of samples per batch
# $3 = number of bins (for generate function)
# $4 = starting mu
# $5 = ending mu
# $6 = mu step
export LC_NUMERIC="en_US.UTF-8"
AUXFILE="auxFile.sh"
for mu in $(seq $4 $5 $6)
do
echo "Submitting BatchMu $mu"
/bin/cat <<EOM > $AUXFILE
#BSUB -J ToyMC_BatchMu_v02
cd ..
root -l -b -q '../../../macros/MC/v02/ToyMC_Batch_v02.cc('$2', '$3', '$mu');'
EOM
bsub -q "local-cms-long" < $AUXFILE
done
