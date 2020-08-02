import math
import numpy as np
import matplotlib.pyplot as plt

from scipy.special import erfinv
from scipy import stats

def plot_hists(nb, idx, upper=300):
    h0, e0 = np.histogram(data_AllMu[0]  , bins=nb, range=(0,upper))
    h1, e1 = np.histogram(data_AllMu[idx], bins=nb, range=(0,upper))
    med0 = np.median(data_AllMu[0])
    med1 = np.median(data_AllMu[idx])
    sig0 = math.sqrt(np.var(data_AllMu[0]))
    sig1 = math.sqrt(np.var(data_AllMu[idx]))
    
    plt.plot(np.linspace(0,upper, nb), h0)
    plt.plot(np.linspace(0,upper, nb), h1)
    plt.axvline(med0, c='g', ls='--')
    plt.axvline(med1, c='r', ls='--')
    plt.axvline(med0+2*sig0, c='g', ls='--')
    plt.axvline(med0-2*sig0, c='g', ls='--')
    plt.axvline(med1+2*sig1, c='r', ls='--')
    plt.axvline(med1-2*sig1, c='r', ls='--')
    
    
def significance( nb, idx, upper=120, ndf=38):
    h1, e1 = np.histogram(data_AllMu[idx], bins=nb, range=(0,upper))
    med1 = np.median(data_AllMu[idx])
    pObs = 1-stats.chi2.cdf(med1, ndf)
    sigObs = math.sqrt(2)*erfinv(1-pObs)
    return pObs, sigObs
    
def sub_plot_hists(ax, nb, idx, ndf=38, upper=120):
    h0, e0 = np.histogram(data_AllMu[0]  , bins=nb, range=(0,upper), density=True)
    h1, e1 = np.histogram(data_AllMu[idx], bins=nb, range=(0,upper), density=True)
    med1 = np.median(data_AllMu[idx])
    pObs, sigObs = significance(nb, idx, upper, ndf)
    
    ax.fill(np.linspace(0,upper, nb), h0, alpha=0.6, label="q($\mu=0$) distribution")
    ax.fill(np.linspace(0,upper, nb), h1, alpha=0.6, label="q($\mu="+str(mus[idx])+"$) distribution")
    ax.axvline(med1, 0.05, 1, c='r', ls='--', label="q($\mu="+str(mus[idx])+"$) median")
    ax.set_title("Distribution for $\mu=$"+str(mus[idx]))
    ax.set_xlabel("q($\mu$)")
    ax.set_ylabel("Density")
    ax.text(0.85,0.6,   "$p_{med}$= "+str(round(pObs,3))+ "\n$\sigma_{med}$= "+str(round(sigObs,3)), size=12, ha="center", transform=ax.transAxes)
    ax.legend(fontsize=12, loc='upper right')

def plot_all_distr_isoTrk(sigs, h_mu, plot_arrow=False, bound=2):
    xs = 5*np.linspace(0,100, 101)
    
    if plot_arrow:
        x = 0
        q = 0
        m = 0
        for i in range(len(sigs)-1):
            if sigs[i]<=bound and sigs[i+1]>=bound:
                m = (sigs[i+1] - sigs[i]) / (xs[i+1] - xs[i])
                q = sigs[i]
                x = (2.0 - q) / m + xs[i]
    
    fig, axs = plt.subplots(1,2, figsize=(18,6))
    ax0 = axs[0].hist2d(h_mu.flatten(), data_AllMu.flatten(), density=False, range=        [[-2.5, 502.5], [20, 100]], bins=[101, 120], cmap='inferno')
    axs[0].set_xlabel("$\mu$")
    axs[0].set_ylabel("q($\mu$)")
    axs[0].set_title("Distribution dependence on $\mu$")
    fig.colorbar(ax0[3], ax=axs[0])
    axs[1].plot(xs,sigs, lw=2)
    if plot_arrow: axs[1].arrow(xs[0], 2., x-xs[0], 0,    length_includes_head=True, head_width=0.1,  lw=2, head_length=10,  color="darkorange")
    if plot_arrow: axs[1].arrow(x,     2., 0,      -1.48, length_includes_head=True, head_width=10,   lw=2, head_length=0.1, color="darkorange")
    axs[1].set_xlabel("$\mu$")
    axs[1].set_ylabel("$\sigma_{med}(\mu)$")
    axs[1].set_title("Significance dependence on $\mu$")
    if plot_arrow: return x
    
    
def plot_all_distr_isoMu(sigs, h_mu, plot_arrow=False, bound=2):
    xs = 10*np.linspace(0,100, 101)
    
    if plot_arrow:
        x = 0
        q = 0
        m = 0
        for i in range(len(sigs)-1):
            if sigs[i]<=bound and sigs[i+1]>=bound:
                m = (sigs[i+1] - sigs[i]) / (xs[i+1] - xs[i])
                q = sigs[i]
                x = (2.0 - q) / m + xs[i]
    
    fig, axs = plt.subplots(1,2, figsize=(18,6))
    ax0 = axs[0].hist2d(h_mu.flatten(), data_AllMu.flatten(), density=False, range=        [[-5, 1005], [20, 100]], bins=[101, 120], cmap='inferno')
    axs[0].set_xlabel("$\mu$")
    axs[0].set_ylabel("q($\mu$)")
    axs[0].set_title("Distribution dependence on $\mu$")
    fig.colorbar(ax0[3], ax=axs[0])
    axs[1].plot(xs,sigs, lw=2)
    if plot_arrow: axs[1].arrow(xs[0], 2., x-xs[0], 0,    length_includes_head=True, head_width=0.1,  lw=2, head_length=15,  color="darkorange")
    if plot_arrow: axs[1].arrow(x,     2., 0,      -1.48, length_includes_head=True, head_width=15,   lw=2, head_length=0.1, color="darkorange")
    axs[1].set_xlabel("$\mu$")
    axs[1].set_ylabel("$\sigma_{med}(\mu)$")
    axs[1].set_title("Significance dependence on $\mu$")
    if plot_arrow: return x
    
def plot_all_distr_SingleMuon(sigs, h_mu, plot_arrow=False, bound=2):
    xs = 2*np.linspace(0,200, 201)
    
    if plot_arrow:
        x = 0
        q = 0
        m = 0
        for i in range(len(sigs)-1):
            if sigs[i]<=bound and sigs[i+1]>=bound:
                m = (sigs[i+1] - sigs[i]) / (xs[i+1] - xs[i])
                q = sigs[i]
                x = (2.0 - q) / m + xs[i]
    
    fig, axs = plt.subplots(1,2, figsize=(18,6))
    ax0 = axs[0].hist2d(h_mu.flatten(), data_AllMu.flatten(),  range= [[-1, 401], [20, 100]], bins=(201, 120), cmap='inferno')
    axs[0].set_xlabel("$\mu$")
    axs[0].set_ylabel("q($\mu$)")
    axs[0].set_title("Distribution dependence on $\mu$")
    fig.colorbar(ax0[3], ax=axs[0])
    axs[1].plot(xs,sigs, lw=2)
    if plot_arrow: axs[1].arrow(xs[0], 2., x-xs[0], 0,    length_includes_head=True, head_width=0.1,  lw=2, head_length=15,  color="darkorange")
    if plot_arrow: axs[1].arrow(x,     2., 0,      -1.48, length_includes_head=True, head_width=15,   lw=2, head_length=0.1, color="darkorange")
    axs[1].set_xlabel("$\mu$")
    axs[1].set_ylabel("$\sigma_{med}(\mu)$")
    axs[1].set_title("Significance dependence on $\mu$")
    if plot_arrow: return x