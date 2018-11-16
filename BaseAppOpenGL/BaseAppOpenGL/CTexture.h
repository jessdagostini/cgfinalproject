#pragma once

#include <windows.h>	// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <glext.h>		// Header File For The Glext Library
#include <string>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

using namespace std;

#define NUM_TEXTURES 50
#define MAX_TEXTURE_MIPS 14

static GLenum faceTarget[6] = {
  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};



class CTexture
{
public:
	CTexture();
	~CTexture();


	bool CreateTextureLinear(unsigned int textureID, LPSTR strFileName);
	bool CreateTextureMipMap(unsigned int textureID, LPSTR strFileName);
	bool CreateTextureClamp(unsigned int textureID, LPSTR strFileName);
	bool CreateTextureNearest(unsigned int textureID, LPSTR strFileName);

	bool CreateTextureCubeMap(LPSTR* cubeMap);
	bool CreateTextureTGA(unsigned int textureID, LPSTR strFileName);

	void ApplyTexture(unsigned int ID);
	void DeleteTexture(unsigned int ID);

	void BeginSphereMapping(unsigned int textureID);
	void EndSphereMapping();

	void BeginCubeMapping();
	void EndCubeMapping();

	void ApplySkyBoxTexture(unsigned int textureID);
	
	

	

private:

	GLuint	textureArray[NUM_TEXTURES];		// Storage For NUM_TEXTURES Textures
	GLuint	cubeTextureArray[6];
	LPSTR finalPath;

	LPSTR AddPath(LPSTR textureName);

	// this variable is used for image series
	int savedImages;

};



/*-----------------------------------------------------------
This is a very simple TGA lib. It will only load and save
uncompressed images in greyscale, RGB or RGBA mode.

If you want a more complete lib I suggest you take
a look at Paul Groves' TGA loader. Paul's home page is at

http://paulyg.virtualave.net


Just a little bit about the TGA file format.

Header - 12 fields


id						unsigned char
colour map type			unsigned char
image type				unsigned char

1	-	colour map image
2	-	RGB(A) uncompressed
3	-	greyscale uncompressed
9	-	greyscale RLE (compressed)
10	-	RGB(A) RLE (compressed)

colour map first entry	short int
colour map length		short int
map entry size			short int

horizontal origin		short int
vertical origin			short int
width					short int
height					short int
pixel depth				unsigned char

8	-	greyscale
24	-	RGB
32	-	RGBA

image descriptor		unsigned char

From all these fields, we only care about the image type,
to check if the image is uncompressed and not color indexed,
the width and height, and the pixel depth.

You may use this library for whatever you want. This library is
provide as is, meaning that I won't take any responsability for
any damages that you may incur for its usage.

Antonio Ramires Fernandes ajbrf@yahoo.com
-------------------------------------------------------------*/