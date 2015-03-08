// /* This system will read input files in .obj format, build scene graphs using them, and render the results. */
// /* Display at most 10 objects */
// /* arg is folder name in the same directory of the out file */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <math.h>
#include <iostream>

#ifdef MACINTOSH
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "loader.h"
#include "BaseNode.h"


// #define ZOOM_VELOCITY 0.2f
// #define ANGLE_VELOCITY 0.5f
// #define RADIAN 0.0174532925f

// enum Mode { POINT, WIREFRAME, SOLID, SHADED };

// string dir;
// Trimesh t;
// TrimeshLoader tl;

// int currentX = 0, currentY = 0;

// float viewZ = -3.0f, viewX = 0.0f, viewY = 0.0f, viewR = 3;
// int enableZoom = 0, enableMove = 0;
// float phi = 0, theta = 0;

// Mode mode = POINT;

// void PointMode() {
//     glBegin(GL_POINTS);
//     for (vector<Vertex>::const_iterator it = t.v_list.begin(); it != t.v_list.end(); ++it){
//         glVertex3f((*it).x, (*it).y, (*it).z);
//     }
//     glEnd();
// }

// void WireframeMode() {
//     glBegin(GL_LINES);
//     for (vector<Face>::const_iterator it = t.f_list.begin(); it != t.f_list.end(); ++it){
//         glVertex3f(t.v_list[(*it).u].x, t.v_list[(*it).u].y, t.v_list[(*it).u].z);
//         glVertex3f(t.v_list[(*it).v].x, t.v_list[(*it).v].y, t.v_list[(*it).v].z);

//         glVertex3f(t.v_list[(*it).v].x, t.v_list[(*it).v].y, t.v_list[(*it).v].z);
//         glVertex3f(t.v_list[(*it).w].x, t.v_list[(*it).w].y, t.v_list[(*it).w].z);

//         glVertex3f(t.v_list[(*it).w].x, t.v_list[(*it).w].y, t.v_list[(*it).w].z);
//         glVertex3f(t.v_list[(*it).u].x, t.v_list[(*it).u].y, t.v_list[(*it).u].z);
//     }
//     glEnd();
// }

// void SolidMode() {
//     glBegin(GL_TRIANGLES);
//     for (vector<Face>::const_iterator it = t.f_list.begin(); it != t.f_list.end(); ++it){
//         glVertex3f(t.v_list[(*it).u].x, t.v_list[(*it).u].y, t.v_list[(*it).u].z);
//         glVertex3f(t.v_list[(*it).v].x, t.v_list[(*it).v].y, t.v_list[(*it).v].z);
//         glVertex3f(t.v_list[(*it).w].x, t.v_list[(*it).w].y, t.v_list[(*it).w].z);
//     }
//     glEnd();
// }

// void ShadedMode() {

//     float lightDiffuse[] = {1.0f, 1.0f, 0.0f, 1.0f};    //yellow diffuse : color where light hit directly the object's surface
//     float lightPosition[] = {20.0f, 0.0f, 0.0f, 1.0f};

//     float lightDiffuse_2[] = {0.149, 0.694, 0.988, 1.0f};    //yellow diffuse : color where light hit directly the object's surface
//     float lightPosition_2[] = {-20.0f, 0.0f, 0.0f, 1.0f};

//     glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
//     glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

//     glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse_2);
//     glLightfv(GL_LIGHT1, GL_POSITION, lightPosition_2);


//     //Enable the first light and the lighting mode
//     glEnable(GL_LIGHT0);
//     glEnable(GL_LIGHT1);

//     glEnable(GL_COLOR_MATERIAL);
//     glEnable(GL_LIGHTING);

//     glBegin(GL_TRIANGLES);
//     for (vector<Face>::const_iterator it = t.f_list.begin(); it != t.f_list.end(); ++it){

//         // cout << (*it).normal.vx << " "<< (*it).normal.vy << " "<< (*it).normal.vz << endl;
//         glNormal3f((*it).normal.vx, (*it).normal.vy, (*it).normal.vz);
//         glVertex3f(t.v_list[(*it).u].x, t.v_list[(*it).u].y, t.v_list[(*it).u].z);
//         glVertex3f(t.v_list[(*it).v].x, t.v_list[(*it).v].y, t.v_list[(*it).v].z);
//         glVertex3f(t.v_list[(*it).w].x, t.v_list[(*it).w].y, t.v_list[(*it).w].z);
//     }
//     glEnd();

//     glDisable(GL_LIGHTING);
// }

