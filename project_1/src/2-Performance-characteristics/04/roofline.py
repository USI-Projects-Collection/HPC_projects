import matplotlib.pyplot as plt
import numpy as np


def plot_roofline(Pmax, bmax, Imin, Imax, N=1000, ax=None, **plt_kwargs):
    if ax is None:
        ax = plt.gca()
    I = np.logspace(np.log(Imin), np.log(Imax), N)
    P = bmax * I
    P = np.minimum(P, Pmax)
    ax.loglog(I, P, **plt_kwargs)
    ax.set_xscale('log', base=2)
    # ax.set_yscale('log', base=2)
    ax.grid(True)
    ax.set_xlim(Imin, Imax)
    ax.set_xlabel(rf"Operational Intensity $I$ [Flops/Byte]")
    ax.set_ylabel(rf"Performance $P$ [GFlops/s]")
    return ax


if __name__ == "__main__":
    fig, ax = plt.subplots()
    # Rosa single core: Peak = 36.8 GFlops/s, Bandwidth = 12.3 GB/s
    ax = plot_roofline(Pmax=36.8, bmax=12.3, Imin=1.e-2, Imax=1.e+3, ax=ax,
                       label="Intel Xeon E5-2650 v3 (single core)")
    ax.legend()
    plt.savefig("roofline.pdf")
    plt.show()
