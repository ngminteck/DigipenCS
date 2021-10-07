#include <GL/glut.h>
#include <math.h>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Camera.h"
#include "drawutil.h"
#include "Problem2D.h"
#include "Problem3D.h"

float currentTime = 0;
int currentPointIndex = 0;

Problem2D problem2D;
Problem3D problem3D;
Problem* currentProblem;
int startTime = 0;

Camera* camera = 0;
bool cullBackfaces = false;
bool drawTarget = false;
Ray pickingRay;
bool drawPickingRay = true;
bool drawWorldAxes = true;

enum DrawMode { WIREFRAME, FLAT_SHADED, SMOOTH_SHADED, NUM_DRAW_MODES };

int drawMode = FLAT_SHADED;

bool LMB = false, MMB = false, RMB = false;
bool CTRL = false, ALT = false, SHIFT = false;
int mouse_x, mouse_y;

// MI stands for Menu Item
#define MI_SWITCH_PROBLEM1 2
#define MI_SWITCH_PROBLEM2 5
#define MI_TOGGLE_DISPLAY_OF_WORLD_SPACE_AXES 21
#define MI_QUIT 50

// callbacks
void reshapeCallback(int width, int height);
void mouseCallback(int button, int state, int x, int y);
void passiveMotionCallback(int x, int y);
void motionCallback(int x, int y);
void menuCallback(int menuItem);
void keyboardCallback(unsigned char key, int x, int y);
void specialCallback(int key, int x, int y);
void timerCallback(int /* id */);

const int timerInterval =
    20;  // must be > 0 // in milliseconds // decrease for higher framerate

Matrix object;

void problemInit() {
    currentTime = 0;
    currentPointIndex = 0;
}

void problemIncrement() {
    // init start time
    if (startTime == 0) startTime = glutGet(GLUT_ELAPSED_TIME);

    // increment time between two points
    int time = glutGet(GLUT_ELAPSED_TIME);
    int delta = time - startTime;
    startTime = time;
    currentTime += delta / 1000.0f;

    // increment point for each second
    if (currentTime >= 1) {
        currentTime -= 1;
        currentPointIndex++;
        if (currentPointIndex == currentProblem->getPointCount() - 1) {
            currentPointIndex = 0;
        }
    }
}

void start() {
    currentProblem = &problem2D;
    problemInit();
}

void step() {
    Matrix r, p;

    problemIncrement();

    Vector3 r1, r2, r3;
    Vector3 position;
    currentProblem->getRotationAndPosition(currentPointIndex, currentTime, r1, r2, r3, position);
    r.setToRotation(r1, r2, r3);
    r.transpose();
    p.setToTranslation(position);
    object = p * r;
}

void draw() {
    Matrix m;
    float s = 1.0f / 200;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    if (cullBackfaces)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    camera->transform();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw interpolated curve
    Vector3 r1, r2, r3;
    Vector3 position;
    glLineWidth(2.0);
    glEnable(GL_LINE_SMOOTH );
    glEnable(GL_POLYGON_SMOOTH );
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < currentProblem->getPointCount() - 1; i++) {
		for (int t = 0; t <= 20; t++) {
            currentProblem->getRotationAndPosition(i, t / 20.0f, r1, r2, r3, position);
			float x = position.x();
			float y = position.y();
			float z = position.z();
			glVertex3f(x, y, z);
		}
	}
	glEnd();

    // draw points
    float radius = 0.3f;
    for (int i = 0; i < currentProblem->getPointCount(); i++) {
        currentProblem->getRotationAndPosition(i, 0, r1, r2, r3, position);
        float x = position.x();
        float y = position.y();
        float z = position.z();

        glPushMatrix();
        glTranslatef(x, y, z);
        glutSolidSphere(radius, 8, 8);
        glPopMatrix();
    }

    // ----- draw a string suspended in space
    glPushMatrix();
    glMultMatrixf(object.get());
    if (drawWorldAxes) {
        drawAxes(Point3(0, 0, 0), 2, true);
    }
    glScalef(s, s, s);
    string label = string("   ") + currentProblem->getName();
    glColor3f(1, 1, 1);
    for (size_t j = 0; j < label.size(); j++)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, label[j]);
    glPopMatrix();

    // ----- setup for drawing objects
    if (drawMode == 0) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        GLfloat ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat position[] = {0.0f, 3.0f, 3.0f, 0.0f};

        GLfloat lmodel_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat local_view[] = {0.0};

        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
        glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

        glFrontFace(GL_CW);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_AUTO_NORMAL);
        glEnable(GL_NORMALIZE);
        glEnable(GL_DEPTH_TEST);

        glShadeModel(drawMode == 2 ? GL_SMOOTH : GL_FLAT);
    }

    glColor3f(1, 1, 1);

    // ----- draw objects
    glPushMatrix();
    // object1
    glMultMatrixf(object.get());
    glutSolidTeapot(1.0);
    glPopMatrix();

    if (drawMode == 0)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glDisable(GL_LIGHTING);

    // ----- draw the selected picking ray
    if (drawPickingRay) {
        glColor3f(0, 1, 1);
        drawRay(pickingRay);
    }

    // ----- draw little crosshairs at the camera's target
    if (drawTarget) {
        const Point3& t = camera->getTarget();
        glColor3f(0, 1, 1);
        drawCrossHairs(t, camera->convertLength(t, 0.05f));
    }

    // ----- finish up
    glutSwapBuffers();
}

