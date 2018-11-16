#pragma once
#include "SceneBaseClass.h"

#include "CShaderManager.h"
#include "CTexture.h"
#include "CTimer.h"
#include "CTexto.h"
#include "CCamera.h"
#include "CModel_3DS.h"





class CScene2 : public CSceneBaseClass
{
public:
	CScene2();
	~CScene2(void);

	
	virtual void MouseMove(void);					// Tratamento de movimento do mouse
	virtual void KeyPressed(void);					// Tratamento de teclas pressionadas
	virtual void KeyDownPressed(WPARAM	wParam);	// Tratamento de teclas pressionadas
	virtual int DrawGLScene(void);					// Função que desenha a cena

	void Draw3DSGrid(float width, float length);

private:
	
	bool	keys[256];		// Array usado para rotinas do teclado
	bool	active;			// Window Active Flag Set To TRUE By Default
	bool	fullscreen;		// Exibir janela em modo fullscreem (TRUE) ou em janela (FALSE)
	

	CCamera			*pCamera;	// Gerencia câmera OpenGL
	float fDeltaY;				// Rotação da câmera OpenGL no eixo Y
	float fDeltaX;				// Rotação da câmera OpenGL no eixo X

	CTexto			*pTexto;	// Objeto que gerencia texto
	CTexture		*pTextures;	// Objeto que gerencia texturas
	CTimer			*pTimer;	// Objeto que gerencia o timer
	CShaderManager	*pShaderMgr; // Objeto que gerencia os shaders


	CModel_3DS		*pModel3DS1;
	CModel_3DS		*pModel3DS2;



	int		iFPS;			// FPS and FPS Counter
	int		iFrames;		// FPS and FPS Counter
	DWORD	ulLastFPS;		// FPS and FPS Counter
	char	szTitle[256];	// FPS and FPS Counter


	bool bIsWireframe;	// Modos Wireframe/Solid
	bool bIsCameraFPS;	// Ativa modo de camera First Person Shooter (true) ou Third Person Shooter (false)



	// Definindo as propriedades da fonte de luz
	GLfloat LightAmbient[4];	
	GLfloat LightDiffuse[4];	
	GLfloat LightSpecular[4];
	GLfloat LightPosition[4];
	GLfloat LightDirection[3];
	GLfloat LightDirectionCam[3];

	// Definindo as propriedades do material
	GLfloat MatAmbient[4];	
	GLfloat MatDiffuse[4];	
	GLfloat MatSpecular[4];
	GLfloat MatShininess;

	// Diferença angular entre a borda interna e externa do spot (em graus)
	float borda;

	// Borda externa do spot (em graus)
	float cutoff;

	// Cosseno da borda interna, para ser empregado no fragment shader
	float cosborda;

	// Handle para variável uniform "interna"
	GLint uinterna;

	bool bActiveShaderProgram;
	
};

