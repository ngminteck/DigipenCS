////////////////////////////////////////////////////////////////////////
// All keyboard, mouse, and other interactions are implemented here.
// The single entry point, InitInteraction, sets up GLUT callbacks for
// various events that an interactive graphics program needs to
// handle.
//
// The various callbacks are:
//    glutDisplayFunc: called repeatedly to redraw the scene every 30th of a second.
//    glutReshapeFunc: called whenever the window changes size
//    glutKeyboardFunc and glutKeyboardUpFunc: called whenever a key is pressed or released
//    glutMouseFunc:  called when a mouse button is pressed or released
//    glutMotionFunc: called when the mouse is moved

#include "framework.h"

extern Scene scene;       // Declared in framework.cpp, but used here.

// Some globals used for mouse handling.
int mouseX, mouseY;
bool shifted = false;
bool ctrl = false;
bool space = false;
bool leftDown = false;
bool middleDown = false;
bool rightDown = false;



////////////////////////////////////////////////////////////////////////
// Called by GLUT when the scene needs to be redrawn.
void ReDraw()
{
    scene.DrawScene();
    glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////
// Function called to exit
void Quit(void *clientData)
{
    glutLeaveMainLoop();
}

////////////////////////////////////////////////////////////////////////
// Called by GLUT when the window size is changed.
void ReshapeWindow(int w, int h)
{
    if (w && h) {
        glViewport(0, 0, w, h); }
    scene.width = w;
    scene.height = h;

    // Force a redraw
    glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////
// Called by GLut for keyboard actions.
void KeyboardDown(unsigned char key, int x, int y)
{
    printf("key down %c(%d)\n", key, key);
    fflush(stdout);
    
	
    switch(key) 
	{
		// tab
		case 9:
			++scene.project;
			if (scene.project > 2)
				scene.project = 1;
			break;
		case 16:
			shifted = true;
			break;

		case 'w':
			scene.wDown = true;
			break;
		case 's':
			scene.sDown = true;
			break;
		case 'a':
			scene.aDown = true;
			break;
		case 'd':
			scene.dDown = true;

			break;
		case '0': 
		case '1': 
		case '2': 
		case '3': 
		case '4':
		case '5': 
		case '6': 
		case '7': 
		case '8': 
		case '9':
        scene.mode = key-'0';
        break;

    case 27: case 'q':       // Escape and 'q' keys quit the application
        exit(0);
    }
}

void KeyboardUp(unsigned char key, int x, int y)
{
    fflush(stdout);

	switch (key)
	{
	case 16:
		shifted = false;
		break;
	case 'w':
		scene.wDown = false;
		break;
	case 's':
		scene.sDown = false;
		break;
	case 'a':
		scene.aDown = false;
		break;
	case 'd':
		scene.dDown = false;
	}
}

////////////////////////////////////////////////////////////////////////
// Called by GLut when a mouse button changes state.
void MouseButton(int button, int state, int x, int y)
{        
    
    // Record the position of the mouse click.
    mouseX = x;
    mouseY = y;

    // Test if the SHIFT key was down for this mouse click
    shifted = glutGetModifiers() & GLUT_ACTIVE_SHIFT;
	if ((glutGetModifiers() & GLUT_ACTIVE_CTRL) != 0)
		ctrl = true;
	else
		ctrl = false;

    // Ignore high order bits, set by some (stupid) GLUT implementation.
    button = button%8;

	if (shifted)
	{
		// Figure out the mouse action, and handle accordingly
		if (button == 3 )
		{
			// Scroll light in
			scene.lightDist = pow(scene.lightDist, 1.0f / 1.02f);
			printf("shifted scroll up\n");
		}

		if (button == 4 )
		{
			// Scroll light out
			scene.lightDist = pow(scene.lightDist, 1.02f);
			printf("shifted scroll down\n");
		}

	}
	else if (ctrl)
	{
		if (button == 3 )
		{
			scene.ry += 0.1f;
			
			printf("control scroll up\n");
		}

		if (button == 4 )
		{
			scene.ry -= 0.1f;
			printf("control scroll down\n");
		}
	}
	else
	{
		if (button == 3)
		{
			scene.zoom += 1.0f;
			printf("scroll up\n");
		}

		if (button == 4)
		{
			scene.zoom -= 1.0f;
			if (scene.zoom <= 0.0f)
				scene.zoom = 1.0f;
			printf("scroll down\n");
		}
	}
   
    if (button == GLUT_LEFT_BUTTON) {
        leftDown = (state == GLUT_DOWN);
        printf("Left button down\n"); }

    if (button == GLUT_MIDDLE_BUTTON) {
        middleDown = (state == GLUT_DOWN);
        printf("Middle button down\n");  }

    if (button == GLUT_RIGHT_BUTTON) {
        rightDown = (state == GLUT_DOWN);
        printf("Right button down\n");  }


    // Force a redraw
    glutPostRedisplay();
    fflush(stdout);
}

////////////////////////////////////////////////////////////////////////
// Called by GLut when a mouse moves (while a button is down)
void MouseMotion(int x, int y)
{
    // Calculate the change in the mouse position
    int dx = x-mouseX;
    int dy = y-mouseY;

	if (shifted)
	{
		if (leftDown && shifted)
		{
			// Rotate light position
			scene.lightSpin += dx / 3.0f;
			scene.lightTilt -= dy / 3.0f;
		}

		if (middleDown && shifted)
		{
			scene.lightDist = pow(scene.lightDist, 1.0f - dy / 200.0f);
		}

		if (rightDown && shifted)
		{

		}
		
	}
	else
	{
		if (leftDown)
		{
			if (scene.project == 1)
			{
				scene.spin += dx / 3.0f;
				scene.tilt -= dy / 3.0f;
			}
			else if (scene.project == 2)
			{
				scene.spin += static_cast<float>((dx / 3.0f));

				

				scene.tilt -= static_cast<float>((dy / 3.0f));
				
				if (scene.tilt < -90.0f)
					scene.tilt = -90.0f;

				if(scene.tilt > 90.0f)
					scene.tilt = 90.0f;

				
			}
		}

		if (middleDown)
		{
			scene.ry = 0.2f;
			scene.spin = 0.0f;
			scene.tilt = 2.0f;
			scene.tx = 0.0f;
			scene.ty = 0.0f;
			scene.zoom = 25.0f;
		}

		if (rightDown)
		{
			scene.tx += dx / 30.0f;
			scene.ty -= dy / 30.0f;
		}
	}

    // Record this position
    mouseX = x;
    mouseY = y;

    // Force a redraw
    glutPostRedisplay();
}

void InitInteraction()
{
    glutIgnoreKeyRepeat(true);  // DO NOT change or disable this line!
    
    glutDisplayFunc(&ReDraw);
    glutReshapeFunc(&ReshapeWindow);

    glutKeyboardFunc(&KeyboardDown);
    glutKeyboardUpFunc(&KeyboardUp);

    glutMouseFunc(&MouseButton);
    glutMotionFunc(&MouseMotion);
}
