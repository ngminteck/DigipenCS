#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <math.h>

#include "Camera.h"
#include "drawutil.h"

#include <GL/glut.h>


std::string inPathP1 = "path1.txt";
std::string inPathP2 = "path2.txt";
std::string outPathP1 = "output1.txt";
std::string outPathP2 = "output2.txt";

int theta1P1, theta2P1, NP1, P1 = 0;
Vector3 v1P2, v2P2, outputVec;
int theta1P2, theta2P2, NP2, P2 = 0;
float outputP1Angle, outputP2Angle;
bool p1Done = false;
bool p2Done = false;

Quaternion quat1;
Quaternion quat2;
Quaternion outputQuat;

int problem = 0;



Camera* camera = 0;
bool cullBackfaces = true;
bool drawTarget = false;
Ray pickingRay;
bool drawPickingRay = true;
bool drawWorldAxes = true;

enum DrawMode {
	WIREFRAME,
	FLAT_SHADED,
	SMOOTH_SHADED,
	NUM_DRAW_MODES
};

int drawMode = 1;


bool LMB=false, MMB=false, RMB=false;
bool CTRL=false, ALT=false, SHIFT=false;
int mouse_x, mouse_y;

// MI stands for Menu Item
#define MI_SWITCH_PROBLEM1 2
#define MI_SWITCH_PROBLEM2 5
#define MI_CYCLE_DRAW_MODE 10
#define MI_TOGGLE_DISPLAY_OF_WORLD_SPACE_AXES 21
#define MI_TOGGLE_DISPLAY_OF_CAMERA_TARGET 22
#define MI_TOGGLE_DISPLAY_OF_BACKFACES 23
#define MI_TOGGLE_DISPLAY_OF_PICKING_RAY 24
#define MI_LOOKAT_OBJ1 30
#define MI_LOOKAT_OBJ2 35
#define MI_RESET_CAMERA 40
#define MI_QUIT 50

//callbacks
void reshapeCallback( int width, int height );
void mouseCallback( int button, int state, int x, int y );
void passiveMotionCallback( int x, int y );
void motionCallback( int x, int y );
void menuCallback( int menuItem );
void keyboardCallback( unsigned char key, int x, int y );
void specialCallback( int key, int x, int y );
void timerCallback( int /* id */ );

float timerInterval = 300;  //must be > 0 // in milliseconds // decrease to make program faster

Point3 object1Position;
Point3 object2Position;
Matrix object1;
Matrix object2;




void problem1Init()
{
  //open and read file
  std::ifstream inputFile(inPathP1);
  if(!inputFile.is_open()){
    std::cout << "ERROR at 1: Could not open file " << inPathP1 << std::endl;
    return;
  }

  inputFile >> theta1P1 >> theta2P1 >> NP1;

	outputP1Angle = theta1P1;

	std::ofstream outputFile(outPathP1);
	if(outputFile.is_open()){
		outputFile << "";
	}else{
		std::cout << "ERROR at 1: Could not open file " << outPathP1 << std::endl;
	}
}

void problem1Increment()
{
	//problem 1
	if (P1 < (NP1+1))
	{
		float step = float(
	    theta1P1 > theta2P1 ? 360-abs(theta2P1-theta1P1) : abs(theta2P1-theta1P1)
	  ) / NP1;

		//printf("step %f\n", step);
		if (p1Done == false){
			std::ofstream outputFile;
			outputFile.open(outPathP1, std::fstream::app);
		  if(outputFile.is_open())
			{
		    outputFile << outputP1Angle << ",\n";
		  }else{
		    std::cout << "ERROR at 1: Could not open file " << outPathP1 << std::endl;
		  }
		}

		outputP1Angle = fmod(outputP1Angle + step, 360);
		P1++;
	}else{
		P1 = 0;
		outputP1Angle = theta1P1;
		p1Done = true;
	}
}




