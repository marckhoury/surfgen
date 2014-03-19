#include "io.h"

enum ObjFaceType {VERTEX, VERTEX_TEXTURE, VERTEX_NORMAL, VERTEX_TEXTURE_NORMAL};

void fatal_error(const string& err, int code = 1)
{
    //escape codes for red text (probably isn't portable)
    cerr << "\033[1;31mFatal Error:\033[0m " << err << endl;
    exit(code);
}

bool get_next_line(ifstream& fin, string& line)
{
    bool not_eof = static_cast<bool>(getline(fin, line));
    while(not_eof && (line.length() == 0 || line[0] == '#')) {
        not_eof = static_cast<bool>(getline(fin, line));
    }
    return not_eof;
}

ObjFaceType get_face_type(const string& ids)
{
    size_t slash_1 = ids.find('/');
    size_t slash_2 = ids.find('/', slash_1 + 1);
    if(slash_1 == string::npos) {
        return VERTEX;
    } else if(slash_2 == string::npos) {
        return VERTEX_TEXTURE;
    } else if(slash_1 + 1 == slash_2) {
        return VERTEX_NORMAL;
    } else {
        return VERTEX_TEXTURE_NORMAL;
    }
}

Mesh read_obj(const string& file)
{
    ifstream fin(file.c_str());
    stringstream ss;
    Mesh mesh;
    int norm_index = 0, tex_index = 0;
    vector<double> normals;
    vector<double> texcoords;
    string line;

    while(get_next_line(fin, line)) {
        string label;
        ss.clear(); ss.str(""); ss << line;

        if(line[0] == 'v') {
            if(line[1] == 'n') {
                double n1, n2, n3;
                ss >> label >> n1 >> n2 >> n3;
                normals.push_back(n1); 
                normals.push_back(n2); 
                normals.push_back(n3);
            } else if(line[1] == 't') {
                double t1, t2;
                ss >> label >> t1 >> t2;
                texcoords.push_back(t1);
                texcoords.push_back(t2);
            } else {
                Vertex* v = new Vertex;
                ss >> label >> v->v[0] >> v->v[1] >> v->v[2];
                v->has_normal = v->has_texture = false;
                v->n[0] = v->n[1] = v->n[2] = 0;
                v->t[0] = v->t[1] = 0;
                mesh.add(v);
            }
        } else if(line[0] == 'f') {
            Face* f = new Face;
            Vertex *v0 = NULL, *v1;
            ss >> label;
            while(ss >> label) {
                int vid = 0, tid = 0, nid = 0;
                ObjFaceType face_type = get_face_type(label);
                if(face_type == VERTEX) {
                    sscanf(label.c_str(), "%d", &vid);
                } else if(face_type == VERTEX_TEXTURE) {
                    sscanf(label.c_str(), "%d/%d", &vid, &tid);    
                } else if(face_type == VERTEX_NORMAL) {
                    sscanf(label.c_str(), "%d//%d", &vid, &nid);
                } else {
                    sscanf(label.c_str(), "%d/%d/%d", &vid, &tid, &nid);
                }
                vid--; tid--; nid--;
                v1 = mesh.get_vertex(vid);
                if(tid >= 0) {
                    v1->t[0] = texcoords[2*tid];
                    v1->t[1] = texcoords[2*tid+1];
                    v1->has_texture = true;
                }
                if(nid >= 0) {
                    v1->n[0] += normals[3*nid];
                    v1->n[1] += normals[3*nid+1];
                    v1->n[2] += normals[3*nid+2];
                    v1->has_normal = true;
                }
                f->v.push_back(v1);
                v1->f.push_back(f);
                if(v0 != NULL) {
                    Edge* e = create_edge(v0, v1, f, mesh);
                    f->e.push_back(e);
                }
                v0 = v1;
            }
            v1 = f->v[0];
            Edge* e = create_edge(v0, v1, f, mesh);
            f->e.push_back(e);
            mesh.add(f);
        }
    }
    if(normals.size() == 0) {
        compute_normals(mesh);
    } else {
        normalize(mesh);
    }
    return mesh;
}

