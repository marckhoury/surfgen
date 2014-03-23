#ifndef GRID_H
#define GRID_H

#include <vector>
#include <string>
#include <iostream>

using std::string;
using std::vector;
using std::ostream;
using std::cerr;
using std::endl;

class Grid
{
public:
	Grid(size_t axis[3], double spacing[3]);
	Grid(const Grid& rhs);
	~Grid();
	
	Grid subsample(int s);
	int cube_count();
	int vertex_count();
	int get_axis(int index);
	double get_spacing(int index);
	int index(int x, int y, int z);
	double& operator[](int index);
	bool is_intersected(int iv, double iso);
	Grid& operator=(const Grid& rhs);
	friend ostream& operator<<(ostream& os, Grid& g);

private:
	vector<double> grid;
	size_t axis[3];
	double spacing[3];
	int offset[8];
	
	void compute_offset();
};

#endif


