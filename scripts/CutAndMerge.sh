root -l -b 'mumuG_Cutter.cc("Selected/Partial_Sel0.root","Partial0"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_Sel1.root","Partial1"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_Sel2.root","Partial2"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_Sel3.root","Partial3"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_Sel4.root","Partial4"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_Sel5.root","Partial5"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_Sel6.root","Partial6"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_Sel7.root","Partial7"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_Sel8.root","Partial8"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_Sel9.root","Partial9"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_SelA.root","PartialA"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_SelB.root","PartialB"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_SelC.root","PartialC"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_SelD.root","PartialD"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_SelE.root","PartialE"); exit(0)'
root -l -b 'mumuG_Cutter.cc("Selected/Partial_SelF.root","PartialF"); exit(0)'
mv Cutted_Partial* Cutted/
cd Cutted 
$ROOTSYS/bin/hadd Total_Cutted.root Cutted_*.root
cd ..