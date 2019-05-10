# Copy & Pasted from https://github.com/alsignoriello/vicsek_model/
import matplotlib.pyplot as plt
from .geometry import angle_2_vector


def plot_vectors(coords, thetas, method="birds", **kwargs):
    # generate random color for every particle
    # [TCY]: I did not know that matplotlib can so colorful!
    if method == "birds":
        colors = ["b", "g", "y", "m", "c", "pink", "purple", "seagreen",
                  "salmon", "orange", "paleturquoise", "midnightblue",
                  "crimson", "lavender"]
        depths = None
    else:
        colors = ["grey", "b"]
        try:
            depths = kwargs["depths"]
        except KeyError:
            raise(KeyError, "You have to feed in the `depths` argument in plot_vectors.")

    for i, (x, y) in enumerate(coords):
        # c = colors[i % len(colors)]
        if method == "birds":
            c = colors[i % len(colors)]
        else:
            c = colors[depths[i]]

        # plot point
        plt.scatter(x, y, color=c, marker=".")

        # plot tail
        theta = thetas[i]
        v = angle_2_vector(theta)
        x1 = x - (0.025 * v[0])
        y1 = y - (0.025 * v[1])
        plt.plot([x, x1], [y, y1], color=c)


def plot_foods(coords):
    for _, (x, y) in enumerate(coords):
        plt.scatter(x, y, color='red', marker=".")



def save_plot(file, eta):
    # axes between 0 and 1
    plt.axis([0, 1, 0, 1])

    # remove tick marks
    frame = plt.gca()
    frame.axes.get_xaxis().set_ticks([])
    frame.axes.get_yaxis().set_ticks([])

    # title
    # plt.title("n = 100; Angular noise = 0.2; Sensible Radius = 0.5 \n But the larva at the top are _more_ and active!")
    plt.title("n = 100; Angular noise = 0.2; Sensible Radius = 0.5 \n Upper larva weigh more (angular noise coupled with depth)")

    # save plot
    plt.savefig("img/%s.png" % file[4:10])

    plt.close()

    # clear for next plot
    plt.cla()

    return


