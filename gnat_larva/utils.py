import numpy as np
from numba import jit
import random


def init_boids(n, method="birds", **kwargs):
    # Generate random particle coordinations
    # particles[i,0] = x
    # particles[i,1] = y
    if method == "birds":
        return np.random.uniform(0, 1, size=(n, 2))
    elif method == "larva":
        try:
            r1 = kwargs["r1"]
            r2 = kwargs["r2"]
        except KeyError:
            raise(KeyError, "Please assign r1 and r2 to the arguments.")
        x, y = get_random_ellipse(n, r1, r2)

        return np.array(list(zip(x, y)))
    else:
        raise NotImplementedError


@jit()
def get_random_ellipse(n, r1, r2):
    xout = np.zeros(n)
    yout = np.zeros(n)
    nkeep = 0
    while nkeep < n:
        x = 2 * r1 * (np.random.random(n - nkeep) - 0.5)
        y = 2 * r2 * (np.random.random(n - nkeep) - 0.5)

        w, = np.where(((x / r1) ** 2 + (y / r2) ** 2) < 1)
        if w.size > 0:
            xout[nkeep:nkeep + w.size] = x[w] + 0.5
            yout[nkeep:nkeep + w.size] = y[w] + 0.5
            nkeep += w.size

    return xout, yout


def get_depth(n, depth=1):
    return random.choices(range(depth), k=n)
