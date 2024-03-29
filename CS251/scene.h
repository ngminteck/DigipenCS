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

#include "shapes.h"
#include "object.h"
#include "texture.h"
#include "fbo.h"

enum ObjectIds {
    nullId	= 0,
    skyId	= 1,
    seaId	= 2,
    groundId	= 3,
    roomId	= 4,
    boxId	= 5,
    frameId	= 6,
    lPicId	= 7,
    rPicId	= 8,
    teapotId	= 9,
    spheresId	= 10,
};

class Shader;

class Scene
{
public:
    // Viewing transformation parameters (suggested) FIXME: This is a
    // good place for the transformation values which are set by the
    // user mouse/keyboard actions and used in DrawScene to create the
    // transformation matrices.

	float spin;
	float tilt;

	float tx;
	float ty;
	float zoom;

	float rx;
	float ry;
	float front;
	float back;

	vec3 eye;
	float speed;

	bool wDown ;
	bool sDown ;
	bool aDown ;
	bool dDown ;

	int project;
	int time_since_last_refresh;


    ProceduralGround* ground;

    // Light position parameters
    float lightSpin, lightTilt, lightDist;

    vec3 basePoint;  // Records where the scene building is centered.
    int mode; // Extra mode indicator hooked up to number keys and sent to shader
    
    // Viewport
    int width, height;

    // All objects in the scene are children of this single root object.
    Object* objectRoot;
    std::vector<Object*> animated;

    // Shader programs
    ShaderProgram* lightingProgram;


    //void append(Object* m) { objects.push_back(m); }

    void InitializeScene();
    void DrawScene();

};
