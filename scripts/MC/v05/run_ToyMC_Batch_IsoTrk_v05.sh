#!/bin/bash
# $1 = number of batches
# $2 = number of samples per batch
# $3 = number of bins (for generate function)
# $4 = number of events per sample
# $5 = mu

USAGE="
$(basename "$0") [-h] \$1 \$2 \$3 \$4 \$5 -- bash script to launch ToyMC_Batch_IsoTrk_v05 bjobs to LSF

ARGUMENTS:
	\$1 = number of batches
	\$2 = number of samples per batch
	\$3 = number of bins (for generate function)
	\$4 = number of events per sample
	\$5 = mu value (signal strength)
"


if [[ "$1" == "-h" ]] ; then
	echo "$USAGE"
fi

if [[ "$1" != "-h" ]] ; then
export LC_NUMERIC="en_US.UTF-8"
AUXFILE="auxFile.sh"
for i in {1..$1}
do
echo "Submitting Batch $i"
/bin/cat <<EOM > $AUXFILE
#BSUB -J ToyMC_Batch_IsoTrk_v05
cd ..
root -l -b -q '../../../macros/MC/v05/ToyMC_Batch_IsoTrk_v05.cc('$i', '$2', '$3', '$4', '$5');'
EOM
bsub -q "local-cms-long" < $AUXFILE
done
fi
