#pragma once
#include <windows.h>			// Header File For Windows
#include <glew.h>
#include <gl.h>				// Header File For The OpenGL32 Library
#include <glu.h>				// Header File For The GLu32 Library
#include <glut.h>				// Header File For The GLut32 Library
#include <glaux.h>				// Header File For The Glaux Library
#include <glext.h>
#include <wglext.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include "Defines.h"



class CSceneBaseClass
{
public:
	CSceneBaseClass(void);
	virtual ~CSceneBaseClass(void);

	virtual void MouseMove(void)  = 0;		// Tratamento de movimento do mouse
	virtual void KeyPressed(void) = 0;	// Tratamento de teclas pressionadas
	virtual void KeyDownPressed(WPARAM wParam) = 0;	// Tratamento de teclas pressionadas
	virtual int DrawGLScene(void) = 0;	// Função que desenha a cena

	bool WGLExtensionSupported(const char *extension_name); // Teste para suporte de extensões OpenGL
	void SetVSync(bool enable); // Habilita a Sincronização Vertical (VSync) (0 - desabilita | 1 - habilita)

	void OrthoMode(int left, int top, int right, int bottom);  // Projeção Ortográfica de 3D para 2D.
	void PerspectiveMode(void); //	Projeção Perspectiva de 2D para 3D.

	void Draw3DSGrid(float width, float length); //	Cria um grid horizontal ao longo dos eixos X e Z


private:
	PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT;		// VSync extension
	PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT;	// VSync extension
};

