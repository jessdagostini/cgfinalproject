#include "CTexture.h"
#pragma warning(disable:4996)

CTexture::CTexture()
{
	for (int i = 0; i < NUM_TEXTURES; i++)
		this->textureArray[i] = 0;
}

CTexture::~CTexture()
{
	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		if (glIsTexture(this->textureArray[i]))
		{
			glDeleteTextures(1, &this->textureArray[i]);
			this->textureArray[i] = 0;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		if (glIsTexture(this->cubeTextureArray[i]))
		{
			glDeleteTextures(1, &this->cubeTextureArray[i]);
			this->cubeTextureArray[i] = 0;
		}
	}
}


LPSTR CTexture::AddPath(LPSTR textureName)
{
	string textureNameAux = textureName;
	string strPath = "./Texturas/" + textureNameAux;	
	this->finalPath = const_cast<LPSTR>(strPath.c_str()); 

	return finalPath;
}

bool CTexture::CreateTextureMipMap(unsigned int textureID, LPSTR strFileName)
{
	ILuint imageID;				// Create an image ID as a ULuint
	ILboolean success;			// Create a flag to keep track of success/failure
	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID
	ilBindImage(imageID); 			// Bind the image

	success = ilLoadImage(strFileName); 	// Load the image file

											// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			MessageBox(NULL, iluErrorString(error), "DEVIL ERROR", MB_OK | MB_ICONINFORMATION);
			exit(-1);
		}

		// Generate a new texture
		glGenTextures(1, &textureArray[textureID]);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Set texture interpolation method to use linear interpolation and (MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

		gluBuild2DMipmaps(GL_TEXTURE_2D,
			3,
			ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT),
			ilGetInteger(IL_IMAGE_FORMAT),
			GL_UNSIGNED_BYTE,
			ilGetData());

	}
	else // If we failed to open the image file in the first place...
	{
		error = ilGetError();
		MessageBox(NULL, iluErrorString(error), "DEVIL ERROR", MB_OK | MB_ICONINFORMATION);
		exit(-1);
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	return true; // Return the GLuint to the texture so you can use it!
}


bool CTexture::CreateTextureLinear(unsigned int textureID, LPSTR strFileName)
{
	ILuint imageID;				// Create an image ID as a ULuint
	ILboolean success;			// Create a flag to keep track of success/failure
	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID
	ilBindImage(imageID); 			// Bind the image

	success = ilLoadImage(strFileName); 	// Load the image file

											// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			MessageBox(NULL, iluErrorString(error), "DEVIL ERROR", MB_OK | MB_ICONINFORMATION);
			exit(-1);
		}

		// Generate a new texture
		glGenTextures(1, &textureArray[textureID]);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);


		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 		// Type of texture
			0,								// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,								// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,				// Image data type
			ilGetData());					// The actual image data itself

	}
	else // If we failed to open the image file in the first place...
	{
		error = ilGetError();
		MessageBox(NULL, iluErrorString(error), "DEVIL ERROR", MB_OK | MB_ICONINFORMATION);
		exit(-1);
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	return true; // Return the GLuint to the texture so you can use it!
}


bool CTexture::CreateTextureTGA(unsigned int textureID, LPSTR strFileName)
{
	ILuint imageID;				// Create an image ID as a ULuint
	ILboolean success;			// Create a flag to keep track of success/failure
	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID
	ilBindImage(imageID); 			// Bind the image

	success = ilLoadImage(strFileName); 	// Load the image file

											// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			MessageBox(NULL, iluErrorString(error), "DEVIL ERROR", MB_OK | MB_ICONINFORMATION);
			exit(-1);
		}

		// Generate a new texture
		glGenTextures(1, &textureArray[textureID]);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);


		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 		// Type of texture
			0,								// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,								// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,				// Image data type
			ilGetData());					// The actual image data itself

	}
	else // If we failed to open the image file in the first place...
	{
		error = ilGetError();
		MessageBox(NULL, iluErrorString(error), "DEVIL ERROR", MB_OK | MB_ICONINFORMATION);
		exit(-1);
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	return true; // Return the GLuint to the texture so you can use it!
}



bool CTexture::CreateTextureClamp(unsigned int textureID, LPSTR strFileName)
{
	ILuint imageID;				// Create an image ID as a ULuint
	ILboolean success;			// Create a flag to keep track of success/failure
	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID
	ilBindImage(imageID); 			// Bind the image

	success = ilLoadImage(strFileName); 	// Load the image file

											// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			MessageBox(NULL, iluErrorString(error), "DEVIL ERROR", MB_OK | MB_ICONINFORMATION);
			exit(-1);
		}

		// Generate a new texture
		glGenTextures(1, &textureArray[textureID]);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);


		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Set texture interpolation method to use linear interpolation and (MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

		gluBuild2DMipmaps(GL_TEXTURE_2D,
			3,
			ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT),
			ilGetInteger(IL_IMAGE_FORMAT),
			GL_UNSIGNED_BYTE,
			ilGetData());

	}
	else // If we failed to open the image file in the first place...
	{
		error = ilGetError();
		MessageBox(NULL, iluErrorString(error), "DEVIL ERROR", MB_OK | MB_ICONINFORMATION);
		exit(-1);
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	return true; // Return the GLuint to the texture so you can use it!
}

