#include "Scene2.h"


CScene2::CScene2()
{

	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;
	pShaderMgr = NULL;

	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;


	// Cria gerenciador de impressão de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(0.0f, 10.0f, 30.0f);

	// Cria o Timer
	pTimer = new CTimer();

	// Carrega todas as texturas
	pTextures = new CTexture();
	pTextures->CreateTextureMipMap(0, "../Scene2/grass.bmp");


	// Carrega os Shader Programs
	pShaderMgr = new CShaderManager();
	pShaderMgr->InitShaderManager("../Scene2/SpotLightShader.vert",  "../Scene2/SpotLightShader.frag");
	bActiveShaderProgram = false;

	LightAmbient[0] = 0.0f;		LightAmbient[1] = 0.0f;		LightAmbient[2] = 0.0f;	LightAmbient[3] = 1.0f;
	LightDiffuse[0] = 1.0f;		LightDiffuse[1] = 1.0f;		LightDiffuse[2] = 1.0f;		LightDiffuse[3] = 1.0f;
	LightSpecular[0] = 1.0f;	LightSpecular[1] = 1.0f;	LightSpecular[2] = 1.0f;	LightSpecular[3] = 1.0f;
	LightPosition[0] = 0.0f;	LightPosition[1] = 10.0f;	LightPosition[2] = 0.0f;	LightPosition[3] = 1.0f;
	LightDirection[0] = 0.0f;	LightDirection[1] = -1.0f;	LightDirection[2] = 0.0f;

	MatAmbient[0] = 0.0f;	MatAmbient[1] = 0.0f;	MatAmbient[2] = 0.0f;	MatAmbient[3] = 1.0f;
	MatDiffuse[0] = 1.0f;	MatDiffuse[1] = 1.0f;	MatDiffuse[2] = 1.0f;	MatDiffuse[3] = 1.0f;
	MatSpecular[0] = 1.0f;	MatSpecular[1] = 1.0f;	MatSpecular[2] = 1.0f;	MatSpecular[3] = 1.0f;
	MatShininess = 128.0f;



	borda = 10.0f;
	cutoff = 45.0f;
	// Armazena posição da variável "interna"
	uinterna = glGetUniformLocation(pShaderMgr->id(0), "interna");
	// Inicializa o valor de cosborda
	cosborda = cos((cutoff-borda) * GL_PI / 180.0f);


	// Criando a SpotLight
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, borda);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, LightDirection);


	// Aplicando Material 1 aos objetos
	glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, MatShininess);



	// Carrega Objetos da  Cena
	pModel3DS1 = new CModel_3DS();
	pModel3DS1->Load("../Scene2/Caixas.3ds");

	pModel3DS2 = new CModel_3DS();
	pModel3DS2->Load("../Scene2/Teapot.3ds");

}


CScene2::~CScene2(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}

	if (pShaderMgr)
	{
		delete pShaderMgr;
		pShaderMgr = NULL;
	}

	if (pModel3DS1)
	{
		delete pModel3DS1;
		pModel3DS1 = NULL;
	}

	if (pModel3DS2)
	{
		delete pModel3DS2;
		pModel3DS2 = NULL;
	}
}




int CScene2::DrawGLScene(void)	// Função que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter

	// Modo FILL ou WIREFRAME (pressione barra de espaço)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual

	// Para a fonte de luz  acompanhar o player, faça as definição aqui
	// Criando a SpotLight
	//glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	//glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);	
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, borda);	
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, LightDirection);	
	
	// Seta as posições da câmera
	pCamera->setView();



	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, LightDirection);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	// Habilita iluminação
	glEnable(GL_LIGHTING);

	// Habilita a luz
	glEnable(GL_LIGHT0);	


	// Ativa ou não o Shader Program (TRUE = Spotlight per Pixel | FALSE = Spotlight per Vertex)
	if (bActiveShaderProgram == true)
	{
		pShaderMgr->bind(0);	
		glUniform1f(uinterna, cosborda); 	// E envia o valor atual da borda para o shader
	}


	glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Cor branca para os polígonos


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glEnable(GL_TEXTURE_2D);	// Habilita Textura

	// Desenha chão
	pTextures->ApplyTexture(0);
	Draw3DSGrid(20.0f, 20.0f);



	// Desenha os objetos que compõem a cena (OBJs)
	pModel3DS1->Draw();
	pModel3DS2->Draw();











	

	
	
	glDisable(GL_TEXTURE_2D);	// Desabilita Textura

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                               DESENHA OS OBJETOS DA CENA (FIM)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	// Desabilita o processamento de shaders dos objetos
	if (bActiveShaderProgram == true)
	{
		pShaderMgr->unbind();
	}

	// Impressão de texto na tela...
	// Muda para modo de projeção ortogonal 2D
	// OBS: Desabilite Texturas e Iluminação antes de entrar nesse modo de projeção
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 100, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);


	glRasterPos2f(10.0f, 0.0f);	// Posicionando o texto na tela
	if (!bIsWireframe) {
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[TAB]  Modo FILL");
	}

	glRasterPos2f(10.0f, 20.0f);
	pTexto->glPrint("[ENTER] Ativa Shaders %d", bActiveShaderProgram);


	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posição do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Angulo de Rotação do Player
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d", iFPS);


	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;
}




void CScene2::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os cálculos de rotação da visão do Player (através das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a câmera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene2::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	// Senão, interrompe movimento do Player
	else
	{
	}


	// Verifica se a tecla 'SPACE' foi pressionada e inicia pulo do Player
	if (GetKeyState(VK_SPACE) & 0x80)
	{		
	}

	if (GetKeyState(VK_UP) & 0x80)
	{
		LightPosition[2] -= 0.1f;
	}
	if (GetKeyState(VK_DOWN) & 0x80)
	{
		LightPosition[2] += 0.1f;
	}
	if (GetKeyState(VK_LEFT) & 0x80)
	{
		LightPosition[0] -= 0.1f;
	}
	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		LightPosition[0] += 0.1f;
	}
	if (GetKeyState(VK_PRIOR) & 0x80)
	{
		LightPosition[1] += 0.1f;
	}
	if (GetKeyState(VK_NEXT) & 0x80)
	{
		LightPosition[1] -= 0.1f;
	}
}

void CScene2::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
		break;

	case VK_RETURN:
		bActiveShaderProgram = !bActiveShaderProgram;
		break;


	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene2::Draw3DSGrid(float width, float length)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}