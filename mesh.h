#ifndef MESH_H
#define MESH_H

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>

using std::vector;
using std::max;

struct Face;

struct Vertex
{
    double v[3];
    double n[3];
    double t[2];
    bool has_normal, has_texture;
    size_t index;
    vector<Face*> f;
};

struct Edge
{
    Vertex* v[2];
    Face* f[2];
    size_t index;
};

struct Face
{
    vector<Vertex*> v;
    vector<Edge*> e;
    size_t index;
};

class Mesh
{
public:
    void add(Vertex* v);
    void add(Edge* e);
    void add(Face* f);

    Vertex* get_vertex(int i);
    Edge* get_edge(int i);
    Face* get_face(int i);

    size_t vertex_count() const;
    size_t edge_count() const;
    size_t face_count() const;

private:
    vector<Vertex*> vertices;
    vector<Edge*> edges;
    vector<Face*> faces;
};

Edge* get_edge(Vertex* v0, Vertex* v1);
Edge* create_edge(Vertex* v0, Vertex* v1, Face* f, Mesh& mesh);
void normalize(Mesh& mesh);
void cross_product(double* v, double* u, double* res);
void compute_normals(Mesh& mesh);
void center_on_screen(Mesh& mesh);
#endif
