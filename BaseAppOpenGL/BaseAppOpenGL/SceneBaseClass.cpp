#include "SceneBaseClass.h"


CSceneBaseClass::CSceneBaseClass(void)
{
	wglSwapIntervalEXT = NULL;
	wglGetSwapIntervalEXT = NULL;
}


CSceneBaseClass::~CSceneBaseClass(void)
{
}

// Teste para suporte de extensões OpenGL
bool CSceneBaseClass::WGLExtensionSupported(const char *extension_name)  
{
	const size_t extlen = strlen(extension_name);
	const char *supported = NULL;

	// Try To Use wglGetExtensionStringARB On Current DC, If Possible
	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

	if (wglGetExtString)
		supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

	// If That Failed, Try Standard Opengl Extensions String
	if (supported == NULL)
		supported = (char*)glGetString(GL_EXTENSIONS);

	// If That Failed Too, Must Be No Extensions Supported
	if (supported == NULL)
		return false;

	// Begin Examination At Start Of String, Increment By 1 On False Match
	for (const char* p = supported;; p++)
	{
		// Advance p Up To The Next Possible Match
		p = strstr(p, extension_name);

		if (p == NULL)
			return false;                       // No Match

		// Make Sure That Match Is At The Start Of The String Or That
		// The Previous Char Is A Space, Or Else We Could Accidentally
		// Match "wglFunkywglExtension" With "wglExtension"
		// Also, Make Sure That The Following Character Is Space Or NULL
		// Or Else "wglExtensionTwo" Might Match "wglExtension"
		if ((p == supported || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' '))
			return true;                        // Match
	}
}

// Habilita a Sincronização Vertical (VSync) (0 - desabilita | 1 - habilita)
void CSceneBaseClass::SetVSync(bool enable) 
{
	if (WGLExtensionSupported("WGL_EXT_swap_control"))
	{    
		// Extension is supported, init pointers.    
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");    

		// this is another function from WGL_EXT_swap_control extension    
		wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC) wglGetProcAddress("wglGetSwapIntervalEXT");
	}

	// Ativa o VSync 1    
	// Desativa o VSync 0
	if(wglSwapIntervalEXT)
		wglSwapIntervalEXT(enable);
}



// Projeção Ortográfica de 3D para 2D.
void CSceneBaseClass::OrthoMode(int left, int top, int right, int bottom)  
{
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glLoadIdentity();

	glOrtho( left, right, bottom, top, 0, 1 );
	
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

// Projeção Perspectiva de 2D para 3D.
void CSceneBaseClass::PerspectiveMode(void)  
{
	glMatrixMode( GL_PROJECTION );

	glPopMatrix();							

	glMatrixMode( GL_MODELVIEW );
}



