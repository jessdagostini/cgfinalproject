#ifndef CAMERA_H
#define CAMERA_H

/*
All the Win/OpenGL stuff.. 
if youre not using Windows you most likely know what goes here
*/
#ifdef _WIN32
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl.h>
#endif

/*
Usage:
You can access the Right/Up/Forward vectors like a float[3] array,
but cannot write to them, because that would screw up the matrix.
Same goes for Position, except its safe to write to it.

setView() has to be called before you draw anything.
Just call it instead of gluLookAt (which most are using)

move and rotate come in two versions. Loc means the transformation
is in local coords, so rotating around (1,0,0) means youre rotating
around your current Right-vector while Glob would rotate around the
global x-axis.

Most likely you will use Loc for controlling the CCamera, though Glob
can be usefull if you need to apply physics. Also walking characters
will usually rotate around the global rather than around their local Up,
while flying objects will always use local axes.

If talking about objects when this is a CCamera confuses you: if you drop
the setView() method you can use this for objects in your world too. Just
rename the class to Object3D or something and derive a CCamera class from it.
*/
class CCamera {
public:
	float const *Right, *Up, *Forward;
	float *Position;
private:
	float Transform[16];
	float velocity;

public:
	CCamera(float x=0.0f, float y=0.0f, float z=0.0f, float camVelocity = 1.0f);
	~CCamera();

	void setView();
	void moveLoc(float x, float y, float z);
	void moveGlob(float x, float y, float z);
	void rotateLoc(float deg, float x, float y, float z);
	void rotateGlob(float deg, float x, float y, float z);
	void setCameraVelocity(float v);
	float getCameraVelocity();
};

#endif

