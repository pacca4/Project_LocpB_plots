#!/bin/bash
# $1 = number of batches
# $2 = number of samples per batch
# $3 = number of bins (for generate function)
# $4 = mu
export LC_NUMERIC="en_US.UTF-8"
AUXFILE="auxFile.sh"
for i in {1..$1}
do
echo "Submitting Batch $i"
/bin/cat <<EOM > $AUXFILE
#BSUB -J ToyMC_Batch_v02
cd ..
root -l -b -q '../../../macros/MC/v02/ToyMC_Batch_v02.cc('$i', '$2', '$3', '$4');'
EOM
bsub -q "local-cms-long" < $AUXFILE
done
