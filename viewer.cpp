/* This system will read input files in .obj format, build scene graphs using them, and render the results. */
/* Display at most 10 objects */
/* arg is folder name in the same directory of the out file */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#ifdef MACINTOSH
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "geom.h"
#include "loader.h"


#define ZOOM_VELOCITY 0.2f
#define ANGLE_VELOCITY 0.5f
#define RADIAN 0.0174532925f

enum Mode { POINT, WIREFRAME, SOLID, SHADED };

// Point mode: Just draw the vertices.
// Wireframe mode: Draw the edges of the mesh.
// Solid mode: Draw filled triangles.
// Shaded mode: Basic lighting, see below.

string dir;
Trimesh t;
TrimeshLoader tl;

int currentX = 0, currentY = 0;

float viewZ = -3.0f, viewX = 0.0f, viewY = 0.0f, viewR = 3;
int enableZoom = 0, enableMove = 0;
float phi = 0, theta = 0;

Mode mode = POINT;

void PointMode() {
    glBegin(GL_POINTS);
    for (vector<Vertex>::const_iterator it = t.v_list.begin(); it != t.v_list.end(); ++it){
        glVertex3f((*it).x, (*it).y, (*it).z);
    }
    glEnd();
}

void WireframeMode() {
    glBegin(GL_LINES);
    for (vector<Face>::const_iterator it = t.f_list.begin(); it != t.f_list.end(); ++it){
        glVertex3f(t.v_list[(*it).u].x, t.v_list[(*it).u].y, t.v_list[(*it).u].z);
        glVertex3f(t.v_list[(*it).v].x, t.v_list[(*it).v].y, t.v_list[(*it).v].z);

        glVertex3f(t.v_list[(*it).v].x, t.v_list[(*it).v].y, t.v_list[(*it).v].z);
        glVertex3f(t.v_list[(*it).w].x, t.v_list[(*it).w].y, t.v_list[(*it).w].z);

        glVertex3f(t.v_list[(*it).w].x, t.v_list[(*it).w].y, t.v_list[(*it).w].z);
        glVertex3f(t.v_list[(*it).u].x, t.v_list[(*it).u].y, t.v_list[(*it).u].z);


    }
    glEnd();
}

void display() {


    viewX = viewR * cosf(phi*RADIAN) * sinf(theta*RADIAN);
    viewY = viewR * sinf(phi*RADIAN) * sinf(theta*RADIAN);
    viewZ = viewR * cosf(theta*RADIAN);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    gluLookAt(viewX, viewY, viewZ, 0, 0, 0, 0, 1, 0);

    glTranslatef(-(t.maxx + t.minx)/2.0f, -(t.maxy + t.miny)/2.0f, -(t.maxz + t.minz)/2.0f);

    switch(mode){
        case POINT: PointMode(); break;
        case WIREFRAME: WireframeMode(); break;
        case SOLID: break;
        case SHADED: break;
    } 

    glutSwapBuffers();
}

void reshape(int width, int height) {
      // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;

   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);

   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void MouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // enableZoom = 1; // For Right Click
        enableMove = 1; // For Left Click
    }
    currentX = x;
    currentY = y;
}

void MouseMove(int x, int y) {
    if(enableZoom)
        viewR -= (currentY < y) ? ZOOM_VELOCITY : -ZOOM_VELOCITY;

    if(enableMove){
        phi += (currentY -y ) * ANGLE_VELOCITY;
        theta += (currentX -x) * ANGLE_VELOCITY;

        phi = fmod(phi, 360.0);
        theta = fmod(theta, 360.0);

    }

    currentX = x;
    currentY = y;
    glutPostRedisplay();
}

void KeyboardFunc(unsigned char Key, int x, int y)
{
    switch(Key)
    {
        case '1': mode = POINT; glutPostRedisplay(); break;
        case '2': mode = WIREFRAME; glutPostRedisplay(); break;
        case '-': viewR-=ZOOM_VELOCITY; glutPostRedisplay(); break;
        case '=': viewR+=ZOOM_VELOCITY; glutPostRedisplay(); break;

    }
}

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        if(string(dirp->d_name).find(".obj") < 100)
            files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}


void SelectFromMenu(int idCommand) {

    t.v_list.clear();
    t.f_list.clear();
    t.minx = t.miny = t.minz = t.maxx = t.maxy = t.maxz = 0;

    vector<string> files = vector<string>();
    getdir(dir,files);

    switch (idCommand)
    {
        case 0:
        tl.loadOBJ(string(dir+"/"+files[0]).c_str(), &t);
        break;
        case 1:
        tl.loadOBJ(string(dir+"/"+files[1]).c_str(), &t);
        break;
        case 2:
        tl.loadOBJ(string(dir+"/"+files[2]).c_str(), &t);
        break;
        case 3:
        tl.loadOBJ(string(dir+"/"+files[3]).c_str(), &t);
        break;
        case 4:
        tl.loadOBJ(string(dir+"/"+files[4]).c_str(), &t);
        break;
        case 5:
        tl.loadOBJ(string(dir+"/"+files[5]).c_str(), &t);
        break;
        case 6:
        tl.loadOBJ(string(dir+"/"+files[6]).c_str(), &t);
        break;
        case 7:
        tl.loadOBJ(string(dir+"/"+files[7]).c_str(), &t);
        break;
        case 8:
        tl.loadOBJ(string(dir+"/"+files[8]).c_str(), &t);
        break;
        case 9:
        tl.loadOBJ(string(dir+"/"+files[9]).c_str(), &t);
        break;
        case 100:
        exit (0);
        break;
    }
    // Almost any menu selection requires a redraw
    glutPostRedisplay();
}

int BuildPopupMenu (void) {
    int menu, submenu;
    vector<string> files = vector<string>();

    submenu = glutCreateMenu (SelectFromMenu);
    getdir(dir,files);

    int options = files.size();
    for (unsigned int i = 0; i < options; i++) {
        string s = files[i].substr(0, files[i].length()-4);
        glutAddMenuEntry (s.c_str(), i);
    }

    menu = glutCreateMenu (SelectFromMenu);
    glutAddSubMenu("Display", submenu);

    glutAddMenuEntry ("Exit", 100);

    return submenu;
}

int main(int argc, char **argv)
{
    dir = string(argv[1]);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("3D Model Viewer");

    glutKeyboardFunc(KeyboardFunc);

    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMove);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // glutIdleFunc(display);
    glEnable(GL_DEPTH_TEST);
    // glutTimerFunc(0, timer, 0);
    BuildPopupMenu ();
    glutAttachMenu (GLUT_RIGHT_BUTTON);
    glClearColor(0.0,0.0,0.0,1.0);
    glutMainLoop();
}








