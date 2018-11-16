#pragma warning(disable:4996)

#include "CTexto.h"

CTexto::CTexto()
{
	this->BuildFont();
}


CTexto::~CTexto()
{
	this->KillFont();
}


void CTexto::BuildFont()
{

	HWND hWnd1 = FindWindow("OpenGL", "OpenGL Application");
	HDC hDC1 = GetDC(hWnd1); 

	HFONT font;		// Variável para usar fontes do windows

	base = glGenLists(96);	// Cria 256 display list(uma lista para cada caracter da tabela ASCII)
	
	// Criando a fonte...
	font = CreateFont(-12,						// Altura da fonte (tem q ser negativo)
					    0,						// Largura da fonte (0 = default)
						0,						// Rotação da fonte em graus (angulo de escape). Sempre 0
						0,						// Angulo de orientação. Sempre 0
						FW_BOLD,				// Espessura da fonte(FW_DONTCARE = 0 | FW_NORMAL = 400 | FW_BOLD = 700 | FW_BLACK = 900)
						FALSE,					// Itálico
						FALSE,					// Sobre-escrito
						FALSE,					// Sub-escrito
						ANSI_CHARSET,			// Tipo de caracter (Para fonte Webdings e Wingdings usar SYMBOL_CHARSET)
						OUT_TT_PRECIS,			// Se houver mais de uma fonte com o mesmo nome, usa a que for TRUETYPE
						CLIP_DEFAULT_PRECIS,	// Tipo de clipping usado na fonte (melhor deixar essa opção default)
						ANTIALIASED_QUALITY,	// Qualidade da fonte na tela (essa é a melhor opção)
						FF_DONTCARE | DEFAULT_PITCH,	// Familia que pertence a fonte: FF_DECORATIVE, FF_MODERN, FF_ROMAN, FF_SCRIPT, FF_SWISS   |   Pitch: FIXED_PITCH, VARIABLE_PITCH
						"Courier New");			// Nome da fonte a ser usada



	SelectObject(hDC1, font);					// Seleciona a fonte que foi criada

	wglUseFontBitmaps(hDC1, 32, 96, base);		// Cria 96 caracteres a partir do caracter 32 (espaço em branco)

}






// Função usada para deletar as lista de caracteres da memória no final do programa
void CTexto::KillFont()
{
	glDeleteLists(base,   96);
}



// Função que escreve o texto na tela
void CTexto::glPrint(const char *fmt, ...)
{

	char text[256];		// Armazena o texto

	va_list	ap;			// Ponteiro para lista de argumentos

	// Testa se há algum texto para ser impresso
	if (fmt == NULL) return;


	// Converte simbolos no número atual que representa o símbolo. O texto final e os 
	// símbolos são armazenados em TEXT
	va_start(ap, fmt);		// Passa a string para as variáveis

		vsprintf(text, fmt, ap);	// Adiciona simbolos convertidos para seus valores atuais

	va_end(ap);				// O resultado é armazenado em text


	glPushAttrib(GL_LIST_BIT);	// Previne glBaseList para não afetar outras display Lists usadas no programa
	glListBase(base - 32);		// Localiza o caracter na tabela ASCII

	// Desenha na tela o texto contido na display list
	glCallLists(strlen(text),			// Analisa quantas letras serão impressas na tela
				GL_UNSIGNED_BYTE,		// Qual o número máximo de caracteres (GL_UNSIGNED_BYTE = 0 até 255)
				text);					// Variável que contém o texto que será mostrado na tela

	glPopAttrib();

}

