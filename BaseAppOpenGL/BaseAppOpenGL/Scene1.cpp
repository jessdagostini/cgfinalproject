#include "Scene1.h"


CScene1::CScene1()
{

	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;
	pModel3DS_1 = NULL;
	pModel3DS_2 = NULL;
	pModel3DS_3 = NULL;

	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impressão de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(30.0f, 65.0f, 30.0f, 1.0f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;
	posI = 0;
	posJ = 0; 
	I = 0;
	J = 0;

	// Carrega todas as texturas
	pTextures = new CTexture();
	pTextures->CreateTextureMipMap(0, "../Scene1/crate.bmp");
	pTextures->CreateTextureMipMap(1, "../Scene1/grass.bmp");
	pTextures->CreateTextureLinear(2, "../Scene1/pl_earth_full.jpg");
	pTextures->CreateTextureTGA(3, "../Scene1/tree.tga");
	pTextures->CreateTextureTGA(4, "../Scene1/tree2.tga");
	

	fPosX = 0.0f;
	fPosY = 10.0f;
	fPosZ = 0.0f;
	fMovementFactor = 0.1f;

	// Cria esfera com coordenadas de textura
	this->sphere1 = gluNewQuadric();
	gluQuadricTexture(this->sphere1, TRUE);

	// Carrega Objetos da  Cena (casa)
	pModel3DS_1 = new CModel_3DS();
	pModel3DS_1->Load("../Scene1/ok.3DS");

	// Carrega Objetos da  Cena (muro)
	pModel3DS_2 = new CModel_3DS();
	pModel3DS_2->Load("../Scene1/Muro.3DS");

	// Carrega Objetos da  Cena (gramado)
	pModel3DS_3 = new CModel_3DS();
	pModel3DS_3->Load("../Scene1/Plane001.3DS");

	// Inicializa o vetor que contém o controle das peças
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			visible[i][j] = false;
		}
	}

	// Inicia a jogada sempre com o jogador cor preta
	player = 0.0f;
}


CScene1::~CScene1(void)
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

	gluDeleteQuadric(this->sphere1);

	if (pModel3DS_1)
	{
		delete pModel3DS_1;
		pModel3DS_1 = NULL;
	}

	if (pModel3DS_2)
	{
		delete pModel3DS_2;
		pModel3DS_2 = NULL;
	}

	if (pModel3DS_3)
	{
		delete pModel3DS_3;
		pModel3DS_3 = NULL;
	}
}




int CScene1::DrawGLScene(void)	// Função que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter
	
	pTimer->Update();							// Atualiza o timer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posições da câmera
	//pCamera->setView();

	gluLookAt(30.0f, 65.0f, 30.0f,
		-20.0f, -20.0f, -20.0f,
		0.0f, 1.0f, 0.0f);

	// Desenha grid 
	//Draw3DSGrid(20.0f, 20.0f);

	DrawAxis();

	// Modo FILL ou WIREFRAME (pressione barra de espaço)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(.0f, 27.0f, 0.0f);	
	DrawPecas();
	pecas[I][J]->showPeca(0.5f, player);
	glPopMatrix();

	// Habilita mapeamento de texturas 2D
	glEnable(GL_TEXTURE_2D);
	glTranslatef(.0f,.0f, 0.0f);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	pModel3DS_1->Draw();
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	pModel3DS_1->Draw();
	glPopMatrix();

	
	// Desenha as árvores aplicando Blending.
	// A operação é baseada no canal Alpha da textura .TGA
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 0.0f, 0.0f, 0.6f);
	glPushMatrix();
	auxSolidBox(100.0f, 0.1f, 100.0f);
	glPopMatrix();
	glDisable(GL_BLEND);


	glDisable(GL_TEXTURE_2D);



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.2f;

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


	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posição do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplicação e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime()/1000));

	//// Imprime o FPS da aplicação e o Timer
	glRasterPos2f(10.0f, 100.0f);
	pTexto->glPrint("Valor de I: %d ---- Valor de J: %d", I, J);

	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;
}

