# Copy & Pasted from https://github.com/alsignoriello/vicsek_model/
from nb import *
from utils import *
from plot import *
import os
import numpy as np
import sys
import glob
from tqdm import tqdm
from numba import jit

# number of particles
N = int(sys.argv[1])

# noise intensity
eta = float(sys.argv[2])

# neighbor radius
r = float(sys.argv[3])

# method (larva or birds)
m = str(sys.argv[4])

# time step
delta_t = 0.05

# Maximum time
t = 0.
T = 1.

particles = init_boids(N, method=m, r1=0.1, r2=0.02)

if m == "larva":
    depths = get_depth(N, depth=1)
else:
    depths = get_depth(N, depth=0)

# initialize random angles
thetas = np.zeros((N, 1))
for i, theta in enumerate(thetas):
    thetas[i, 0] = rand_angle(method=m, depth=0)

# remove existing files in `txt/` and `img/`
files = glob.glob("txt/*.txt")
for f in files:
    os.remove(f)
files = glob.glob("img/*.png")
for f in files:
    os.remove(f)

# Currently run until time ends
@jit()
def run():
    idx = 0
    for _ in tqdm(np.arange(t, T, delta_t)):
        # save coordinates & corresponding thetas to text file
        output = np.concatenate((particles, thetas), axis=1)
        filename = "txt/{}.txt".format(str(idx).zfill(6))
        np.savetxt(filename, output)

        for agent_i, (x, y) in enumerate(particles):
            # get neighbor indices for current particle
            neighbors = get_neighbors(particles, r, x, y)

            # get average theta vector
            avg = get_average(thetas, neighbors)

            # get noise vector
            nx = rand_angle(method=m, depth=depths[agent_i])
            ny = rand_angle(method=m, depth=depths[agent_i])
            noise = eta * np.array([nx, ny])

            # move to new position
            particles[agent_i, :] += delta_t * (avg + noise)

            # get new theta
            thetas[agent_i] = vector_2_angle(avg + noise)

            # assure correct boundaries (xmax,ymax) = (1,1)
            if particles[agent_i, 0] < 0:
                particles[agent_i, 0] = 1 + particles[agent_i, 0]

            if particles[agent_i, 0] > 1:
                particles[agent_i, 0] = particles[agent_i, 0] - 1

            if particles[agent_i, 1] < 0:
                particles[agent_i, 1] = 1 + particles[agent_i, 1]

            if particles[agent_i, 1] > 1:
                particles[agent_i, 1] = particles[agent_i, 1] - 1

        idx += 1

        # read in txt data
        mat = np.loadtxt(filename)
        coords_ = mat[:, 0:2]
        thetas_ = mat[:, 2]
        plot_vectors(coords_, thetas_, method=m, depths=depths)
        save_plot(filename, eta)


if __name__ == "__main__":
    run()