bool CTexture::CreateTextureNearest(unsigned int textureID, LPSTR strFileName)
{
	ILuint imageID;				// Create an image ID as a ULuint
	ILboolean success;			// Create a flag to keep track of success/failure
	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID
	ilBindImage(imageID); 			// Bind the image

	success = ilLoadImage(strFileName); 	// Load the image file

											// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			MessageBox(NULL, iluErrorString(error), "DEVIL ERROR", MB_OK | MB_ICONINFORMATION);
			exit(-1);
		}

		// Generate a new texture
		glGenTextures(1, &textureArray[textureID]);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);


		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 		// Type of texture
			0,								// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,								// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,				// Image data type
			ilGetData());					// The actual image data itself

	}
	else // If we failed to open the image file in the first place...
	{
		error = ilGetError();
		MessageBox(NULL, iluErrorString(error), "DEVIL ERROR", MB_OK | MB_ICONINFORMATION);
		exit(-1);
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	return true; // Return the GLuint to the texture so you can use it!
}


bool CTexture::CreateTextureCubeMap(LPSTR* cubeMap)
{
	for (int i = 0; i < 6; i++)
	{
		if (!cubeMap[i])									// Return from the function if no file name was passed in
			return false;

		ILuint imageID;				// Create an image ID as a ULuint
		ILboolean success;			// Create a flag to keep track of success/failure
		ILenum error;				// Create a flag to keep track of the IL error state

		ilGenImages(1, &imageID); 		// Generate the image ID
		ilBindImage(imageID); 			// Bind the image

		success = ilLoadImage(cubeMap[i]); 	// Load the image file

												// If we managed to load the image, then we can start to do things with it...
		if (success)
		{
			// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
			ILinfo ImageInfo;
			iluGetImageInfo(&ImageInfo);
			if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			{
				iluFlipImage();
			}

			// Convert the image into a suitable format to work with
			// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

			// Quit out if we failed the conversion
			if (!success)
			{
				error = ilGetError();
				MessageBox(NULL, iluErrorString(error), "DEVIL ERROR", MB_OK | MB_ICONINFORMATION);
				exit(-1);
			}

			// Generate a new texture
			glGenTextures(1, &cubeTextureArray[i]);

			// Bind the texture to a name
			glBindTexture(faceTarget[i], cubeTextureArray[i]);


			// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR is the smoothest.	
			glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// Specify the texture specification
			glTexImage2D(GL_TEXTURE_2D, 		// Type of texture
				0,								// Pyramid level (for mip-mapping) - 0 is the top level
				ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
				ilGetInteger(IL_IMAGE_WIDTH),	// Image width
				ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
				0,								// Border width in pixels (can either be 1 or 0)
				ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
				GL_UNSIGNED_BYTE,				// Image data type
				ilGetData());					// The actual image data itself

		}
		else // If we failed to open the image file in the first place...
		{
			error = ilGetError();
			MessageBox(NULL, iluErrorString(error), "DEVIL ERROR", MB_OK | MB_ICONINFORMATION);
			exit(-1);
		}

		ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.
	}

	return true;
}

void CTexture::ApplyTexture(unsigned int ID)
{
	glBindTexture(GL_TEXTURE_2D, textureArray[ID]);
}

void CTexture::DeleteTexture(unsigned int ID)
{
	if (glIsTexture(this->textureArray[ID]))
	{
		glDeleteTextures(1, &this->textureArray[ID]);
		this->textureArray[ID] = 0;
	}
}

void CTexture::ApplySkyBoxTexture(unsigned int textureID)
{
	glBindTexture(GL_TEXTURE_2D, this->cubeTextureArray[textureID]);
}


void CTexture::BeginSphereMapping(unsigned int textureID)
{
	// Sphere Mapping
	this->ApplyTexture(textureID);
	//Sphere mapping and enable s & t texture generation
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
}



void CTexture::EndSphereMapping()
{
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}


void CTexture::BeginCubeMapping()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, this->cubeTextureArray[5]);
	glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, this->cubeTextureArray[4]);
	glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, this->cubeTextureArray[2]);
	glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, this->cubeTextureArray[3]);
	glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, this->cubeTextureArray[0]);
	glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, this->cubeTextureArray[1]);

	glEnable(GL_TEXTURE_CUBE_MAP);

//GL_REFLECTION_MAP for s,t,r texture coordinates
   glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
   glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
   glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

   glEnable(GL_TEXTURE_GEN_S);
   glEnable(GL_TEXTURE_GEN_T);
   glEnable(GL_TEXTURE_GEN_R);
}

void CTexture::EndCubeMapping()
{

	glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);

	glDisable(GL_TEXTURE_CUBE_MAP);
}