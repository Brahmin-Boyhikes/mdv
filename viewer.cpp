/* This system will read input files in .obj format, build scene graphs using them, and render the results. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h> /* for linux */

GLfloat cx = 0.707f, cy = 0.707f, cz = 0.707f, angleXZ = 0.0f, angleY = 0.0f;


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(cx,cy,cz,0,0,0,0,1,0);
    glutSwapBuffers();
}


// void displayInverse() {
//     currentTime = glutGet(GLUT_ELAPSED_TIME);
//     elapsedTime = currentTime - previousTime;
//     if(elapsedTime > 60) {
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//         glLoadIdentity();
//         gluLookAt(cx,cy,cz,0,0,0,0,1,0);
//         glBegin(GL_QUADS);
//         divide_cube_i(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], n+1);
//         glEnd();
//         glutSwapBuffers();
//     }
// }

// void displayBoth() {
//     currentTime = glutGet(GLUT_ELAPSED_TIME);
//     elapsedTime = currentTime - previousTime;
//     if(elapsedTime > 60) {
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//         glMatrixMode(GL_MODELVIEW);

//         glLoadIdentity();
//         gluLookAt(cx,cy,cz,0,0,0,0,1,0);
//         glTranslatef(-1.0f, 0.0f, 0.0f);

//         glBegin(GL_QUADS);
//         divide_cube(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], n);
//         glEnd();

//         glLoadIdentity();
//         gluLookAt(cx,cy,cz,0,0,0,0,1,0);
//         glTranslatef(1.0f, 0.0f, 0.0f);

//         glBegin(GL_QUADS);
//         divide_cube_i(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], n+1);
//         glEnd();
//          glutSwapBuffers();
//     }
// }

void myReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0f, 2.0f, -2.0f, 2.0f, -4.0f, 4.0f);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

// void timer(int value) {
// glutPostRedisplay();      // Post re-paint request to activate display()
// glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
// }

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
  // glutAddMenuEntry ("Menger Sponge", 0);
  // glutAddMenuEntry ("Inverse Menger Sponge", 1);
  // glutAddMenuEntry ("Both", 2);
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
    //     middle_pressed = 0;
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

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("3D Gasket");
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