#include <cmath>
#include <cstdlib>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265
/* 
 * Compilation and Execution:
 * 
 * Compilation: g++ -o clown animatedClown.cpp -lglut -lGL -lGLU -lm
 *  
 * Execution: ./clown
 */
using namespace std;
/* Adicionar um nariz ao palhaço, de formato circular que age como as orelhas */
// Globals
static float angle = 0.0;   // Rotation.angle of hat
static int isAnimate = 0;   // Animated?
static int animationPeriod = 100; // Time interval between frames
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;

// Drawing Routine
/**Indicating the parts contribued by clown1.cpp, clown2.cpp and clown3.cpp */
/*Uncomment the part from clown2.cpp and them clown3.cpp for those programs*/

void drawScene(void)
{
    float t; // Parameter along helix
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    /* Modeling Transformations */
    glTranslatef(0.0, 0.0, -9.0);
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);


    /* clown1.cpp starts here */
    //glTranslatef(0.0, 0.0, -9.0);
    
    /* Head */
    glColor3f(0.0, 0.0, 1.0);
    glutWireSphere(2.0, 20, 20);
    /* clown1.cpp ends here */

    /* clown2.cpp starts here */
    glPushMatrix();

        /* Transformations of the hat and brim */
        glRotatef(angle, 0.0, 1.0, 0.0);
        glRotatef(-30.0, 0.0, 0.0, 1.0);
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        glTranslatef(0.0, 0.0, 2.0);

        /* Hat */
        glColor3f(0.0, 1.0, 0.0);
        glutWireCone(2.0, 4.0, 20, 20);

        /* Brim */
        glutWireTorus(0.2, 2.2, 10, 25);
    
    glPopMatrix();
    /* clown2.cpp ends here */

    /* clown3.cpp starts here */
    glPushMatrix();

        /* Transformations of the left ear */
        glTranslatef(sin((PI / 180.0) * angle), 0.0, 0.0);
        glTranslatef(3.5, 0.0, 0.0);

        /* Left Ear */
        glColor3f(1.0, 0.0, 0.0);
        glutWireSphere(0.5, 10, 10);

    glPopMatrix();

    glPushMatrix();
        
        /* Transformations of the right ear */
        glTranslatef(-sin((PI / 180.0) * angle), 0.0, 0.0);
        glTranslatef(-3.5, 0.0, 0.0);

        /* Right Ear */
        glColor3f(1.0, 0.0, 0.0);
        glutWireSphere(0.5, 10, 10);

    glPopMatrix();

    glPushMatrix();

        /* Transformations of the spring to the left ear */
        glTranslatef(-2.0, 0.0, 0.0);
        glScalef((-1 - sin((PI / 180.0) * angle)), 1.0, 1.0);

        /* Spring to the left ear */
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_LINE_STRIP);
        for(t = 0.0; t <= 1.0; t += 0.05)
            glVertex3f(t, 0.25 * cos(10*PI*t), 0.25 * sin(10.0 * PI * t));
        glEnd();
    
    glPopMatrix();

    glPushMatrix();

        /* Transformations of the spring to the right ear */
        glTranslatef(2.0, 0.0, 0.0);
        glScalef((1 + sin((PI / 180.0) * angle)), 1.0, 1.0);

        /* Spring to the right ear */
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_LINE_STRIP);
        for(t = 0.0; t <= 1.0; t += 0.05)
            glVertex3f(t, 0.25 * cos(10*PI*t), 0.25 * sin(10.0 * PI * t));
        glEnd();
    
    glPopMatrix();

    /* clown3.cpp ends here */

    /* PT2 starts here */
    glPushMatrix();

        /* Spheric Nose */
        glTranslatef(0.0, 0.0, sin((PI / 180.0) * angle));
        glTranslatef(0.0, 0.0, 3.3);
        
       glColor3f(6.0, 0.0, 6.0);
       glutWireSphere(0.5, 10, 10);
   glPopMatrix();

    glPushMatrix();

        /* Transformations of the nose */
        glTranslatef(0.0, 0.0, 2.0);
        glScalef(1.0, 1.0, 1+sin((PI / 180.0) * angle));

        /* Spring to the nose */
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINE_STRIP);
        for(t = 0.0; t <= 1.0; t += 0.05)
            glVertex3f(0.25 * cos(10*PI*t), 0.25 * sin(10.0 * PI * t), t);
        glEnd();
    
    glPopMatrix();

    /* PT2 ends here */
    glutSwapBuffers();
}

/* TIMER FUNCTION. */
void animate(int value)
{
    if(isAnimate)
    {
        angle += 5.0;
        if(angle > 360)
            angle -= 360.0;
    }
    glutTimerFunc(animationPeriod, animate, 1);
    glutPostRedisplay();
}

/* Initialization routine */
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

/* OpenGL window reshape routine */
void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

/* Keyboard input processing routine */
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
        case ' ':
            if(isAnimate)
                isAnimate=0;
            else
                isAnimate=1;
            glutPostRedisplay();
            break;
        case 'x':
            Xangle += 5.0;
            if(Xangle > 360.0) Xangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'X':
            Xangle -= 5.0;
            if (Xangle < 0.0) Xangle += 360.0;
            glutPostRedisplay();
            break;
        case 'y':
            Yangle += 5.0;
            if(Yangle > 360.0) Yangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Y':
            Yangle -= 5.0;
            if (Yangle < 0.0) Yangle += 360.0;
            glutPostRedisplay();
            break;
        case 'z':
            Zangle += 5.0;
            if(Zangle > 360.0) Zangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Z':
            Zangle -= 5.0;
            if (Zangle < 0.0) Zangle += 360.0;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

/* Callback routine for non-ASCII key entry */
void specialKeyInput(int key, int x, int y)
{
    if(key == GLUT_KEY_DOWN)
        animationPeriod += 5;
    if(key == GLUT_KEY_UP)
        if(animationPeriod > 5)
            animationPeriod-= 5;
    glutPostRedisplay;
}

/* Routine to output interaction instructions to the C++ Window */
void printInteraction(void)
{
    cout << "Interaction: " << endl;
    cout << "Press space to toggle between animation on and off." << endl
         << "Press the up/down arrow keys to speed up/slow down animation." << endl
         << "Press x/X to change X axle" << endl
         << "Press y/Y to change Y axle" << endl
         << "Press Z/Z to change Z axle" << endl;
}

/* Main Routine*/
int main(int argc, char **argv)
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("PT2 - Gabriel Faga");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);
    glutTimerFunc(5, animate, 1);
    glutMainLoop();

    return 0;
}