void problem2Init()
{
	//open and read file
  std::ifstream inputFile(inPathP2);
  if(!inputFile.is_open()){
    std::cout << "ERROR at 2: Could not open file " << inPathP2 << std::endl;
    return;
  }


  std::string workString;

  //v1
  std::getline(inputFile, workString);
  std::stringstream stream(workString);
  float p;
  for (int i=0; i<4; i++){
    if (stream){
      stream >> p;
      v1P2.get()[i] = p;
    }
  }

  //v2
  std::getline(inputFile, workString);
  std::stringstream stream1(workString);
  for (int i=0; i<4; i++){
    if (stream1){
      stream1 >> p;
      v2P2.get()[i] = p;
    }
  }

  //N
  inputFile >> p;
  NP2 = int(p);


  outputVec.x() = float(v1P2.x());
  outputVec.y() = float(v1P2.y());
  outputVec.z() = float(v1P2.z());
  outputP2Angle = float(v1P2.w());

  //float radianAngle1 = degrees_to_radians(v1P2.w());
  //float radianAngle2 = degrees_to_radians(v2P2.w());
  //Vector3 nVec1 = v1P2.normalized();
  //Vector3 nVec2 = v2P2.normalized();
 
  //quat1.w() = cos(radianAngle1 * 0.5);
  //float sin1 = sin(radianAngle1 * 0.5);
  //quat1.x() = nVec1.x() * sin1;
  //quat1.y() = nVec1.y() * sin1;
  //quat1.z() = nVec1.z() * sin1;
  //
  //quat2.w() = cos(radianAngle2 * 0.5);
  //float sin2 = sin(radianAngle2 * 0.5);
  //quat2.x() = nVec2.x() * sin2;
  //quat2.y() = nVec2.y() * sin2;
  //quat2.z() = nVec2.z() * sin2;


	quat1 = Quaternion::EulerToQuaternion(axisRotToEuler(degrees_to_radians(v1P2.w()), v1P2));
	quat2 = Quaternion::EulerToQuaternion(axisRotToEuler(degrees_to_radians(v2P2.w()), v2P2));

	outputQuat = quat1;

	std::ofstream outputFile(outPathP2);
	if(outputFile.is_open()){
		outputFile << "";
	}else{
		std::cout << "ERROR at 1: Could not open file " << outPathP2 << std::endl;
	}
}

void problem2Increment()
{
	//problem 1
	if (P2 < (NP2+1))
	{
		float step = float(
	    v1P2.w() > v2P2.w() ? 360-abs(v2P2.w()-v1P2.w()) : abs(v2P2.w()-v1P2.w())
	  ) / NP2;

		if (p2Done == false){
			std::ofstream outputFile;
			outputFile.open(outPathP2, std::fstream::app);
		  if(outputFile.is_open())
			{
		    outputFile << outputQuat.x() << ", " << outputQuat.y() << ", " << outputQuat.z() << ", " << outputQuat.w() << "\n";
		  }else{
		    std::cout << "ERROR at 2: Could not open file " << outPathP2 << std::endl;
		  }
		}

		outputP2Angle = fmod(outputP2Angle + step, 360);

		
		outputQuat = Slerp(quat1, quat2, float(P2) / NP2);


		P2++;
	}else{
		P2 = 0;
		outputVec.x() = float(v1P2.x());
	  outputVec.y() = float(v1P2.y());
	  outputVec.z() = float(v1P2.z());
		outputP2Angle = float(v1P2.w());
		outputQuat = quat1;
		p2Done = true;
	}
}



void start()
{
	problem1Init();
	problem2Init();
}


void step()
{
	Matrix r, p;

	problem1Increment();
	r.setToRotation( Vector3(0, 0, degrees_to_radians(outputP1Angle)) );
	p.setToTranslation(Vector3(0, 0, 0));
	object1 = p * r;
	object1Position = Point3(0, 0, 0);

	problem2Increment();
	r.setToRotation( outputQuat.toEuler() );
	p.setToTranslation(Vector3(0, 0, 0));
	object2 = p * r;
	object2Position = Point3(0, 0, 0);
}


