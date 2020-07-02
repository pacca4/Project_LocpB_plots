#!/bin/bash
AUXFILE="auxFile.sh"
for i in {1..250}
do
echo "Submitting batch $i"
/bin/cat <<EOM > $AUXFILE
#BSUB -J ToyMC_Batch_v00
root -l -b -q '/lustre/cmswork/ardino/LOCP/LOCP_modB_Project/macros/MC/ToyMC_Batch_v00.cc('$i', 10000, 100);'
EOM
bsub -q "local-cms-long" < $AUXFILE
done
