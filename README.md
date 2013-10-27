Mold
====

Mold is an updated version of a program my dad and I wrote well over a decade ago.
The original program took a small square of colors and discretely walked it randomly
around the screen, so that it each pixel had the color of the last time the square left.
This version uses continuous motion, a circular shape, and jittered multisampling.

The license is 2-clause BSD (see the included `LICENSE` file).

For questions or discussion, email Geoffrey Irving <irving@naml.us>.

### Dependency

mold depends on the geode geometry library.  At the moment, geode can only be installed
from source; see the instructions at https://github.com/otherlab/geode.

### Setup

With paths modified appropriately, do

    # Borrow geode's build system
    ~/geode/bin/build-setup

    # Build
    scons

    # Search for libraries in the right place
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/mold/build/native/release/lib

    # Run mold.  E.g.,
    ./mold --sizes 2880,1800 --samples 16 --raw raw-full.npz --mode walk --radius 20 --time 10000000 -o full.png
