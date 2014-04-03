#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include <unordered_set>
#include "mesh.h"
#include "grid.h"
#include "cutfunc.h"

using std::unordered_set;

Mesh marching_cubes(Grid& grid, double isovalue, SurfaceType surface_type = NO_SURFACE);

#endif