void CScene1::DrawPecas()
{
	float valorX = -12.10f;
	float valorZ = 0.40f;
	float x = valorX;
	float z = valorZ;
	float y = 0.5f;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// Seta as quatro peças iniciais como visíveis
			if ((i == 3 && j == 3) || (i == 4 && j == 4)) {
				pecas[i][j] = new Peca(x, y, z, 1.0f);
				visible[i][j] = true;
			} else if ((i == 3 && j == 4) || (i == 4 && j == 3)) {
				glColor3f(1.0f, 1.0f, 1.0f);
				pecas[i][j] = new Peca(x, y, z, 0.0f);
				visible[i][j] = true;
			}
			else if (visible[i][j]) {
				pecas[i][j] = new Peca(x, y, z, colors[i][j]);
			}
			else {
				pecas[i][j] = new Peca(x, 0.0f, z, 0.0f);
			}
			x += 1.75f;
			z -= 1.75;
		}

		valorX += 1.75f;
		valorZ += 1.75f;
		x = valorX;
		z = valorZ;
	}
}

void CScene1::DrawCircle(GLdouble x, GLdouble y, GLdouble z, GLdouble raio, int num_linhas)
{
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < num_linhas; i++) {
		angulo = i * 2 * M_PI / num_linhas;
		glVertex3f(x + (cos(angulo) * raio), y + (sin(angulo) * raio), z);
	}
	glEnd();
	glPopMatrix();
}

void CScene1::MouseMove(void) // Tratamento de movimento do mouse
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

void CScene1::KeyPressed(void) // Tratamento de teclas pressionadas
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


	if (GetKeyState(VK_UP) & 0x80)
	{
		if (posI >= 0.0f) posI -= 0.1f;
		I = rint(int(posI));
	}
	if (GetKeyState(VK_DOWN) & 0x80)
	{
		if (posI <= 7.0f) posI += 0.1f;
		I = rint(int(posI));
	}
	if (GetKeyState(VK_LEFT) & 0x80)
	{
		if (posJ >= 0.0f) posJ -= 0.1f;
		J = rint(int(posJ));
	}
	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		if (posJ <= 7.0f) posJ += 0.1f;
		J = rint(int(posJ));
	}

	if (GetKeyState(VK_PRIOR) & 0x80)
	{
		fPosY += fMovementFactor;
	}
	if (GetKeyState(VK_NEXT) & 0x80)
	{
		fPosY -= fMovementFactor;
	}
}

void CScene1::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
	{
		I = rint(int(posI));
		J = rint(int(posJ));
		visible[I][J] = true;
		colors[I][J] = player;
		if (player == 1.0f) {
			player = 0.0f;
		}
		else {
			player = 1.0f;
		}
		I++;
		J++;
	}
		break;

	case VK_RETURN:

		break;


	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene1::Draw3DSGrid(float width, float length)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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


void CScene1::DrawCube(float pX, float pY, float pZ,
	float rX, float rY, float rZ, float angle,
	float sX, float sY, float sZ,
	int texID)
{

	// Seta a textura ativa
	if (texID >= 0)
		pTextures->ApplyTexture(texID);

	glPushMatrix();
	glTranslatef(pX, pY, pZ);
	glRotatef(angle, rX, rY, rZ);
	glScalef(sX, sY, sZ);

	glBegin(GL_QUADS);
	// face frente
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, 0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, 0.5f);

	// face trás
	glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
	
	// face direita
	glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);

	// face esquerda
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

	// face baixo
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);

	// face cima
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f,  0.5f,  -0.5f);

	glEnd();

	glPopMatrix();


}

void CScene1::DrawAxis()
{
	glPushMatrix();
	glTranslatef(0.0f, 27.5f, 0.0f);
	glBegin(GL_LINES);
	// Eixo X
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Eixo Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);

	// Eixo Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
	glPopMatrix();
}