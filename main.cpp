#include <iostream>
#include <cstdlib>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "io.h"
#include "mesh.h"
#include "gengrid.h"
#include "mathutil.h"
#include "glcontext.h"
#include "marchingcubes.h"

using namespace std;

size_t resolution[3] = {150, 150, 150};
double spacing[3] = {0.1, 0.1, 0.1};
Grid grid(resolution, spacing);
Mesh mesh;
Context ctx;

void display()
{
    float mat_diffuse[4] = {1,1,0,1};
    float mat_ambient[4] = {0,0,0,1};
    float mat_specular[4] = {0,0,0,1};
    float mat_shininess = 1;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, ctx.wireframe ? GL_LINE : GL_FILL);

    glEnable(GL_LIGHT0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ctx.global_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, ctx.light_pos);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &mat_shininess);

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0, 0, -4);
    glMultMatrixf(ctx.mat);
    for(int i = 0; i < mesh.face_count(); i++) {
        Face* f = mesh.get_face(i);
        glBegin(GL_POLYGON);
        for(int j = 0; j < f->v.size(); j++) {
            Vertex* v = f->v[j];
            glNormal3dv(v->n);
            glTexCoord2dv(v->t);
            glVertex3dv(v->v);  
        }
        glEnd();
    }
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    ctx.window[0] = -static_cast<double>(w)/h;
    ctx.window[1] = -ctx.window[0];
    ctx.window[2] = 1.0;
    ctx.window[3] = -1.0;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(ctx.fovy, static_cast<double>(w)/h, ctx.znear, ctx.zfar);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    SurfaceType surface_type = NO_SURFACE;
    switch(key) {
        case 'a':
            surface_type = JACK;
        break;
        case 's':
            surface_type = WIFFLE_CUBE;
        break;
        case 'd':
            surface_type = TORUS;
        break;
        case 'f':
            surface_type = SPHERE;
        break;
        case 'g':
            surface_type = MOBIUS_STRIP;
        break;
        case 'h':
            surface_type = KLEIN_BOTTLE;
        break;
        case 'j':
            surface_type = HYPERBOLOID_ONE_SHEET;
        break;
        case 'k':
            surface_type = CAYLEY;
        break;
        case 'l':
            surface_type = BOHEMIAN_DOME;
        break;
        case 'z':
            surface_type = CHUBS;
        break;
        case 'x':
            surface_type = QUADRIFOLIA;
        break;
        case 'c':
            surface_type = DEVIL;
        break;
        case 'v':
            surface_type = QUARTIC_CYLINDER;
        break;
        case 'b':
            surface_type = GUMDROP_TORUS;
        break;
        case 'n':
            surface_type = BARTH_SEXTIC;
        break; 
        case 'm':
            surface_type = BOHEMIAN_STAR;
        break;
        case 'w':
            ctx.wireframe = !ctx.wireframe;
        break;
        case 27: //ESC key
            exit(0);
        break;
        default:
            cout << "Unassigned character: " << key << endl;
        break;
    }

    if(surface_type != NO_SURFACE) {
        generate(grid, surface_type);
        mesh = marching_cubes(grid, 0, surface_type);
        center_on_screen(mesh);
    }
}

void world_coords(int x, int y, double* p)
{
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    p[0] = static_cast<double>(x-viewport[0])/viewport[2];
    p[1] = static_cast<double>(y-viewport[1])/viewport[3];

    p[0] = ctx.window[0] + p[0]*(ctx.window[1]-ctx.window[0]);
    p[1] = ctx.window[2] + p[1]*(ctx.window[3]-ctx.window[2]);
    p[2] = ctx.znear;
}

void mouse(int button, int state, int x, int y)
{
    int cursor = GLUT_CURSOR_RIGHT_ARROW;
    if(state == GLUT_DOWN) {
        if(button == GLUT_LEFT_BUTTON) {
            cursor = GLUT_CURSOR_CYCLE;
            ctx.mouse_button[0] = true;
        } else if(button == GLUT_MIDDLE_BUTTON) {
            cursor = GLUT_CURSOR_CROSSHAIR;
            ctx.mouse_button[1] = true;
        } else if(button == GLUT_RIGHT_BUTTON) {
            cursor = GLUT_CURSOR_UP_DOWN;
            ctx.mouse_button[2] = true;
        }
    } else {
        ctx.mouse_button[0] = ctx.mouse_button[1] = ctx.mouse_button[2] = false;
    }
    glutSetCursor(cursor);
    ctx.mouse_pos[0] = x;
    ctx.mouse_pos[1] = y;
    world_coords(x, y, ctx.drag);
}

void motion(int x, int y)
{
    int dx = x - ctx.mouse_pos[0];
    int dy = y - ctx.mouse_pos[1];
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    if(dx == 0 && dy == 0) {
        return;
    } else if(ctx.mouse_button[0]) {
        double angle = sqrt(dy*dy + dx*dx)/static_cast<double>(viewport[2]+1)*180.0;
        double rx = ctx.matinv[0]*dy + ctx.matinv[4]*dx;
        double ry = ctx.matinv[1]*dy + ctx.matinv[5]*dx;
        double rz = ctx.matinv[2]*dy + ctx.matinv[6]*dx;
        glRotatef(angle, rx, ry, rz);
    } else if(ctx.mouse_button[1]) {
        double p[3];
        world_coords(x, y, p);
        glLoadIdentity();
        glTranslatef(p[0] - ctx.drag[0], p[1] - ctx.drag[1], p[2] - ctx.drag[2]);
        glMultMatrixf(ctx.mat);
        ctx.drag[0] = p[0], ctx.drag[1] = p[1], ctx.drag[2] = p[2];
    } else if(ctx.mouse_button[2]) {
        glLoadIdentity();
        glTranslatef(0, 0, dy*0.01);
        glMultMatrixf(ctx.mat);
    }

    ctx.mouse_pos[0] = x;
    ctx.mouse_pos[1] = y;
    
    glGetFloatv(GL_MODELVIEW_MATRIX, ctx.mat);
    inverse(ctx.mat, ctx.matinv);
    glutPostRedisplay();
}

void idle()
{
    glutPostRedisplay();
}

void init()
{
    glClearColor(0.0, 0.71, 1.0, 1.0);
    glShadeModel(GL_SMOOTH);
    
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ctx.ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, ctx.diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, ctx.specular);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(ctx.fovy, 1, ctx.znear, ctx.zfar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void menu()
{
    cout << "\033[1;31m   Surface Generator Menu\033[0m" << endl;
    cout << "\033[1;31mSurface                   Key\033[0m" << endl;
    cout << "Jack                      a" << endl;
    cout << "Wiffle Cube               s" << endl;
    cout << "Torus                     d" << endl;
    cout << "Sphere                    f" << endl;
    cout << "Mobius Strip              g" << endl;
    cout << "Klein Bottle              h" << endl;
    cout << "Hyperboloid of One Sheet  j" << endl;
    cout << "Cayley                    k" << endl;
    cout << "Bohemian Dome             l" << endl;
    cout << "Chubs                     z" << endl;
    cout << "Quadrifolia               x" << endl;
    cout << "Devil                     c" << endl;
    cout << "Quartic Cylinder          v" << endl;
    cout << "Gumdrop Torus             b" << endl;
    cout << "Barth Sextic              n" << endl;
    cout << "Bohemian Star             m" << endl;
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(ctx.width, ctx.height);
    glutCreateWindow("Surface Generator");
    init();
    menu();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}
