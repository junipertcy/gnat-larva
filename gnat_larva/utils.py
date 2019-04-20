import numpy as np


def init_boids(n, method="birds"):
    # Generate random particle coordinations
    # particles[i,0] = x
    # particles[i,1] = y
    if method == "birds":
        return np.random.uniform(0, 1, size=(n, 2))
    elif method == "larva":
        return
    else:
        raise NotImplementedError