// void display() {
//     viewX = (t.maxx + t.minx) / 2.0f + viewR * cosf(phi*RADIAN) * sinf(theta*RADIAN);
//     viewY = (t.maxy + t.miny) / 2.0f + viewR * sinf(phi*RADIAN) * sinf(theta*RADIAN);
//     viewZ = (t.maxz + t.minz) / 2.0f + viewR * cosf(theta*RADIAN);

//     float length = sqrt (viewX * viewX  +
//                                  viewY * viewY  +
//                                   viewZ  * viewZ );

//     Vector s = {0, 1, 0};
//     Vector p = {viewX/length, viewY/length, viewZ/length};

//     Vector r = {
//         s.vy * p.vz - s.vz * p.vy,
//         s.vz * p.vx - s.vx * p.vz,
//         s.vx * p.vy - s.vy * p.vx
//     };

//     Vector up = {
//         s.vy * r.vz - s.vz * r.vy,
//         s.vz * r.vx - s.vx * r.vz,
//         s.vx * r.vy - s.vy * r.vx
//     };

//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//     glMatrixMode(GL_MODELVIEW);

//     glLoadIdentity();
//     gluLookAt(viewX, viewY, viewZ, 0, 0, 0, up.vx, up.vy, up.vz);

//     cout << up.vx << " " << up.vy << " " << up.vz << " "<< endl;

//     glutSolidCube(0.5f);

//     glTranslatef(-(t.maxx + t.minx)/2.0f, -(t.maxy + t.miny)/2.0f, -(t.maxz + t.minz)/2.0f);

//     switch(mode){
//         case POINT: PointMode(); break;
//         case WIREFRAME: WireframeMode(); break;
//         case SOLID: SolidMode(); break;
//         case SHADED: ShadedMode(); break;
//     } 

//     glutSwapBuffers();
// }

// void reshape(int width, int height) {
//       // Compute aspect ratio of the new window
//    if (height == 0) height = 1;                // To prevent divide by 0
//    GLfloat aspect = (GLfloat)width / (GLfloat)height;

//    // Set the viewport to cover the new window
//    glViewport(0, 0, width, height);

//    // Set the aspect ratio of the clipping volume to match the viewport
//    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
//    glLoadIdentity();             // Reset
//    // Enable perspective projection with fovy, aspect, zNear and zFar
//    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
// }

// void MouseButton(int button, int state, int x, int y) {
//     if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
//         // enableZoom = 1; // For Right Click
//         enableMove = 1; // For Left Click
//     }
//     currentX = x;
//     currentY = y;
// }

// void MouseMove(int x, int y) {
//     if(enableZoom)
//         viewR -= (currentY < y) ? ZOOM_VELOCITY : -ZOOM_VELOCITY;

//     if(enableMove){
//         phi += (currentY - y) * ANGLE_VELOCITY;
//         theta += (currentX - x) * ANGLE_VELOCITY;

//         phi = fmod(phi, 180.0);
//         theta = fmod(theta, 360.0);

//     }

//     currentX = x;
//     currentY = y;
//     glutPostRedisplay();
// }


// int getdir (string dir, vector<string> &files)
// {
//     DIR *dp;
//     struct dirent *dirp;
//     if((dp  = opendir(dir.c_str())) == NULL) {
//         cout << "Error(" << errno << ") opening " << dir << endl;
//         return errno;
//     }

//     while ((dirp = readdir(dp)) != NULL) {
//         if(string(dirp->d_name).find(".obj") < 100)
//             files.push_back(string(dirp->d_name));
//     }
//     closedir(dp);
//     return 0;
// }


// void SelectFromMenu(int idCommand) {

//     t.v_list.clear();
//     t.f_list.clear();
//     t.minx = t.miny = t.minz = t.maxx = t.maxy = t.maxz = 0;

//     vector<string> files = vector<string>();
//     getdir(dir,files);