void draw()
{
	Matrix m;
	float s = 1.0f/200;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDepthFunc( GL_LEQUAL );
	glEnable( GL_DEPTH_TEST );

	if ( cullBackfaces ) glEnable( GL_CULL_FACE );
	else glDisable( GL_CULL_FACE );

	camera->transform();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();



	// if ( drawWorldAxes ) {
  //     drawAxes( Point3(0,0,0), 2.5, true );
	//
  //     glColor3f( 0.5f, 0.5f, 0.5f );
  //     m.setToLookAt( Point3(2.5,0,0), Point3(0,0,0), Vector3(0,1,0), true );
  //     glPushMatrix();
  //        glMultMatrixf( m.get() );
  //        glScalef( s, s, s );
  //        glutStrokeCharacter( GLUT_STROKE_MONO_ROMAN, 'x' );
  //     glPopMatrix();
  //     m.setToLookAt( Point3(0,2.5,0), Point3(0,0,0), Vector3(1,0,0), true );
  //     glPushMatrix();
  //        glMultMatrixf( m.get() );
  //        glScalef( s, s, s );
  //        glutStrokeCharacter( GLUT_STROKE_MONO_ROMAN, 'y' );
  //     glPopMatrix();
  //     m.setToLookAt( Point3(0,0,2.5), Point3(0,0,0), Vector3(0,1,0), true );
  //     glPushMatrix();
  //        glMultMatrixf( m.get() );
  //        glScalef( s, s, s );
  //        glutStrokeCharacter( GLUT_STROKE_MONO_ROMAN, 'z' );
  //     glPopMatrix();
  //  }



	// ----- draw a string suspended in space
	if (problem == 0){

		glPushMatrix();
			 glMultMatrixf( object1.get() );
			 if ( drawWorldAxes ){ drawAxes( Point3(0,0,0), 2, true ); }
			 glScalef( s, s, s );
			 char q1[] = "   Rotate";
			 glColor3f( 1, 1, 1 );
			 for ( int j = 0; q1[j] != '\0'; ++j )
					glutStrokeCharacter( GLUT_STROKE_MONO_ROMAN, q1[j] );
		glPopMatrix();

	}else if (problem == 1){

		glPushMatrix();
			 glMultMatrixf( object2.get() );
			 if ( drawWorldAxes ){ drawAxes( Point3(0,0,0), 2, true ); }
			 glScalef( s, s, s );
			 char q2[] = "   Slerp";
			 glColor3f( 1, 1, 1 );
			 for ( int j = 0; q2[j] != '\0'; ++j )
					glutStrokeCharacter( GLUT_STROKE_MONO_ROMAN, q2[j] );
		glPopMatrix();

	}




	// ----- setup for drawing objects
	if ( drawMode == 0 ) {
		 glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	else {
			GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
			GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
			GLfloat position[] = {0.0, 3.0, 3.0, 0.0};

			GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
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

		 glShadeModel( drawMode==2 ? GL_SMOOTH : GL_FLAT );
	}

	glColor3f( 1, 1, 1 );

	// ----- draw objects
	if (problem == 0){

		glPushMatrix();
			 // object1
			 glMultMatrixf( object1.get() );
			 glutSolidTeapot( 1.0 );
		glPopMatrix();

	}else if (problem == 1){

		glPushMatrix();
			 // object2
			 glMultMatrixf( object2.get() );
			 glutSolidTeapot( 1.0 );
		glPopMatrix();

	}



	if ( drawMode == 0 ) glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	else glDisable( GL_LIGHTING );



	// ----- draw the selected picking ray
	if ( drawPickingRay ) {
		 glColor3f( 0, 1, 1 );
		 drawRay( pickingRay );
	}

	// ----- draw little crosshairs at the camera's target
	if ( drawTarget ) {
		 const Point3& t = camera->getTarget();
		 glColor3f( 0, 1, 1 );
		 drawCrossHairs( t, camera->convertLength( t, 0.05 ) );
	}

	// ----- finish up
	glutSwapBuffers();
}



void drawCallback()
{
	step();
	draw();
}



int main( int argc, char *argv[] )
{
	start();

	 glutInit( &argc, argv );
   glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
   glutInitWindowSize( 800, 600 );
   glutCreateWindow("Interpolation Questions Basic Rendering in OpenGL");

   glutDisplayFunc( drawCallback );
   glutReshapeFunc( reshapeCallback );
   glutKeyboardFunc( keyboardCallback );
   glutSpecialFunc( specialCallback );
   glutMouseFunc( mouseCallback );
   glutPassiveMotionFunc( passiveMotionCallback );
   glutMotionFunc( motionCallback );

   glutTimerFunc( timerInterval, timerCallback, 0 );

   // create a menu
  // int mainMenu = glutCreateMenu( menuCallback );
  // glutSetMenu( mainMenu ); // make it the current menu
	// glutAddMenuEntry( "Switch to problem 1 (1)", MI_SWITCH_PROBLEM1 );
	// glutAddMenuEntry( "Switch to problem 2 (2)", MI_SWITCH_PROBLEM2 );
	// glutAddMenuEntry( "Cycle Display Mode (d)", MI_CYCLE_DRAW_MODE );
  //    int displayToggleMenu = glutCreateMenu( menuCallback );
  //    glutSetMenu( displayToggleMenu );
  //    glutAddMenuEntry( "Axes (a)",
  //       MI_TOGGLE_DISPLAY_OF_WORLD_SPACE_AXES );
  //    glutAddMenuEntry( "Camera Target (t)",
  //       MI_TOGGLE_DISPLAY_OF_CAMERA_TARGET );
  //    glutAddMenuEntry( "Picking Ray", MI_TOGGLE_DISPLAY_OF_PICKING_RAY );
  // glutSetMenu( mainMenu );
  // glutAddSubMenu( "Toggle Display of...", displayToggleMenu );
  // glutAddMenuEntry( "Look At Object1 (l)", MI_LOOKAT_OBJ1 );
	// glutAddMenuEntry( "Look At Object2 (k)", MI_LOOKAT_OBJ2 );
  // glutAddMenuEntry( "Reset Camera (r)", MI_RESET_CAMERA );
  // glutAddMenuEntry( "Quit (Esc)", MI_QUIT );
  // glutAttachMenu( GLUT_RIGHT_BUTTON );//attach the menu to the current window

   int width = glutGet( GLUT_WINDOW_WIDTH );
   int height = glutGet( GLUT_WINDOW_HEIGHT );
   camera = new Camera(
      width, height, 8, Point3( 0, 0, 0)
   );

   glutMainLoop();
}




//callbacks

void reshapeCallback( int width, int height ) {
   if ( 0 != camera )
      camera->resizeViewport( width, height );
}



void mouseCallback( int button, int state, int x, int y ) {
   int mod = glutGetModifiers();
   CTRL = mod & GLUT_ACTIVE_CTRL;
   ALT = mod & GLUT_ACTIVE_ALT;
   SHIFT = mod & GLUT_ACTIVE_SHIFT;

   switch ( button ) {
      case GLUT_LEFT_BUTTON : LMB = state == GLUT_DOWN; break;
      case GLUT_MIDDLE_BUTTON : MMB = state == GLUT_DOWN; break;
      case GLUT_RIGHT_BUTTON : RMB = state == GLUT_DOWN; break;
   }

   if ( state == GLUT_DOWN && !(CTRL || ALT) ) {
      pickingRay = camera->computeRay( x, y );
      glutPostRedisplay();
   }

   mouse_x = x;
   mouse_y = y;

   printf("buttons:(%d,%d,%d); modifiers:(%c,%c,%c); position:(%d,%d)\n",
      LMB ? 1 : 0, MMB ? 1 : 0, RMB ? 1 : 0,
      CTRL ? 'C' : '-', ALT ? 'A' : '-', SHIFT ? 'S' : '-',
      mouse_x, mouse_y
   );
}




void passiveMotionCallback( int x, int y ) {
   mouse_x = x;
   mouse_y = y;
   // printf("move:(%d,%d)\n", mouse_x, mouse_y );
}

void motionCallback( int x, int y ) {
   int delta_x = x - mouse_x;
   int delta_y = mouse_y - y;
   int delta = delta_x + delta_y;

   if ( SHIFT ) {
      // The redundant functionality here is for users with 2-button mice.
      if ( LMB ) {
         if ( ALT )
            camera->dollyCameraForward( 3*delta, false );
         else
            camera->translateSceneRightAndUp( delta_x, delta_y );
      }
   }
   else if ( CTRL ) {
      if ( LMB && MMB )
         camera->rollCameraRight( delta );
      else if ( LMB ) {
         camera->pitchCameraUp( delta_y );
         camera->yawCameraRight( delta_x );
      }
      else if ( MMB )
         camera->zoomIn( delta );
   }
   else if ( ALT ) {
      if ( LMB && MMB )
         camera->dollyCameraForward( 3*delta, false );
      else if ( LMB )
         camera->orbit( mouse_x, mouse_y, x, y );
      else if ( MMB )
         camera->translateSceneRightAndUp( delta_x, delta_y );
   }

   glutPostRedisplay();

   mouse_x = x;
   mouse_y = y;
   // printf("drag:(%d,%d)\n", mouse_x, mouse_y );
}




void menuCallback( int menuItem ) {
   switch ( menuItem ) {
      case MI_CYCLE_DRAW_MODE :
         drawMode = (DrawMode)( ( drawMode + 1 ) % NUM_DRAW_MODES );
         glutPostRedisplay();
         break;
		 case MI_SWITCH_PROBLEM1 :
        problem = 0;
        glutPostRedisplay();
        break;
		case MI_SWITCH_PROBLEM2 :
       problem = 1;
       glutPostRedisplay();
       break;
      case MI_TOGGLE_DISPLAY_OF_WORLD_SPACE_AXES :
         drawWorldAxes = ! drawWorldAxes;
         glutPostRedisplay();
         break;
      case MI_TOGGLE_DISPLAY_OF_CAMERA_TARGET :
         drawTarget = ! drawTarget;
         glutPostRedisplay();
         break;
      case MI_TOGGLE_DISPLAY_OF_BACKFACES :
         cullBackfaces = ! cullBackfaces;
         glutPostRedisplay();
         break;
      case MI_TOGGLE_DISPLAY_OF_PICKING_RAY :
         drawPickingRay = ! drawPickingRay;
         glutPostRedisplay();
         break;
      case MI_LOOKAT_OBJ1 :
         camera->lookAt( object1Position );
         glutPostRedisplay();
         break;
		  case MI_LOOKAT_OBJ2 :
         camera->lookAt( object2Position );
         glutPostRedisplay();
         break;
      case MI_RESET_CAMERA :
         camera->reset();
         glutPostRedisplay();
         break;
      case MI_QUIT :
         exit(0);
         break;
      default:
         printf("unknown menu item %d was selected\n", menuItem );
         break;
   }
}




void keyboardCallback( unsigned char key, int x, int y ) {
   switch ( key ) {
      case 'a':
         menuCallback( MI_TOGGLE_DISPLAY_OF_WORLD_SPACE_AXES );
         break;
		 case '1':
        menuCallback( MI_SWITCH_PROBLEM1 );
        break;
		case '2':
       menuCallback( MI_SWITCH_PROBLEM2 );
       break;
      case 'b':
         menuCallback( MI_TOGGLE_DISPLAY_OF_BACKFACES );
         break;
      case 'd':
         menuCallback( MI_CYCLE_DRAW_MODE );
         break;
      case 'l':
         menuCallback( MI_LOOKAT_OBJ1 );
         break;
		 case 'k':
        menuCallback( MI_LOOKAT_OBJ2 );
        break;
      case 'r':
         menuCallback( MI_RESET_CAMERA );
         break;
      case 't':
         menuCallback( MI_TOGGLE_DISPLAY_OF_CAMERA_TARGET );
         break;
      case 27: // Escape
         menuCallback( MI_QUIT );
         break;
      default:
         printf("untrapped key %d\n", (int)key );
         break;
   }
}




void specialCallback( int key, int x, int y ) {
   switch ( key ) {
      // function keys
      case GLUT_KEY_F1 :
      case GLUT_KEY_F2 :
      case GLUT_KEY_F3 :

      // arrow keys
      case GLUT_KEY_LEFT :
      case GLUT_KEY_RIGHT :
      case GLUT_KEY_UP :
      case GLUT_KEY_DOWN :

      default:
         printf("untrapped special key %d\n", (int)key );
         break;
   }
}




void timerCallback( int /* id */ ) {


	 glutPostRedisplay();
   glutTimerFunc( timerInterval, timerCallback, 0 );
}
