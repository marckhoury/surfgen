#include "gengrid.h"

void generate(Grid& grid, SurfaceType surface_type)
{
    for(int z = 0; z < grid.get_axis(2); z++) {
        for(int y = 0; y < grid.get_axis(1); y++) {
            for(int x = 0; x < grid.get_axis(0); x++) {
                double xc = (x - grid.get_axis(0)/2.0) * grid.get_spacing(0);
                double yc = (y - grid.get_axis(1)/2.0) * grid.get_spacing(1);
                double zc = (z - grid.get_axis(2)/2.0) * grid.get_spacing(2);
                grid[grid.index(x,y,z)] = eval(xc, yc, zc, surface_type);
            }
        }
    }
}
