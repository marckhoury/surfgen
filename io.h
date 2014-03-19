#ifndef IO_H
#define IO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "mesh.h"

using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::string;

Mesh read_obj(const string& file);
Mesh read_off(const string& file);

void write_obj(const string& file, Mesh& mesh);
void write_off(const string& file, Mesh& mesh);

#endif
