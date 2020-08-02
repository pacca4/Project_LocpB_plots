import numpy as np

nsamp = 100000
nbins = 40
nev   = 6000
mus = [5.0*x for x in range(0,101)]

data_AllMu = []
for mu in mus:
    fname = 'ToyMC_BatchMu_IsoTrk__nsamples_'+str(nsamp)+'__nbins_'+str(nbins)+'__nev_'+str(nev)+'__mu_'+str(mu)+'.txt'
    data_AllMu.append(np.loadtxt("../MC_results/ToyMC_BatchMu_IsoTrk__nsamples_100000__nbins_40__nev_6000/"+fname))

data_AllMu = np.array(data_AllMu)