////////////////////////////////////////////////////////////////////////
// The scene class contains all the parameters needed to define and
// draw a simple scene, including:
//   * Geometry
//   * Light parameters
//   * Material properties
//   * Viewport size parameters
//   * Viewing transformation values
//   * others ...
//
// Some of these parameters are set when the scene is built, and
// others are set by the framework in response to user mouse/keyboard
// interactions.  All of them can be used to draw the scene.

#include "math.h"
#include <fstream>
#include <stdlib.h>

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
using namespace gl;

#include <freeglut.h>
#include <glu.h>                // For gluErrorString

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/glm.hpp>
using namespace glm;

#include "shader.h"
#include "shapes.h"
#include "scene.h"
#include "object.h"
#include "texture.h"
#include "transform.h"

const float PI = 3.14159f;
const float rad = PI/180.0f;    // Convert degrees to radians

MAT4 Identity;

const float grndSize = 100.0;    // Island radius;  Minimum about 20;  Maximum 1000 or so
const int   grndTiles = int(grndSize);
const float grndOctaves = 4.0;  // Number of levels of detail to compute
const float grndFreq = 0.03f;    // Number of hills per (approx) 50m
const float grndPersistence = 0.03f; // Terrain roughness: Slight:0.01  rough:0.05
const float grndLow = -3.0;         // Lowest extent below sea level
const float grndHigh = 5.0;        // Highest extent above sea level

////////////////////////////////////////////////////////////////////////
// This macro makes it easy to sprinkle checks for OpenGL errors
// throughout your code.  Most OpenGL calls can record errors, and a
// careful programmer will check the error status *often*, perhaps as
// often as after every OpenGL call.  At the very least, once per
// refresh will tell you if something is going wrong.
#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { fprintf(stderr, "OpenGL error (at line scene.cpp:%d): %s\n", __LINE__, gluErrorString(err)); exit(-1);} }

// Create an RGB color from human friendly parameters: hue, saturation, value
vec3 HSV2RGB(const float h, const float s, const float v)
{
    if (s == 0.0)
        return vec3(v,v,v);

    int i = (int)(h*6.0) % 6;
    float f = (h*6.0f) - i;
    float p = v*(1.0f - s);
    float q = v*(1.0f - s*f);
    float t = v*(1.0f - s*(1.0f-f));
    if      (i == 0)     return vec3(v,t,p);
    else if (i == 1)  return vec3(q,v,p);
    else if (i == 2)  return vec3(p,v,t);
    else if (i == 3)  return vec3(p,q,v);
    else if (i == 4)  return vec3(t,p,v);
    else   /*i == 5*/ return vec3(v,p,q);
}

////////////////////////////////////////////////////////////////////////
// Constructs a hemisphere of spheres of varying hues
Object* SphereOfSpheres(Shape* SpherePolygons)
{
    Object* ob = new Object(NULL, nullId);
    
    for (float angle=0.0;  angle<360.0;  angle+= 18.0)
        for (float row=0.075f;  row<PI/2.0f;  row += PI/2.0f/6.0f) {   
            vec3 hue = HSV2RGB(angle/360.0f, 1.0f-2.0f*row/PI, 1.0f);

            Object* sp = new Object(SpherePolygons, spheresId,
                                    hue, vec3(1.0, 1.0, 1.0), 120.0);
            float s = sin(row);
            float c = cos(row);
            ob->add(sp, Rotate(2,angle)*Translate(c,0,s)*Scale(0.075f*c,0.075f*c,0.075f*c));
        }
    return ob;
}

////////////////////////////////////////////////////////////////////////
// Constructs a -1...+1  quad (canvas) framed by four (elongated) boxes
Object* FramedPicture(const MAT4& modelTr, const int objectId, 
                      Shape* BoxPolygons, Shape* QuadPolygons , unsigned int _textureId = 0)
{
    // This draws the frame as four (elongated) boxes of size +-1.0
    float w = 0.05f;             // Width of frame boards.
    
    Object* frame = new Object(NULL, nullId);
    Object* ob;
    
    vec3 woodColor(87.0f/255.0f,51.0f/255.0f,35.0f/255.0f);
    ob = new Object(BoxPolygons, frameId,
                    woodColor, vec3(0.2f, 0.2f, 0.2f), 10.0f);
    frame->add(ob, Translate(0.0f, 0.0f, 1.0f+w)*Scale(1.0f, w, w));
    frame->add(ob, Translate(0.0f, 0.0f, -1.0f-w)*Scale(1.0f, w, w));
    frame->add(ob, Translate(1.0f+w, 0.0f, 0.0f)*Scale(w, w, 1.0f+2.0f*w));
    frame->add(ob, Translate(-1.0f-w, 0.0f, 0.0f)*Scale(w, w, 1.0f+2.0f*w));

    ob = new Object(QuadPolygons, objectId,
                    woodColor, vec3(0.0f, 0.0f, 0.0f), 10.0f);
    frame->add(ob, Rotate(0,90.0f));

	ob->textureId = _textureId;

    return frame;
}

