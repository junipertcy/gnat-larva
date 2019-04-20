# Copy & Pasted from https://github.com/alsignoriello/vicsek_model/
from nb import *
import numpy as np
import sys


# number of particles
N = int(sys.argv[1])

# noise intensity
eta = float(sys.argv[2])

# neighbor radius
r = float(sys.argv[3])

# time step
delta_t = 0.05

# Maximum time
t = 0.
T = 5.

# Generate random particle coordinations
# particles[i,0] = x
# particles[i,1] = y
particles = np.random.uniform(0, 1, size=(N, 2))

# initialize random angles
thetas = np.zeros((N, 1))
for i, theta in enumerate(thetas):
    thetas[i, 0] = rand_angle()

# Currently run until time ends
while t < T:
    # save coordinates & corresponding thetas to text file
    output = np.concatenate((particles, thetas), axis=1)
    np.savetxt("txt/{}.txt".format(round(t, 2)), output)

    for i, (x, y) in enumerate(particles):
        # get neighbor indices for current particle
        neighbors = get_neighbors(particles, r, x, y)

        # get average theta vector
        avg = get_average(thetas, neighbors)

        # get noise vector
        nx = rand_angle()
        ny = rand_angle()
        noise = eta * np.array([nx, ny])

        # move to new position
        particles[i, :] += delta_t * (avg + noise)

        # get new theta
        thetas[i] = vector_2_angle(avg + noise)

        # assure correct boundaries (xmax,ymax) = (1,1)
        if particles[i, 0] < 0:
            particles[i, 0] = 1 + particles[i, 0]

        if particles[i, 0] > 1:
            particles[i, 0] = particles[i, 0] - 1

        if particles[i, 1] < 0:
            particles[i, 1] = 1 + particles[i, 1]

        if particles[i, 1] > 1:
            particles[i, 1] = particles[i, 1] - 1

    # new time step
    t += delta_t

if __name__ == "__main__":
    pass