 #!/usr/bin/env python
import os, sys
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
from importlib import import_module
from PhysicsTools.NanoAODTools.postprocessing.framework.postprocessor import PostProcessor
##soon to be deprecated
from PhysicsTools.NanoAODTools.postprocessing.modules.jme.jetmetUncertainties import *
##new way of using jme uncertainty
from PhysicsTools.NanoAODTools.postprocessing.modules.jme.jetmetHelperRun2 import *

from  mumuG_module import *
from  global_paths import *



import optparse
usage = "usage: %prog [options]"
parser = optparse.OptionParser(usage)

parser.add_option("-l", "--list", action="store", type="string", dest="list", default="")
parser.add_option("-i", "--input", action="store", type="string", dest="input", default="")
parser.add_option("-o", "--output", action="store", type="string", dest="output", default=".")
parser.add_option("-f", "--filter", action="store", type="string", dest="filter", default=None)
parser.add_option("-y", '--year', action='store', type=int, dest="year", default=0)
parser.add_option("-n", '--max', action='store', type=long, dest="maxEntries", default=1e9)
parser.add_option("-v", "--verbose", action="store_true", dest="verbose", default=False)
(options, args) = parser.parse_args()



fileList = []
if len(options.input) > 0:
    for f in options.input.split(','):
        if len(f) > 0: fileList += [FILESITE + f]
if len(options.list) > 0:
    with open(options.list, "r") as f:
        for f in f.read().splitlines():
            if len(f) > 0: fileList += [FILESITE + f]



preselection = None
jsonFile = None

if len(fileList) == 0:
    print "- Filelist is empty"
    exit()
elif "Run2016" in fileList[0]: jsonFile = JSONDIR + 'Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt' # 36.773 /fb
elif "Run2017" in fileList[0]: jsonFile = JSONDIR + 'Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt' # 41.86 /fb
elif "Run2018" in fileList[0]: jsonFile = JSONDIR + 'Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt' # 58.83 /fb



#fname=["dcap://t2-srm-02.lnl.infn.it/pnfs/lnl.infn.it/data/cms//store/data/Run2018A/Charmonium/NANOAOD/Nano1June2019-v1/230000/091E682D-3CC5-C54D-A2D7-FEC8A94410FF.root"]
#json2016="filter_json/Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt"
#json2017="filter_json/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt"
#json2018="filter_json/Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt"

#p=PostProcessor(".",fnames,"Jet_pt>150","",[jetmetUncertainties2016(),exampleModuleConstr()],provenance=True)
#p=PostProcessor(".",fnames,"Jet_pt>150","",[jmeCorrections(),exampleModuleConstr()],provenance=True)
#p=PostProcessor(".",fname,"","",[exampleModuleConstr()],provenance=True, 
#                outputbranchsel="keep_and_drop.txt", jsonInput=json2018)

#p=PostProcessor(outputDir=options.output, inputFiles=fileList, cut=preselection, branchsel=None, modules=[SameSign()], jsonInput=jsonFile, histFileName=None, histDirName=None, outputbranchsel=MAINDIR + "keep_and_drop.txt", maxEntries=long(options.maxEntries))
preselection=None
p=PostProcessor(outputDir=OUTDIR, inputFiles=fileList, cut=preselection, 
            branchsel=None, modules=[MuMuGModuleConstr()],#MuMuGProducer ()], ()
            jsonInput=jsonFile, histFileName=None, histDirName=None, 
            outputbranchsel="keep_and_drop.txt", maxEntries=long(options.maxEntries))

p.run()
