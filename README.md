Mold
====

Mold is an updated version of a program my dad and I wrote well over a decade ago.
The original program took a small square of colors and discretely walked it randomly
around the screen, so that it each pixel had the color of the last time the square left.
This version uses continuous motion, a circular shape, and jittered multisampling.

Example:

    ./mold --sizes 2880,1800 --samples 16 --raw raw-full.npz --mode walk --radius 20 --time 10000000 -o full.png
