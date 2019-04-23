# gnat-larva
CSCI 5423 final project

This program applies a 3D generalization of the Vicsek to simulate the behavior of the collective movements of the gnat larva. Similar to the disorder-to-order phase transition characterizes the 2D Vicsek model, here, we expect to find a phase transition between an unidirectional movement to a bidirectional one.

We reused the codes of the [vicsek_model](https://github.com/alsignoriello/vicsek_model) repository (written by [Alexandra Signoriello](https://cbb.yale.edu/people/alexandra-signoriello) at Yale) on GitHub, and further developed our own 3D alternative.


## Usage

`vicsek` is a command line function. To see what it can do, type:

```commandline
python vicsek.py --help
```

To run the code, do:
```commandline
python vicsek.py --n 100 --eta 0.001 --r 0.05 --m larva
```

This will generate an image sequence in `img/`. To parse them into a video, use `ImageJ`.
