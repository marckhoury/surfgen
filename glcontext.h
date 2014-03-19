#ifndef GL_CONTEXT_H
#define GL_CONTEXT_H

class Context
{
public:

    int fovy;
    int width, height;
    int mouse_pos[2];
    bool mouse_button[3];
    double drag[3];
    bool wireframe;
    double window[4];
    double znear, zfar;
    float mat[16]; 
    float matinv[16];
    float light_pos[4];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float global_ambient[4];

    Context()
    {
        fovy = 60;
        width = 600, height = 900;
        mouse_button[0] = mouse_button[1] = mouse_button[2] = false;
        wireframe = false;
        znear = 0.1, zfar = 100;
        light_pos[0] = 0, light_pos[1] = 0, light_pos[2] = -1, light_pos[3] = 1;
        ambient[0] = 0, ambient[1] = 0, ambient[2] = 0, ambient[3] = 1;
        diffuse[0] = 1, diffuse[1] = 1, diffuse[3] = 1, diffuse[3] = 1;
        specular[0] = 1, specular[1] = 1, specular[2] = 1, specular[3] = 1;
        global_ambient[0] = 0.4, global_ambient[1] = 0.4, global_ambient[2] = 0.4, global_ambient[3] = 1;
        for(int i = 0; i < 16; i++) {
            int entry = i == 0 || i == 5 || i == 10 || i == 15;
            mat[i] = entry;
            matinv[i] = entry;
        }
    }
};

#endif