////////////////////////////////////////////////////////////////////////
// A callback procedure, called regularly to update the atime global
// variable.
float atime = 0.0f;
void animate(int value)
{
    atime = 360.0f*glutGet((GLenum)GLUT_ELAPSED_TIME)/36000.0f;
    glutPostRedisplay();

    // Schedule next call to this function
    glutTimerFunc(30, animate, 1);
}

////////////////////////////////////////////////////////////////////////
// InitializeScene is called once during setup to create all the
// textures, shape VAOs, and shader programs as well as setting a
// number of other parameters.
void Scene::InitializeScene()
{
    glEnable(GL_DEPTH_TEST);
    CHECKERROR;

    // FIXME: This is a good place for initializing the transformation
    // values.
	
	spin = 0.0f;
	tilt = 20.0f;
	tx = 0.0f;
	ty = 0.0f;
	zoom = 25.0f;
	ry = 0.5f;
	rx = ry * (float)width / (float)height;
	front = 0.1f;
	back = 1000.0f;

	eye = { 0,-16,3 };
	speed = 0.01f;
	time_since_last_refresh = 0;
	project = 1;

	wDown = false;
	sDown = false;
	aDown = false;
	dDown = false;

    CHECKERROR;
    objectRoot = new Object(NULL, nullId);
    
    // Set the initial light position parammeters
    lightSpin = 150.0;
    lightTilt = -45.0;
    lightDist = 100.0;

    // Enable OpenGL depth-testing
    glEnable(GL_DEPTH_TEST);

    // FIXME:  Change false to true to randomize the central object position.
    ground =  new ProceduralGround(grndSize, 4*grndTiles, grndOctaves, grndFreq,
                                   grndPersistence, grndLow, grndHigh, false);

    basePoint = ground->highPoint;

    // Create the lighting shader program from source code files.
    lightingProgram = new ShaderProgram();
#if   defined(PHONG_SOLUTION)
    lightingProgram->AddShader("lightingPhong.vert", GL_VERTEX_SHADER);
    lightingProgram->AddShader("lightingPhong.frag", GL_FRAGMENT_SHADER);
#else
    lightingProgram->AddShader("lighting.vert", GL_VERTEX_SHADER);
    lightingProgram->AddShader("lighting.frag", GL_FRAGMENT_SHADER);
#endif

    glBindAttribLocation(lightingProgram->programId, 0, "vertex");
    glBindAttribLocation(lightingProgram->programId, 1, "vertexNormal");
    glBindAttribLocation(lightingProgram->programId, 2, "vertexTexture");
    glBindAttribLocation(lightingProgram->programId, 3, "vertexTangent");
    lightingProgram->LinkProgram();


    // Create all the Polygon shapes
    Shape* TeapotPolygons =  new Teapot(12);
    Shape* BoxPolygons = new Ply("box.ply");
    Shape* SpherePolygons = new Sphere(32);
    Shape* RoomPolygons = new Ply("room.ply");
    Shape* GroundPolygons = ground;
    Shape* QuadPolygons = new Quad();
    Shape* SeaPolygons = new Plane(2000.0f, 50);

    // Various colors used in the subsequent models
    vec3 woodColor(87.0f/255.0f, 51.0f/255.0f, 35.0f/255.0f);
    vec3 brickColor(134.0f/255.0f, 60.0f/255.0f, 56.0f/255.0f);
    vec3 brassColor(0.5f, 0.5f, 0.1f);
    vec3 grassColor(62.0f/255.0f, 102.0f/255.0f, 38.0f/255.0f);
    vec3 waterColor(0.3f, 0.3f, 1.0f);
 
    // Creates all the models from which the scene is composed.  Each
    // is created with a polygon shape (possibly NULL), a
    // transformation, and the surface lighting parameters Kd, Ks, and
    // alpha.
    Object* central    = new Object(NULL, nullId);
    Object* anim       = new Object(NULL, nullId);
    Object* room       = new Object(RoomPolygons, roomId, brickColor, vec3(0.03f, 0.03f, 0.03f), 1);
    Object* teapot     = new Object(TeapotPolygons, teapotId, brassColor, vec3(0.03f, 0.03f, 0.03f), 120);
    Object* podium     = new Object(BoxPolygons, boxId, vec3(woodColor), vec3(0.03f, 0.03f, 0.03f), 10); 
    Object* sky        = new Object(SpherePolygons, skyId, vec3(3.0f,3.0f,3.0f), vec3(3.0f,3.0f,3.0f), 1000000);
    Object* ground     = new Object(GroundPolygons, groundId, grassColor, vec3(0.03f, 0.03f, 0.03f), 1);
    Object* sea        = new Object(SeaPolygons, seaId, waterColor, vec3(0.03f, 0.03f, 0.03f), 120);
    Object* spheres    = SphereOfSpheres(SpherePolygons);
    Object* leftFrame  = FramedPicture(Identity, lPicId, BoxPolygons, QuadPolygons);
    //Object* rightFrame = FramedPicture(Identity, rPicId, BoxPolygons, QuadPolygons); 

    // FIXME: This is where you could read in all the textures and
    // associate them with the various objects just created above.
	Texture groundTexture("textures/grass.jpg");
	Texture wallTexture("textures/Standard_red_pxr128.png");
	Texture podiumTexture("textures/Brazilian_rosewood_pxr128.png");
	Texture teapotTexture("textures/chinese.jpg");
	Texture pictureTexture("textures/claude-comair-digipen-founder-and-president-our-history.jpg");
	Texture skyTexture("textures/sky.jpg");

	ground->textureId = groundTexture.textureId;
	room->textureId = wallTexture.textureId;
	podium->textureId = podiumTexture.textureId;
	teapot->textureId = teapotTexture.textureId;
	sky->textureId = skyTexture.textureId;

	Object* rightFrame = FramedPicture(Identity, rPicId, BoxPolygons, QuadPolygons, pictureTexture.textureId);
	

    // Scene is composed of sky, ground, sea, room and some central models
    objectRoot->add(sky, Scale(2000.0, 2000.0, 2000.0));
    objectRoot->add(ground);
    objectRoot->add(room, Translate(basePoint.x, basePoint.y, basePoint.z));
    objectRoot->add(sea);
    objectRoot->add(central);

    // Central model has a rudimentary animation (constant rotation on Z)
    animated.push_back(anim);

    // Central contains a teapot on a podium and an external sphere of spheres
    central->add(podium, Translate(0.0f, 0.0f,0.0f));
    central->add(anim, Translate(0.0f, 0.0f,0.0f));
    anim->add(teapot, Translate(0.1f, 0.0f, 1.5f)*TeapotPolygons->modelTr);
    anim->add(spheres, Translate(0.0f, 0.0f, 0.0f)*Scale(30.0f, 30.0f, 30.0f));
    
    // Room contains two framed pictures
    room->add(leftFrame, Translate(-1.5f, 9.85f, 1.0f)*Scale(0.8f, 0.8f, 0.8f));
    room->add(rightFrame, Translate( 1.5f, 9.85f, 1.0f)*Scale(0.8f, 0.8f, 0.8f));

    // Schedule first timed animation call
    glutTimerFunc(30, animate, 1);

    CHECKERROR;
}