//     switch (idCommand)
//     {
//         case 0:
//         tl.loadOBJ(string(dir+"/"+files[0]).c_str(), &t);
//         break;
//         case 1:
//         tl.loadOBJ(string(dir+"/"+files[1]).c_str(), &t);
//         break;
//         case 2:
//         tl.loadOBJ(string(dir+"/"+files[2]).c_str(), &t);
//         break;
//         case 3:
//         tl.loadOBJ(string(dir+"/"+files[3]).c_str(), &t);
//         break;
//         case 4:
//         tl.loadOBJ(string(dir+"/"+files[4]).c_str(), &t);
//         break;
//         case 5:
//         tl.loadOBJ(string(dir+"/"+files[5]).c_str(), &t);
//         break;
//         case 6:
//         tl.loadOBJ(string(dir+"/"+files[6]).c_str(), &t);
//         break;
//         case 7:
//         tl.loadOBJ(string(dir+"/"+files[7]).c_str(), &t);
//         break;
//         case 8:
//         tl.loadOBJ(string(dir+"/"+files[8]).c_str(), &t);
//         break;
//         case 9:
//         tl.loadOBJ(string(dir+"/"+files[9]).c_str(), &t);
//         break;
//         case 100:
//         exit (0);
//         break;
//     }
//     // Almost any menu selection requires a redraw
//     glutPostRedisplay();
// }

// int BuildPopupMenu (void) {
//     int menu, submenu;
//     vector<string> files = vector<string>();

//     submenu = glutCreateMenu (SelectFromMenu);
//     getdir(dir,files);

//     int options = files.size();
//     for (unsigned int i = 0; i < options; i++) {
//         string s = files[i].substr(0, files[i].length()-4);
//         glutAddMenuEntry (s.c_str(), i);
//     }

//     menu = glutCreateMenu (SelectFromMenu);
//     glutAddSubMenu("Display", submenu);

//     glutAddMenuEntry ("Exit", 100);

//     return submenu;
// }

ObjectNode world;
TrimeshLoader tl;
CameraNode* c;
GeometryNode* g;
int px, py, enableOrbit, enableZoom, enablePOI, enableInertia;

void slow(int value) {
    int i = 0;
    if(value == 1){
        i = c->decelerate(1);
        glutPostRedisplay();
        glutTimerFunc(10, slow, i);
    }
}

void KeyboardFunc(unsigned char Key, int x, int y)
{
    switch(Key)
    { 
        case 'x': 
            exit(0);
            break;
        case 'l': 
            g->toggle();
            glutPostRedisplay();
            break;
        case 'z':
            enableZoom = enableZoom ? 0 : 1;
            break;
        case 'o':
            enableOrbit = enableOrbit ? 0 : 1;
            break;
        case 'p':
            enablePOI = enablePOI ? 0 : 1;
            break;
        case 'i':
            enableInertia = enableInertia ? 0 : 1;
            break;

    }
}

void MouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        enableOrbit = 1;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        slow(1);
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        // enableZoom = 1;
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        // enablePOV = 1; // For Right Click 
    if(state == GLUT_UP)
        enableOrbit = enableZoom = enablePOI = enableInertia = 0;

    px = x;
    py = y;
}

void MouseMove(int x, int y) {
    if(enableZoom)
        c->zoom(y - py);
    if(enableOrbit)
        c->orbit(px - x, py - y);
    if(enablePOI)
        c->poi(px - x, py - y);
        
    px = x;
    py = y;
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    world.update();
    glutSwapBuffers();
}

void reshape(int width, int height) {
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char **argv)
{
// dir = string(argv[1]);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("3D Model Viewer");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(KeyboardFunc);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMove);

// BuildPopupMenu ();
// glutAttachMenu (GLUT_RIGHT_BUTTON);

    LightNode* l0 = new LightNode();
    LightNode* l1 = new LightNode();

    TransformNode* t = new TransformNode();
    g = new GeometryNode(argv[1]);

    tl.loadOBJ(g);
    g->initialize();
    g->setMode(SHADED);

    t->addChild(g);
    t->translate(-g->center.x, -g->center.y, -g->center.z);

    float lightPosition[] = {20.0f, 0.0f, 0.0f, 1.0f};
    float lightDiffuse[] = {1.0f, 1.0f, 0.0f, 1.0f};
    float lightAmbient[] = {0.0f, 0.0f, 1.0f, 1.0f};
    l0->initialize(lightPosition, lightDiffuse, lightAmbient);
    l0->enable(GL_LIGHT0);

    float lightPosition_1[] = {-20.0f, 0.0f, 0.0f, 1.0f};
    float lightDiffuse_1[] = {0.149f, 0.694f, 0.988f, 1.0f};
    float lightAmbient_1[] = {0.801f, 0.306f, 0.022f, 1.0f};
    l1->initialize(lightPosition_1, lightDiffuse_1, lightAmbient_1);
    l1->enable(GL_LIGHT1);

    c = new CameraNode(0.25f/g->radius);

    world.addChild(c);
    world.addChild(l0);
    world.addChild(l1);
    world.addChild(t);

    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glutTimerFunc(1000, slow, 0);

    glutMainLoop();
}








