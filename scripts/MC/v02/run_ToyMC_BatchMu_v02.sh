#!/bin/bash
AUXFILE="auxFile.sh"
for mu in {0..$1}
do
echo "Submitting BatchMu $i"
/bin/cat <<EOM > $AUXFILE
#BSUB -J ToyMC_Batch_v02
cd ..
root -l -b -q '../macros/MC/v02/ToyMC_Batch_v02.cc('$i', '$2', '$3', '$4');'
EOM
bsub -q "local-cms-long" < $AUXFILE
done
