#!/bin/bash

USAGE="
$(basename "$0") [-h] \$1 \$2 \$3 \$4 \$5 \$6 -- bash script to launch ToyMC_BatchMu_v04 bjobs to LSF

ARGUMENTS:
	\$1 = starting mu
	\$2 = mu step
	\$3 = ending mu
	\$4 = number of samples per batch
	\$5 = number of bins (for generate function)
	\$6 = number of events per sample
"


if [[ "$1" == "-h" ]] ; then
	echo "$USAGE"
fi

if [[ "$1" != "-h" ]] ; then
export LC_NUMERIC="en_US.UTF-8"
AUXFILE="auxFile.sh"
for mu in `seq $1 $2 $3`
do
echo "Submitting BatchMu $mu"
/bin/cat <<EOM > $AUXFILE
#BSUB -J ToyMC_BatchMu_v02
cd ..
root -l -b -q '../../../macros/MC/v04/ToyMC_BatchMu_v04.cc('$4', '$5', '$6', '$mu');'
EOM
bsub -q "local-cms-long" < $AUXFILE
done
fi
