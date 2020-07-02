#!/bin/bash
for i in {1..2}
do
	root -l -b -q '../macros/MC/ToyMC_Batch_v00.cc('$i', 100, 100);'
done
