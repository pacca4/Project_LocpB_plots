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
    
    
def significance( nb, idx, upper=120):
    h1, e1 = np.histogram(data_AllMu[idx], bins=nb, range=(0,upper))
    med1 = np.median(data_AllMu[idx])
    pObs = 1-stats.chi2.cdf(med1, 38)
    sigObs = math.sqrt(2)*erfinv(1-pObs)
    return pObs, sigObs
    
def sub_plot_hists(ax, nb, idx, upper=120):
    h0, e0 = np.histogram(data_AllMu[0]  , bins=nb, range=(0,upper), density=True)
    h1, e1 = np.histogram(data_AllMu[idx], bins=nb, range=(0,upper), density=True)
    med1 = np.median(data_AllMu[idx])
    pObs, sigObs = significance(nb, idx, upper)
    
    ax.fill(np.linspace(0,upper, nb), h0, alpha=0.6)
    ax.fill(np.linspace(0,upper, nb), h1, alpha=0.6)
    ax.axvline(med1, 0.05, 1, c='r', ls='--')
    ax.set_title("Distribution for $\mu=$"+str(mus[idx]))
    ax.set_xlabel("q($\mu$)")
    ax.set_ylabel("Density")
    ax.text(0.85,0.6,   "$p_{obs}$= "+str(round(pObs,3))+ "\n$\sigma_{obs}$=                    "+str(round(sigObs,3)), size=12, ha="center", transform=ax.transAxes)
    
def plot_all_distr_isoTrk(sigs, h_mu):
    fig, axs = plt.subplots(1,2, figsize=(18,6))
    ax0 = axs[0].hist2d(h_mu.flatten(), data_AllMu.flatten(), density=False, range=             [[-2.5, 502.5], [20, 100]], bins=[101, 120], cmap='inferno')
    axs[0].set_xlabel("$\mu$")
    axs[0].set_ylabel("q($\mu$)")
    axs[0].set_title("Distribution dependence on $\mu$")
    fig.colorbar(ax0[3], ax=axs[0])
    axs[1].plot(5*np.linspace(0,100, 101),sigs)
    axs[1].set_xlabel("$\mu$")
    axs[1].set_ylabel("$\sigma_{\mu}$")
    axs[1].set_title("Significance dependence on $\mu$")
    
    
def plot_all_distr_isoMu(sigs, h_mu):
    fig, axs = plt.subplots(1,2, figsize=(18,6))
    ax0 = axs[0].hist2d(h_mu.flatten(), data_AllMu.flatten(), density=False, range=        [[-2.5, 1002.5], [20, 100]], bins=[101, 120], cmap='inferno')
    axs[0].set_xlabel("$\mu$")
    axs[0].set_ylabel("q($\mu$)")
    axs[0].set_title("Distribution dependence on $\mu$")
    fig.colorbar(ax0[3], ax=axs[0])
    axs[1].plot(10*np.linspace(0,100, 101),sigs)
    axs[1].set_xlabel("$\mu$")
    axs[1].set_ylabel("$\sigma_{\mu}$")
    axs[1].set_title("Significance dependence on $\mu$")
    
