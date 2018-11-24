#pragma once
#include "SceneBaseClass.h"
#include "CShaderManager.h"
#include "CTexture.h"
#include "CTimer.h"
#include "CTexto.h"
#include "CCamera.h"
#include "CModel_3DS.h"
#include "Peca.h"


class CScene1 : public CSceneBaseClass
{
public:
	CScene1();
	~CScene1(void);

	
	virtual void MouseMove(void);					// Tratamento de movimento do mouse
	virtual void KeyPressed(void);					// Tratamento de teclas pressionadas
	virtual void KeyDownPressed(WPARAM	wParam);	// Tratamento de teclas pressionadas
	virtual int DrawGLScene(void);					// Função que desenha a cena

	//void Draw3DSGrid(float width, float length);
	void Draw3DSGrid();
	void DrawAxis();
	void DrawPecas();
	void FlipPecas();
	void CountPoints();

	void DrawCircle(GLdouble x, GLdouble y, GLdouble z, GLdouble raio, int num_linhas);
	void DrawCube(float pX = 0.0f, float pY = 0.0f, float pZ = 0.0f,
		float rX = 0.0f, float rY = 0.0f, float rZ = 0.0f, float angle = 0.0f,
		float sX = 1.0f, float sY = 1.0f, float sZ = 1.0f,
	    int texID = -1);

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


	int		iFPS;			// FPS and FPS Counter
	int		iFrames;		// FPS and FPS Counter
	DWORD	ulLastFPS;		// FPS and FPS Counter
	char	szTitle[256];	// FPS and FPS Counter


	bool bIsWireframe;	// Modos Wireframe/Solid
	bool bIsCameraFPS;	// Ativa modo de camera First Person Shooter (true) ou Third Person Shooter (false)

	float fRenderPosY;
	float fTimerPosY;


	// Definindo as propriedades da fonte de luz
	GLfloat LightAmbient[4];
	GLfloat LightDiffuse[4];
	GLfloat LightSpecular[4];
	GLfloat LightPosition[4];

	// Definindo as propriedades do material
	GLfloat MatAmbient[4];
	GLfloat MatDiffuse[4];
	GLfloat MatSpecular[4];
	GLfloat MatShininess;

	float fPosX;
	float fPosY;
	float fPosZ;
	float fMovementFactor;

	GLUquadric *sphere1;

	CModel_3DS* pModel3DS_1;
	CModel_3DS* pModel3DS_2;
	CModel_3DS* pModel3DS_3;
	
	float angulo;

	Peca* pecas[8][8];
	bool visible[8][8];
	float posC, posR;
	int C, R;
	float colors[8][8];
	float player;

	int pBlackPoints, pWhitePoints;
	int winner;
};

