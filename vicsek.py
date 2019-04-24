# Copy & Pasted from https://github.com/alsignoriello/vicsek_model/
from gnat_larva.nb import *
from gnat_larva.utils import *
from gnat_larva.plot import *
import os
import numpy as np
import glob
from tqdm import tqdm

import click


@click.command()
@click.option('--n', default=100, type=int, help='number of agents')
@click.option('--eta', default=0.01, type=float, help='noise intensity')
@click.option('--r', default=0.1, type=float, help='neighbor radius')
@click.option('--m', default='birds', type=str, help='method (larva or birds)')
def run(n, eta, r, m):
    # time step
    delta_t = 0.01

    # Maximum time
    t = 0.
    T = 1.

    particles = init_boids(n, method=m, r1=0.1, r2=0.02)

    if m == "larva":
        depths = get_depth(n, depth=1)
        # is_moving = np.ones(n)
        is_moving = get_depth(n, depth=1)
    else:
        depths = get_depth(n, depth=0)
        is_moving = None

    # initialize random angles
    thetas = np.zeros((n, 1))
    for i, theta in enumerate(thetas):
        thetas[i, 0] = rand_angle(method=m, depth=0)

    # remove existing files in `txt/` and `img/`
    files = glob.glob("txt/*.txt")
    for f in files:
        os.remove(f)
    files = glob.glob("img/*.png")
    for f in files:
        os.remove(f)

    idx = 0
    for _ in tqdm(np.arange(t, T, delta_t)):
        # save coordinates & corresponding thetas to text file
        output = np.concatenate((particles, thetas), axis=1)
        filename = "txt/{}.txt".format(str(idx).zfill(6))
        np.savetxt(filename, output)

        for agent_i, (x, y) in enumerate(particles):
            # get neighbor indices for current particle
            neighbors, f_ = get_neighbors(particles, r, agent_i, x, y, method=m, is_moving=is_moving)
            if m == "larva":
                if f_ == 0:
                    is_moving[agent_i] = 0
                else:
                    is_moving[agent_i] = 1

            # get average theta vector
            avg = get_average(thetas, neighbors)

            # get noise vector
            nx = rand_angle(method=m, depth=depths[agent_i])
            ny = rand_angle(method=m, depth=depths[agent_i])
            noise = eta * np.array([nx, ny])

            # move to new position and get the new theta
            if avg[0] == 0 and avg[1] == 0:
                particles[agent_i, :] += delta_t * 0
                thetas[agent_i] = vector_2_angle([0, 0])
            else:
                particles[agent_i, :] += delta_t * (avg + noise)
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
