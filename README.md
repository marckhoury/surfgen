surfgen
=======

surfgen is an implicit surface reconstruction program. It uses the marching cubes algorithm to reconstruct surfaces from a dozen implicit functions for various surfaces. Some of the beautiful surfaces that surfgen can create are shown below.

![ScreenShot](https://raw.github.com/marckhoury/surfgen/master/images/surfaces.png)

Interaction
===========
surfgen implements a standard trackball interface for interaction. The left, middle, and right buttons of the mouse map to rotation, translation, and zooming respectively. The keys of the lower two rows of the keyboard (a-l and z-m) are mapped to various models that surfgen can generate. The table below details all the surfaces that surfgen can construct (also available upon startup).

Surface                 |Key
------------------------|----
Jack                    |a
Wiffle Cube             |s
Torus                   |d
Sphere                  |f
Mobius Strip            |g
Klein Bottle            |h
Hyperboloid of One Sheet|j
Cayley                  |k
Bohemian Dome           |l
Chubs                   |z
Quadrifolia             |x
Devil                   |c
Quartic Cylinder        |v
Gumdrop Torus           |b
Barth Sextic            |n
Bohemian Star           |m


Build
=====
surfgen uses OpenGL, GLU, and GLUT for rendering and some features from the C++11 standard. The code has been built on both Ubuntu Linux and MacOS systems using g++ and clang++ respectively. Makefiles are included for both platforms. To build on a Linux system, simply type 'make' on the command line. Mac users need to type 'make -f Makefile.apple' to ensure the correct makefile is used.
