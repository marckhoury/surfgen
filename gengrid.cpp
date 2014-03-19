#include "gengrid.h"

double jack(double x, double y, double z)
{
    double a1, a2, a3, a4, a5, a6, a7, a8, a9;

    a1 = x * x / 9.0 + 4.0 * (y * y + z * z);
    a1 = 1.0/(a1 * a1 * a1 * a1);
    a2 = y * y / 9.0 + 4.0 * (x * x + z * z);
    a2 = 1.0/(a2 * a2 * a2 * a2);
    a3 = z * z / 9.0 + 4.0 * (y * y + x * x);
    a3 = 1.0/(a3 * a3 * a3 * a3);
    a4 = 4.0 * x / 3.0 - 4.0;
    a5 = 4.0 * x / 3.0 + 4.0;
    a6 = 16.0 * (y * y + z * z) / 9.0;
    a4 = a4 * a4 + a6;
    a4 = 1.0 / (a4 * a4 * a4 * a4);
    a5 = a5 * a5 + a6;
    a5 = 1.0 / (a5 * a5 * a5 * a5);
    a7 = 4.0 * y / 3.0 - 4.0;
    a8 = 4.0 * y / 3.0 + 4.0;
    a9 = 16.0 * (x * x + z * z) / 9.0;
    a7 = a7 * a7 + a9;
    a7 = 1.0 / (a7 * a7 * a7 * a7);
    a8 = a8 * a8 + a9;
    a8 = 1.0 / (a8 * a8 * a8 * a8);
    a1 = a1 + a2 + a3 + a4 + a5 + a7 + a8;
    return 1.0 - 1.0/sqrt(sqrt(a1));
}

double wiffle_cube(double x, double y, double z)
{
    double a1, a2;
    a1 = 0.25 * (x*x + y*y + z*z);
    a1 = 1.0/ (a1 * a1 * a1 * a1 * a1 * a1);
    a2 = (0.00390625 * (x*x*x*x*x*x*x*x + y*y*y*y*y*y*y*y + z*z*z*z*z*z*z*z));
    a2 = a2 * a2 * a2 * a2 * a2 * a2;
    return 1.0 - a1 - a2;
}

double torus(double x, double y, double z)
{
    double a1, a2;
    a1 = (x*x + y*y + z*z + 9.0 - 1.0);
    a1 = a1 * a1;
    a2 = 36.0*(x*x + y*y);
    return a1 - a2;
}

double sphere(double x, double y, double z)
{
    return x*x + y*y + z*z - 9;
}

double mobius_strip(double x, double y, double z)
{
    return -9*y + x*x*y + y*y*y - 6*x*z - 2*x*x*z - 2*y*y*z + y*z*z; 
}

double klein_bottle(double x, double y, double z)
{
    double a1, a2, a3;
    a1 = x*x + y*y + z*z + 2*y - 1;
    a2 = x*x + y*y + z*z - 2*y - 1;
    a2 = a2 * a2;
    a2 = a2 - 8*z*z;
    a3 = 16*x*z*(x*x + y*y +z*z - 2*y - 1);
    return a1 * a2 + a3; 
}

double hyperboloid_one_sheet(double x, double y, double z)
{
    return x*x/4.0 + y*y/4.0 - z*z/16.0 - 1; 
}

double cayley(double x, double y, double z)
{
    return -5*(x*x*y + x*x*z + y*y*x + y*y*z + z*z*y + z*z*x) + 2*(x*y+ x*z+ y*z);
}

double bohemian_dome(double x, double y, double z)
{
    return -y*y*y*y - x*x*x*x - z*z*z*z + 4*x*x*z*z + 8*y*y - 4*x*x*y*y - 4*y*y*z*z;
}

double chubs(double x, double y, double z)
{
    return x*x*x*x + y*y*y*y + z*z*z*z - x*x - y*y - z*z + 0.5;
}

double quadrifolia(double x, double y, double z)
{
    return x*x*x*x + y*y*y*y + z*z*z*z + 16*x*x*y*y + 16*x*x*z*z 
        + 16*y*y*z*z - 20*y*x*x - 20*y*z*z;
}

double devil(double x, double y, double z)
{
    return x*x*x*x + 2*x*x*z*z - 0.36*x*x - y*y*y*y + 0.25*y*y + z*z*z*z;
}

double quartic_cylinder(double x, double y, double z)
{
    return y*y*x*x + y*y*z*z + 0.5*x*x + 0.5*z*z - 0.5;
}

double gumdrop_torus(double x, double y, double z)
{
    return 4*x*x*x*x + 4*y*y*y*y + 8*y*y*z*z + 4*z*z*z*z 
        + 17*x*x*y*y + 17*x*x*z*z - 20*x*x - 20*y*y - 20*z*z + 17;
}

double barth_sextic(double x, double y, double z)
{
    //Ok this function is just ugly, but it's surface is oh so pretty
    return  67.77708776*x*x*y*y*z*z - 27.41640789*x*x*x*x*y*y - 27.41640789*x*x*z*z*z*z 
        + 10.47213596*x*x*x*x*z*z - 27.41640789*y*y*y*y*z*z + 10.47213596*y*y*y*y*x*x 
        + 10.47213596*y*y*z*z*z*z - 4.236067978*x*x*x*x - 8.472135956*x*x*y*y 
        - 8.472135956*x*x*z*z + 8.472135956*x*x - 4.236067978*y*y*y*y - 8.472135956*y*y*z*z 
        + 8.472135956*y*y - 4.236067978*z*z*z*z + 8.472135956*z*z - 4.236067978;
}

double bohemian_star(double x, double y, double z)
{
    return y*y*y*y*x*x*x*x + 2*y*y*y*y*x*x + 2*y*y*y*y*x*x*z*z 
        + y*y*y*y*y*y*y*y + 2*y*y*y*y*y*y*z*z + y*y*y*y*z*z*z*z 
        - 4*y*y*y*y*x*x - 4*y*y*y*y*z*z - 16*x*x*z*z*y*y + 16*x*x*z*z;
}

double eval(double x, double y, double z, SurfaceType surface_type)
{
    double val;
    switch(surface_type)
    {
        case JACK:
            val = jack(x,y,z);
        break;
        case WIFFLE_CUBE:
            val = wiffle_cube(x,y,z);
        break;
        case TORUS:
            val = torus(x,y,z);
        break;
        case SPHERE:
            val = sphere(x,y,z);
        break;
        case MOBIUS_STRIP:
            val = mobius_strip(x,y,z);
        break;
        case KLEIN_BOTTLE:
            val = klein_bottle(x,y,z);
        break;
        case HYPERBOLOID_ONE_SHEET:
            val = hyperboloid_one_sheet(x,y,z);
        break;
        case CAYLEY:
            val = cayley(x,y,z);
        break;
        case BOHEMIAN_DOME:
            val = bohemian_dome(x,y,z);
        break;
        case CHUBS:
            val = chubs(x,y,z);
        break;
        case QUADRIFOLIA:
            val = quadrifolia(x,y,z);
        break;
        case DEVIL:
            val = devil(x,y,z);
        break;
        case QUARTIC_CYLINDER:
            val = quartic_cylinder(x,y,z);
        break;
        case GUMDROP_TORUS:
            val = gumdrop_torus(x,y,z);
        break;
        case BARTH_SEXTIC:
            val = barth_sextic(x,y,z);
        break;
        case BOHEMIAN_STAR:
            val = bohemian_star(x,y,z);
        break;
        default:
            val = -1;
        break;
    }
    return val;
}

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
