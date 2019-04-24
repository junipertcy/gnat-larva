# Copy & Pasted from https://github.com/alsignoriello/vicsek_model/
from .geometry import *


# returns a list of indices for all neighbors
# includes itself as a neighbor so it will be included in average
def get_neighbors(particles, r, i, x0, y0, method="birds", **kwargs):
    neighbors = []
    front_nbs = []
    behind_nbs = []

    for j, (x1, y1) in enumerate(particles):
        dist = euclidean_distance(x0, y0, x1, y1)
        if method == "birds":
            if dist < r:
                neighbors.append(j)
        elif method == "larva":
            if dist < r and x0 < x1:
                neighbors.append(j)
                front_nbs.append(j)
            elif dist < r and x0 > x1:
                neighbors.append(j)
                behind_nbs.append(j)
        else:
            raise NotImplementedError

    try:
        f = kwargs["is_moving"]
    except KeyError:
        raise(KeyError, "Please feed in the `is_moving` argument.")

    if method == "larva":
        if len(front_nbs) == 0:
            f[i] = 0
        elif len(behind_nbs) == 0:
            f[i] = 1

        if f[i] == 0:
            return [], 0
    return neighbors, 1


def get_neighbors_3d(particles, r, x0, y0, z0):
    neighbors = []

    for j, (x1, y1, z1) in enumerate(particles):
        dist = euclidean_distance(x0, y0, z0, x1, y1, z1)

        if dist < r:
            neighbors.append(j)

    return neighbors


# average unit vectors for all angles
# return average angle
def get_average(thetas, neighbors):
    n_neighbors = len(neighbors)
    avg_vector = np.zeros(2)

    for index in neighbors:
        theta = thetas[index, 0]
        theta_vec = angle_2_vector(theta)
        avg_vector += theta_vec
    if n_neighbors == 0:
        pass
        # avg_vector = avg_vector + 0.0001
    else:
        avg_vector = avg_vector / n_neighbors

    return avg_vector


def get_average_3d(rand_vecs, neighbors):
    n_neighbors = len(neighbors)
    avg_vector = np.zeros(3)

    for index in neighbors:
        vec = rand_vecs[index]
        avg_vector += vec

    avg_vector = avg_vector / n_neighbors

    return avg_vector
