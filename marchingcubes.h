#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include <unordered_set>
#include "mesh.h"
#include "grid.h"

using std::unordered_set;

Mesh marching_cubes(Grid& grid, double isovalue);

#endif
