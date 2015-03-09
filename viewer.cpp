/* This system will read input files in .obj format, build scene graphs using them, and render the results. */
/* Display at most 10 objects */
/* arg is path of the file */

/* Name: Alan Ma */
/* UTEID: am65355 */
/* CSID: ama */

#include <stdlib.h>
#include <stdio.h>
#include <vector>

#ifdef MACINTOSH
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "loader.h"
#include "BaseNode.h"


ObjectNode world;
TrimeshLoader tl;
CameraNode* c;
GeometryNode* g;
LightNode* l0;

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
        case 'b':
            l0->updateDiffuse(0.01, 0.01, 0.01);
            glutPostRedisplay();
            break;
        case 'd': 
            l0->updateDiffuse(-0.01, -0.01, -0.01);
            glutPostRedisplay();
            break;
        case 'f': 
            g->drawFaceNormal();
            glutPostRedisplay();
            break;
        case 'v': 
            g->drawVertexNormal();
            glutPostRedisplay();
            break;
        case 'i':
            enableInertia = enableInertia ? 0 : 1;
            break;

    }
}

void MouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        enableOrbit = 1;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && enableInertia)
        slow(1);
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        enableZoom = 1;
    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
        enablePOI = 1;
    if(state == GLUT_UP)
        enableOrbit = enableZoom = enablePOI = 0;

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

    l0 = new LightNode();
    LightNode* l1 = new LightNode();
    LightNode* l2 = new LightNode();

    TransformNode* t = new TransformNode();
    g = new GeometryNode(argv[1]);

    tl.loadOBJ(g);
    g->initialize();
    g->setMode(SHADED);

    t->addChild(g);
    t->translate(-g->center.x, -g->center.y, -g->center.z);

    float lightPosition[] = {0.0f, -5.0f, 0.0f, 1.0f};
    float lightDiffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
    float lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    l0->initialize(lightPosition, lightDiffuse, lightAmbient);
    l0->enable(GL_LIGHT0);

    float lightPosition_1[] = {0.0f, 5.0f, 0.0f, 1.0f};
    float lightDiffuse_1[] = {0.1f, 0.1f, 0.1f, 1.0f};
    float lightAmbient_1[] = {0.0f, 0.0f, 0.0f, 1.0f};
    l1->initialize(lightPosition_1, lightDiffuse_1, lightAmbient_1);
    l1->enable(GL_LIGHT1);

    float lightPosition_2[] = {0.0f, -5.0f, 0.0f, 1.0f};
    float lightDiffuse_2[] = {0.1f, 0.1f, 0.1f, 1.0f};
    float lightAmbient_2[] = {0.0f, 0.0f, 0.0f, 1.0f};
    l2->initialize(lightPosition_2, lightDiffuse_2, lightAmbient_2);
    l2->enable(GL_LIGHT2);

    c = new CameraNode(0.25f/g->radius);

    world.addChild(c);
    world.addChild(l0);
    world.addChild(l1);
    world.addChild(l2);
    world.addChild(t);

    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glutTimerFunc(1000, slow, 0);

    glutMainLoop();
}








