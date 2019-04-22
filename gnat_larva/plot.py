# Copy & Pasted from https://github.com/alsignoriello/vicsek_model/
import numpy as np
import matplotlib.pyplot as plt
from geometry import angle_2_vector
import glob
import sys
import os
from tqdm import tqdm


def plot_vectors(coords, thetas):
    # generate random color for every particle
    colors = ["b", "g", "y", "m", "c", "pink", "purple", "seagreen",
              "salmon", "orange", "paleturquoise", "midnightblue",
              "crimson", "lavender"]

    for i, (x, y) in enumerate(coords):
        c = colors[i % len(colors)]

        # plot point
        plt.scatter(x, y, color=c, marker=".")

        # plot tail
        theta = thetas[i]
        v = angle_2_vector(theta)
        x1 = x - (0.025 * v[0])
        y1 = y - (0.025 * v[1])
        plt.plot([x, x1], [y, y1], color=c)

    return


def save_plot(file, eta):
    # axes between 0 and 1
    plt.axis([0, 1, 0, 1])

    # remove tick marks
    frame = plt.gca()
    frame.axes.get_xaxis().set_ticks([])
    frame.axes.get_yaxis().set_ticks([])

    # title
    plt.title("$\eta$ = %.3f" % eta)
    # save plot
    plt.savefig("img/%s.png" % file[4:10])

    plt.close()

    # clear for next plot
    plt.cla()

    return


files = glob.glob("img/*.png")
for f in files:
    os.remove(f)

eta = float(sys.argv[1])

for file in tqdm(glob.glob("txt/*.txt")):
    # read in data
    mat = np.loadtxt(file)
    coords = mat[:, 0:2]
    thetas = mat[:, 2]
    plot_vectors(coords, thetas)
    save_plot(file, eta)
