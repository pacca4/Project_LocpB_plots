import numpy as np

nsamp = 100000
nbins = 20
nev   = 1000
mus = [2.0*x for x in range(0,201)]

data_AllMu = []
for mu in mus:
    fname = 'ToyMC_BatchMu_SingleMuon__nsamples_'+str(nsamp)+'__nbins_'+str(nbins)+'__nev_'+str(nev)+'__mu_'+str(mu)+'.txt'
    data_AllMu.append(np.loadtxt("./mc_samples/ToyMC_BatchMu_SingleMuon__nsamples_"+str(nsamp)+"__nbins_"+str(nbins)+"__nev_"+str(nev)+"/"+fname))

data_AllMu = np.array(data_AllMu)
