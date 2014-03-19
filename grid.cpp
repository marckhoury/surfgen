#include "grid.h"

Grid::Grid(size_t axis[3], double spacing[3])
{
    this->axis[0] = axis[0], this->axis[1] = axis[1], this->axis[2] = axis[2];
    this->spacing[0] = spacing[0], this->spacing[1] = spacing[1], this->spacing[2] = spacing[2];
    offset[0] = -1;
    grid.resize(axis[0]*axis[1]*axis[2]);
}

Grid::Grid(const Grid& rhs)
{
    grid.resize(rhs.grid.size());
    for(int i = 0; i < grid.size(); i++) {
        grid[i] = rhs.grid[i];
    }
    for(int i = 0; i < 8; i++) {
        offset[i] = rhs.offset[i];
    }
    axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
    spacing[0] = rhs.spacing[0], spacing[1] = rhs.spacing[1], spacing[2] = rhs.spacing[2];
}

Grid::~Grid()
{
}

Grid Grid::subsample(int s)
{
    if(s == 1) {
        return *this;
    }
    size_t sub_axis[3] = {(axis[0]+1)/s, (axis[1]+1)/s, (axis[2]+1)/s};
    double sub_spacing[3] = {spacing[0]*s, spacing[1]*s, spacing[2]*s};
    Grid subsampled(sub_axis, sub_spacing);
    subsampled.grid.resize(subsampled.axis[0]*subsampled.axis[1]*subsampled.axis[2]);
	
    for(int z = 0; z < subsampled.axis[2]; z++) {
        for(int y = 0; y < subsampled.axis[1]; y++) {
            for(int x = 0; x < subsampled.axis[0]; x++) {
                subsampled.grid[subsampled.index(x,y,z)] = grid[this->index(s*x,s*y,s*z)];
            }
        }
    }
    return subsampled;
}

int Grid::cube_count()
{
    return (axis[0]-1)*(axis[1]-1)*(axis[2]-1);
}

int Grid::vertex_count()
{
    return axis[0]*axis[1]*axis[2];
}

int Grid::get_axis(int i)
{
    return axis[i];
}

double Grid::get_spacing(int i)
{
    return spacing[i];
}

int Grid::index(int x, int y, int z)
{
    return x + y*axis[0] + z*axis[0]*axis[1];
}

double& Grid::operator[](int i)
{
    return grid[i];
}

bool Grid::is_intersected(int iv, double iso)
{
    int min_sv, max_sv;
    if(offset[0] == -1) {
        compute_offset();
    }
    max_sv = grid[iv+offset[0]];
    min_sv = grid[iv+offset[0]];
    for(int i = 1; i < 8; i++) {
        max_sv = max_sv > grid[iv+offset[i]] ? max_sv : grid[iv+offset[i]];
        min_sv = min_sv < grid[iv+offset[i]] ? min_sv : grid[iv+offset[i]];
    }
    return iso >= min_sv && iso < max_sv;
}

Grid& Grid::operator=(const Grid& rhs)
{
    if(this != &rhs) {
        grid.resize(rhs.grid.size());
        for(int i = 0; i < grid.size(); i++) {
            grid[i] = rhs.grid[i];
        }
        for(int i = 0; i < 8; i++) {
            offset[i] = rhs.offset[i];
        }
        axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
        spacing[0] = rhs.spacing[0], spacing[1] = rhs.spacing[1], spacing[2] = rhs.spacing[2];
    }
    return *this;
}

ostream& operator<<(ostream& os, Grid& g)
{
    for(int z = 0; z < g.axis[2]; z++) {
        for(int y = 0; y < g.axis[1]; y++) {
            for(int x = 0; x < g.axis[0]; x++) {
                os << g[g.index(x,y,z)] << " ";
            }
            os << endl;
        }
        os << endl;
    }
    return os;
}

void Grid::compute_offset()
{
    offset[0] = index(0,0,0);
    offset[1] = index(1,0,0);
    offset[2] = index(0,1,0);
    offset[3] = index(1,1,0);
    offset[4] = index(0,0,1);
    offset[5] = index(1,0,1);
    offset[6] = index(0,1,1);
    offset[7] = index(1,1,1);
}


