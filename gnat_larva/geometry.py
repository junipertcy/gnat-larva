# Copy & Pasted from https://github.com/alsignoriello/vicsek_model/
import numpy as np
from math import atan2, pi, sin, cos, sqrt


def vector_2_angle(v):
    x = v[0]
    y = v[1]
    return atan2(y, x)


# generate random angle theta between -pi - pi
def rand_angle(method="birds", **kwargs):
    if method == "birds":
        return np.random.uniform(-pi, pi)
    elif method == "larva":
        try:
            d = kwargs["depth"]
        except KeyError:
            raise(KeyError, "Assign depth to the rand_angle function please.")
        noise = depth2noise(d)
        return np.random.uniform(0 - noise * pi, 0 + noise * pi)


def depth2noise(d):
    a = 0.1
    b = 0.1
    return a * d + b


# returns angle unit vector
def angle_2_vector(theta):
    x = cos(theta)
    y = sin(theta)

    # transform to unit vector
    v1 = np.array([x, y])
    v2 = np.array([0, 0])
    uv = unit_vector(v1, v2)

    return uv


# Unit vector
def unit_vector(v1, v2):
    vector = v1 - v2
    dist = euclidean_distance(v1[0], v1[1], v2[0], v2[1])
    uv = vector / dist
    return uv


# Euclidean distance between (x,y) coordinates
def euclidean_distance(x1, y1, x2, y2):
    return sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2)