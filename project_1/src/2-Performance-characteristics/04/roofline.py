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
    ax = plot_roofline(Pmax=41.6, bmax=18, Imin=1.e-2, Imax=1.e+3, ax=ax,
                       label="EPYC 7H12 core")
    ax = plot_roofline(Pmax=39.2, bmax=25, Imin=1.e-2, Imax=1.e+3, ax=ax,
                       label="EPYC 7763 core")
    ax.legend()
    plt.savefig("roofline.pdf")
    plt.show()