////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be
// drawn. (Which is often: 30 to 60 times per second are the common
// goals.)
void Scene::DrawScene()
{
	rx = ry * (float)width / (float)height;

    // Calculate the light's position.
    float lPos[4] = {
       basePoint.x+lightDist*cos(lightSpin*rad)*sin(lightTilt*rad), 
       basePoint.y+lightDist*sin(lightSpin*rad)*sin(lightTilt*rad), 
       basePoint.z+lightDist*cos(lightTilt*rad), 
       1.0 };

    // Update position of any continuously animating objects
    for (std::vector<Object*>::iterator m=animated.begin();  m<animated.end();  m++)
        (*m)->animTr = Rotate(2, atime);

    // Compute Viewing and Perspective transformations.
    MAT4 WorldProj, WorldView, WorldInverse;

    // FIXME: When you are ready to try interactive viewing, replace
    // the following hardcoded values for WorldProj and WorldView with
    // transformation matrices calculated from variables such as spin, 
    // tilt, tr, basePoint, ry, front, and back.

	if (project == 1 || project ==3)
	{
		WorldView = Translate(tx, ty, -(zoom)) * Rotate(0, tilt - 90.0f) * Rotate(2, spin);
		
	}
	else if (project == 2)
	{
	
		float step = speed * (glutGet((GLenum)GLUT_ELAPSED_TIME) - time_since_last_refresh);
		time_since_last_refresh = glutGet((GLenum)GLUT_ELAPSED_TIME);

		const float pi = 3.14159f;
		float radian = (spin * pi) / 180;

		if (wDown)
		{
			eye += step * vec3(sin(radian), cos(radian), 0.0);
		}
		if (sDown)
		{
			eye -= step * vec3(sin(radian), cos(radian), 0.0);

		}
		if (aDown)
		{
			eye -= step * vec3(cos(radian), -sin(radian), 0.0);
			
		}
		if (dDown)
		{
			eye += step * vec3(cos(radian), -sin(radian), 0.0);
		}
		WorldView = Rotate(0, tilt - 90.0f) * Rotate(2, spin) * Translate(-eye.x,-eye.y,-(ground->HeightAt(eye.x, eye.y)+2.0f));
		
	}
	WorldProj = Perspective(rx, ry, front, back);

   /* WorldProj[0][0]=  2.368f;
    WorldProj[0][1]= -0.800f;
    WorldProj[0][2]=  0.000f;
    WorldProj[0][3]=  0.000f;
    WorldProj[1][0]=  0.384f;
    WorldProj[1][1]=  1.136f;
    WorldProj[1][2]=  2.194f;
    WorldProj[1][3]=  0.000f;
    WorldProj[2][0]=  0.281f;
    WorldProj[2][1]=  0.831f;
    WorldProj[2][2]= -0.480f;
    WorldProj[2][3]= 42.451f;
    WorldProj[3][0]=  0.281f;
    WorldProj[3][1]=  0.831f;
    WorldProj[3][2]= -0.480f;
    WorldProj[3][3]= 43.442f;*/
    
    /*WorldView[0][3]= -(basePoint[0]);
    WorldView[1][3]= -(basePoint[1]);
    WorldView[2][3]= -(basePoint[2]);*/


    // The lighting algorithm needs the inverse of the WorldView matrix
    invert(&WorldView, &WorldInverse);

    ////////////////////////////////////////////////////////////////////////////////
    // Anatomy of a pass:
    //   Choose a shader  (create the shader in InitializeScene above)
    //   Choose and FBO/Render-Target (if needed; create the FBO in InitializeScene above)
    //   Set the viewport (to the pixel size of the screen or FBO)
    //   Clear the screen.
    //   Set the uniform variables required by the shader
    //   Draw the geometry
    //   Unset the FBO (if one was used)
    //   Unset the shader
    ////////////////////////////////////////////////////////////////////////////////

    int loc, programId;

    ////////////////////////////////////////////////////////////////////////////////
    // Lighting pass
    ////////////////////////////////////////////////////////////////////////////////
    
    // Choose the lighting shader
    lightingProgram->Use();
    programId = lightingProgram->programId;

    // Set the viewport, and clear the screen
    glViewport(0, 0, width, height);
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    
    // Set the uniform variables required by the shader (e.g., perspective and viewing matrices)
    loc = glGetUniformLocation(programId, "WorldProj");
    glUniformMatrix4fv(loc, 1, GL_TRUE, WorldProj.Pntr());
    loc = glGetUniformLocation(programId, "WorldView");
    glUniformMatrix4fv(loc, 1, GL_TRUE, WorldView.Pntr());
    loc = glGetUniformLocation(programId, "WorldInverse");
    glUniformMatrix4fv(loc, 1, GL_TRUE, WorldInverse.Pntr());
    loc = glGetUniformLocation(programId, "lightPos");
    glUniform3fv(loc, 1, &(lPos[0]));  
    loc = glGetUniformLocation(programId, "mode");
    glUniform1i(loc, mode);
    CHECKERROR;

    // Draw all objects (This recursively traverses the object hierarchy.)
    objectRoot->Draw(lightingProgram, Identity);
    CHECKERROR; 

    
    // Turn off the shader
    lightingProgram->Unuse();

    ////////////////////////////////////////////////////////////////////////////////
    // End of Lighting pass
    ////////////////////////////////////////////////////////////////////////////////
}
