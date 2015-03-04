/* This system will read input files in .obj format, build scene graphs using them, and render the results. */

#include <stdlib.h>
#include <stdio.h>
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

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glutSwapBuffers();
}

void myReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0f, 2.0f, -2.0f, 2.0f, -4.0f, 4.0f);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}


void SelectFromMenu(int idCommand) {
    switch (idCommand)
    {
    // case 0:
    //     glutDisplayFunc(display);
    //     glutIdleFunc(display);
    //   break;
    // case 1:
    //     glutDisplayFunc(displayInverse);
    //     glutIdleFunc(displayInverse);
    //     break;
    // case 2:
    //     glutDisplayFunc(displayBoth);
    //     glutIdleFunc(displayBoth);
    //     break;
    case 3:
        exit (0);
        break;
    }
  // Almost any menu selection requires a redraw
  glutPostRedisplay();
}

int BuildPopupMenu (void) {
  int menu;
  menu = glutCreateMenu (SelectFromMenu);
  glutAddMenuEntry ("Exit", 3);

  return menu;
}

void MouseButton(int button, int state, int x, int y) {
    // state = 0 pressed
    //       = 1 released
    // if (button == GLUT_LEFT_BUTTON && state == 0) {
    //     n = (n + 1) % 4;
    // }
    // if (button == GLUT_MIDDLE_BUTTON) {
    //     middle_pressed = 1;
    //     mouseX = x;
    //     mouseY = y;
    // }
    // else {
    //    iddle_pressed = 0;
    // }
}

void MouseMove(int x, int y) {
    // if(middle_pressed) {
    //     if(mouseX < x){
    //         angleXZ += 0.05; 
    //     }
    //     else if(mouseX > x){
    //         angleXZ -= 0.05;
    //     }
    //     cx = sin(angleXZ);
    //     cz = -1 * cos(angleXZ);
    // }
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

int main(int argc, char **argv)
{
    Trimesh *t;
    TrimeshLoader tl;
    tl.loadOBJ("models/cactus.obj", t);

    // string dir = string(argv[1]);
    // vector<string> files = vector<string>();

    // getdir(dir,files);

    // for (unsigned int i = 0;i < files.size();i++) {
    //     cout << files[i] << endl;
    // }
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("3D Model Viewer");
    glutReshapeFunc(myReshape);
    // glutMouseFunc(MouseButton);
    // glutMotionFunc(MouseMove);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glEnable(GL_DEPTH_TEST);
    // glutTimerFunc(0, timer, 0);
    BuildPopupMenu ();
    glutAttachMenu (GLUT_RIGHT_BUTTON);
    glClearColor(0.0,0.0,0.0,1.0);
    glutMainLoop();
}