Mesh read_off(const string& file)
{
    ifstream fin(file.c_str());
    stringstream ss;
    Mesh mesh;
    string line;
    int vertex_count, face_count, edge_count;

    getline(fin, line);
    if(line != "OFF") {
        fatal_error("Input OFF file not formatted properly.");
    }
    get_next_line(fin,line);
    ss << line;
    ss >> vertex_count >> face_count >> edge_count;
    for(int i = 0; i < vertex_count; i++) {
        Vertex* v = new Vertex;
        get_next_line(fin, line);
        ss.clear(); ss.str(""); ss << line; 
        ss >> v->v[0] >> v->v[1] >> v->v[2];
        v->has_normal = v->has_texture = false;
        v->n[0] = v->n[1] = v->n[2] = 0;
        v->t[0] = v->t[1] = 0;
        mesh.add(v);
    }
    for(int i = 0; i < face_count; i++) {
        Face* f = new Face;
        int dim, vid;
        Vertex *v0, *v1;

        get_next_line(fin, line);
        ss.clear(); ss.str(""); ss << line;
        ss >> dim >> vid;
        v0 = mesh.get_vertex(vid);
        f->v.push_back(v0);
        v0->f.push_back(f);
        for(int j = 1; j < dim; j++) {
            ss >> vid;
            v1 = mesh.get_vertex(vid);
            f->v.push_back(v1);
            v1->f.push_back(f);
            Edge* e = create_edge(v0, v1, f, mesh);
            f->e.push_back(e);
            v0 = v1;
        }
        v1 = f->v[0];
        Edge* e = create_edge(v0, v1, f, mesh);
        f->e.push_back(e);
        mesh.add(f);
    }
    compute_normals(mesh);
    return mesh;
}

void write_obj(const string& file, Mesh& mesh)
{
    ofstream fout(file.c_str());
    size_t normal_count = 0, texture_count = 0;
    vector<size_t> normal_map(mesh.vertex_count(), -1), texture_map(mesh.vertex_count(), -1);

    for(int i = 0; i < mesh.vertex_count(); i++) {
        Vertex* v = mesh.get_vertex(i);
        fout << "v " << v->v[0] << " " << v->v[1] << " " << v->v[2] << endl;
    }
    for(int i = 0; i < mesh.vertex_count(); i++) {
        Vertex* v = mesh.get_vertex(i);
        if(v->has_normal) {
            fout << "vn " << v->n[0] << " " << v->n[1] << " " << v->n[2] << endl;
            normal_map[i] = normal_count++;
        }
    }
    for(int i = 0; i < mesh.vertex_count(); i++) {
        Vertex* v = mesh.get_vertex(i);
        if(v->has_texture) {
            fout << "vt " << v->t[0] << " " << v->t[1] << endl;
            texture_map[i] = texture_count++;
        }
    }
    for(int i = 0; i < mesh.face_count(); i++) {
        Face* f = mesh.get_face(i);
        fout << "f";
        for(int j = 0; j < f->v.size(); j++) {
            Vertex* v = f->v[j];
            size_t vid = v->index, nid = normal_map[vid], tid = texture_map[vid];
            if(v->has_texture && v->has_normal) {
                fout << " " << vid+1 << "/" << tid+1 << "/" << nid+1;
            } else if(v->has_texture) {
                fout << " " << vid+1 << "/" << tid+1;
            } else if(v->has_normal) {
                fout << " " << vid+1 << "//" << nid+1;
            } else {
                fout << " " << vid+1;
            }
        }
        fout << endl;
    }
    fout.close();
}

void write_off(const string& file, Mesh& mesh)
{
    ofstream fout(file.c_str());

    fout << "OFF" << endl;
    fout << mesh.vertex_count() << " " << mesh.face_count() << " " << mesh.edge_count() << endl;

    for(int i = 0; i < mesh.vertex_count(); i++) {
        Vertex* v = mesh.get_vertex(i);
        fout << v->v[0] << " " << v->v[1] << " " << v->v[2] << endl;
    }
    for(int i = 0; i < mesh.face_count(); i++) {
        Face* f = mesh.get_face(i);
        fout << f->v.size() << " ";
        for(int j = 0; j < f->v.size()-1; j++) {
            fout << f->v[j]->index << " ";
        }
        fout << f->v[f->v.size()-1]->index << endl;
    }
    fout.close();
}