void drawCallback() {
    step();
    draw();
}

// callbacks

void reshapeCallback(int width, int height) {
    if (0 != camera) camera->resizeViewport(width, height);
}

void mouseCallback(int button, int state, int x, int y) {
    int mod = glutGetModifiers();
    CTRL = mod & GLUT_ACTIVE_CTRL;
    ALT = mod & GLUT_ACTIVE_ALT;
    SHIFT = mod & GLUT_ACTIVE_SHIFT;

    switch (button) {
        case GLUT_LEFT_BUTTON:
            LMB = state == GLUT_DOWN;
            break;
        case GLUT_MIDDLE_BUTTON:
            MMB = state == GLUT_DOWN;
            break;
        case GLUT_RIGHT_BUTTON:
            RMB = state == GLUT_DOWN;
            break;
    }

    if (state == GLUT_DOWN && !(CTRL || ALT)) {
        pickingRay = camera->computeRay(x, y);
        glutPostRedisplay();
    }

    mouse_x = x;
    mouse_y = y;

    printf("buttons:(%d,%d,%d); modifiers:(%c,%c,%c); position:(%d,%d)\n",
           LMB ? 1 : 0, MMB ? 1 : 0, RMB ? 1 : 0, CTRL ? 'C' : '-',
           ALT ? 'A' : '-', SHIFT ? 'S' : '-', mouse_x, mouse_y);
}

void passiveMotionCallback(int x, int y) {
    mouse_x = x;
    mouse_y = y;
    // printf("move:(%d,%d)\n", mouse_x, mouse_y );
}

void motionCallback(int x, int y) {
    float delta_x = float(x - mouse_x);
    float delta_y = float(mouse_y - y);
    float delta = delta_x + delta_y;

    //if (SHIFT) {
    //    // The redundant functionality here is for users with 2-button mice.
    //    if (LMB) {
    //        if (ALT)
    //            camera->dollyCameraForward(3 * delta, false);
    //        else
    //            camera->translateSceneRightAndUp(delta_x, delta_y);
    //    }
    //} else if (CTRL) {
    //    if (LMB && MMB)
    //        camera->rollCameraRight(delta);
    //    else if (LMB) {
    //        camera->pitchCameraUp(delta_y);
    //        camera->yawCameraRight(delta_x);
    //    } else if (MMB)
    //        camera->zoomIn(delta);
    //} else if (ALT) {
    //    if (LMB && MMB)
    //        camera->dollyCameraForward(3 * delta, false);
    //    else if (LMB)
    //        camera->orbit(float(mouse_x), float(mouse_y), float(x), float(y));
    //    else if (MMB)
    //        camera->translateSceneRightAndUp(delta_x, delta_y);
    //}
    if(LMB)
          camera->translateSceneRightAndUp(delta_x, delta_y);
    else if(RMB)
        camera->orbit(float(mouse_x), float(mouse_y), float(x), float(y));
    else if (MMB)
        camera->zoomIn(delta);
    

    glutPostRedisplay();

    mouse_x = x;
    mouse_y = y;
    // printf("drag:(%d,%d)\n", mouse_x, mouse_y );
}

void menuCallback(int menuItem) {
    switch (menuItem) {
        case MI_SWITCH_PROBLEM1:
            printf("problem 2D\n");
            currentProblem = &problem2D;
            problemInit();
            glutPostRedisplay();
            break;
        case MI_SWITCH_PROBLEM2:
            printf("problem 3D\n");
            currentProblem = &problem3D;
            problemInit();
            glutPostRedisplay();
            break;
        case MI_TOGGLE_DISPLAY_OF_WORLD_SPACE_AXES:
            drawWorldAxes = !drawWorldAxes;
            glutPostRedisplay();
            break;
        case MI_QUIT:
            exit(0);
            break;
        default:
            printf("unknown menu item %d was selected\n", menuItem);
            break;
    }
}

void keyboardCallback(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            menuCallback(MI_TOGGLE_DISPLAY_OF_WORLD_SPACE_AXES);
            break;
        case '1':
            menuCallback(MI_SWITCH_PROBLEM1);
            break;
        case '2':
            menuCallback(MI_SWITCH_PROBLEM2);
            break;
        case 27:  // Escape
            menuCallback(MI_QUIT);
            break;
        default:
            printf("untrapped key %d\n", (int)key);
            break;
    }
}

void specialCallback(int key, int x, int y) {
    switch (key) {
        // function keys
        case GLUT_KEY_F1:
        case GLUT_KEY_F2:
        case GLUT_KEY_F3:

        // arrow keys
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_UP:
        case GLUT_KEY_DOWN:

        default:
            printf("untrapped special key %d\n", (int)key);
            break;
    }
}

void timerCallback(int /* id */) {
    glutPostRedisplay();
    glutTimerFunc(timerInterval, timerCallback, 0);
}

int main(int argc, char* argv[]) {
    start();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Interpolation Questions Basic Rendering in OpenGL");

    glutDisplayFunc(drawCallback);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keyboardCallback);
    glutSpecialFunc(specialCallback);
    glutMouseFunc(mouseCallback);
    glutPassiveMotionFunc(passiveMotionCallback);
    glutMotionFunc(motionCallback);

    glutTimerFunc(timerInterval, timerCallback, 0);

    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    camera = new Camera(width, height, 12, Point3(0, 0, 0));

    glutMainLoop();
}
