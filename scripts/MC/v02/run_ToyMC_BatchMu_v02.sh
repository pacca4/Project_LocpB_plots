#!/bin/bash
# $1 = number of samples per batch
# $2 = number of bins (for generate function)
# $3 = starting mu
# $4 = mu step
# $5 = ending mu
export LC_NUMERIC="en_US.UTF-8"
AUXFILE="auxFile.sh"
for mu in `seq $3 $4 $5`
do
echo "Submitting BatchMu $mu"
/bin/cat <<EOM > $AUXFILE
#BSUB -J ToyMC_BatchMu_v02
cd ..
root -l -b -q '../../../macros/MC/v02/ToyMC_Batch_v02.cc('$1', '$2', '$mu');'
EOM
bsub -q "local-cms-long" < $AUXFILE
done
