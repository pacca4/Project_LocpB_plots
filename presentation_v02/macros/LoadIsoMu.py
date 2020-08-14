import numpy as np

nsamp = 250000
nbins = 40
nev   = 1600
mus = [2.0*x for x in range(0,201)]

data_AllMu = []
for mu in mus:
    fname = 'ToyMC_BatchMu_IsoMu__nsamples_'+str(nsamp)+'__nbins_'+str(nbins)+'__nev_'+str(nev)+'__mu_'+str(mu)+'.txt'
    data_AllMu.append(np.loadtxt("./mc_samples/ToyMC_BatchMu_IsoMu__nsamples_250000__nbins_40__nev_1600/"+fname))

data_AllMu = np.array(data_AllMu)
