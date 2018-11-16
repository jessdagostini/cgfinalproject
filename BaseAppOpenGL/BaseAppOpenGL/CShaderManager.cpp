#pragma warning (disable:4996)

#include "CShaderManager.h"


CShaderManager::CShaderManager() 
{
	iShadersCont = 0;   
}

CShaderManager::~CShaderManager() 
{

	for (int i = 0; i < this->iShadersCont; i++)
	{
		glDetachShader(this->aShaders[i].shader_id, this->aShaders[i].shader_fp);
		glDetachShader(this->aShaders[i].shader_id, this->aShaders[i].shader_vp);

		glDeleteShader(this->aShaders[i].shader_fp);
		glDeleteShader(this->aShaders[i].shader_vp);
		glDeleteProgram(this->aShaders[i].shader_id);
	}
}

char* CShaderManager::textFileRead(const char *fileName) 
{
	char* text = NULL;

	if (fileName != NULL) {
		FILE *file = fopen(fileName, "rt");

		if (file != NULL) {
			fseek(file, 0, SEEK_END);
			int count = ftell(file);
			rewind(file);

			if (count > 0) {
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}
			fclose(file);
		}
	}
	return text;
}

void CShaderManager::validateShader(GLuint shader, const char* file = 0) 
{
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;

	glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
	if (length > 0) 
	{		
		outLogFile.open("ShaderLogFile.txt", ios::out | ios::trunc);
		outLogFile << "CShaderManager " << shader << " (" << (file?file:"") << ") compile error: " << buffer << endl;
		outLogFile.close();
	}

}

void CShaderManager::validateProgram(GLuint program) 
{
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;


	outLogFile.open("ShaderLogFile.txt", ios::out | ios::app);

	memset(buffer, 0, BUFFER_SIZE);
	glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
	if (length > 0)
		cout << "Program " << program << " link error: " << buffer << endl;

	glValidateProgram(program);
	GLint status;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
		cout << "Error validating shader " << program << endl;

	outLogFile.close();
}



bool CShaderManager::InitOpenGLExtensions(void)
{
	if (extensions_init) return true;
	extensions_init = true;

	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		cout << "Error:" << glewGetErrorString(err) << endl;
		extensions_init = false;
		return false;
	}




	// Abre o arquivo de log e substitui os dados existentes
	outLogFile.open("LogFile.txt", ios::out | ios::trunc);

	// Armazena informações sobre o hardware no arquivo de log
	outLogFile << "OpenGL Vendor: " << (char*) glGetString(GL_VENDOR) << "\n";
	outLogFile << "OpenGL Renderer: " << (char*) glGetString(GL_RENDERER) << "\n";
	outLogFile << "OpenGL Version: " << (char*) glGetString(GL_VERSION) << "\n\n";
	outLogFile << "OpenGL Extensions:\n" << (char*) glGetString(GL_EXTENSIONS) << "\n\n";





	// Fecha o arquivo de log
	outLogFile.close();

	HasGLSLSupport();

	return true;
}


bool CShaderManager::HasGLSLSupport(void)
{
	bGeometryShader = HasGeometryShaderSupport();
	bGPUShader4     = HasShaderModel4();

	if (useGLSL) return true;  // already initialized and GLSL is available
	useGLSL = true;

	if (!extensions_init) InitOpenGLExtensions();  // extensions were not yet initialized!!

	// Abre o arquivo de log e acrescenta dados a partir do fim do arquivo
	outLogFile.open("LogFile.txt", ios::out | ios::app);

	if (GLEW_VERSION_4_3)
	{
		outLogFile << "OpenGL 4.3 core functions are available" << endl;
	}
	else if ((GLEW_VERSION_4_0) || (GLEW_VERSION_4_1) || (GLEW_VERSION_4_2) || (GLEW_VERSION_4_3) || (GLEW_VERSION_4_4))
	{
		outLogFile << "OpenGL 4.0 core functions are available" << endl;
	}
	else if ( (GLEW_VERSION_3_0) || (GLEW_VERSION_3_1) || (GLEW_VERSION_3_2) || (GLEW_VERSION_3_3) )
	{
		outLogFile << "OpenGL 3.0 core functions are available" << endl;
	}
	else if (GLEW_VERSION_2_1)
	{
		outLogFile << "OpenGL 2.1 core functions are available" << endl;
	}
	else if (GLEW_VERSION_2_0)
	{
		outLogFile << "OpenGL 2.0 core functions are available" << endl;
	}
	else if (GLEW_VERSION_1_5)
	{
		outLogFile << "OpenGL 1.5 core functions are available" << endl;
	}
	else if (GLEW_VERSION_1_4)
	{
		outLogFile << "OpenGL 1.4 core functions are available" << endl;
	}
	else if (GLEW_VERSION_1_3)
	{
		outLogFile << "OpenGL 1.3 core functions are available" << endl;
	}
	else if (GLEW_VERSION_1_2)
	{
		outLogFile << "OpenGL 1.2 core functions are available" << endl;
	}

	if (GL_TRUE != glewGetExtension("GL_ARB_fragment_shader"))
	{
		outLogFile << "[WARNING] GL_ARB_fragment_shader extension is not available!\n";
		useGLSL = false;
	}

	if (GL_TRUE != glewGetExtension("GL_ARB_vertex_shader"))
	{
		outLogFile << "[WARNING] GL_ARB_vertex_shader extension is not available!\n";
		useGLSL = false;
	}

	if (GL_TRUE != glewGetExtension("GL_ARB_shader_objects"))
	{
		outLogFile << "[WARNING] GL_ARB_shader_objects extension is not available!\n";
		useGLSL = false;
	}

	if (useGLSL)
	{
		outLogFile << "[OK] OpenGL Shading Language is available!\n";

		// Versão máxima e mínima do GLSL suportadas
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		outLogFile << "GLSL Max: " << major << "\n";
		outLogFile << "GLSL Min: " << minor << "\n";
	}
	else
	{
		outLogFile << "[FAILED] OpenGL Shading Language is not available...\n\n";
	}   


	// Fecha o arquivo de log
	outLogFile.close();

	return useGLSL;
}

bool CShaderManager::HasGeometryShaderSupport(void)
{
	if (GL_TRUE != glewGetExtension("GL_EXT_geometry_shader4"))
		return false;

	return true;
}

bool CShaderManager::HasOpenGL2Support(void)
{
	if (!extensions_init) InitOpenGLExtensions();

	return (GLEW_VERSION_2_0 == GL_TRUE);
}   

bool CShaderManager::HasShaderModel4(void)
{
	if (GL_TRUE != glewGetExtension("GL_EXT_gpu_shader4"))
		return false;

	return true;
}

void CShaderManager::InitShaderManager(const char *vsFile, const char *fsFile) 
{

	extensions_init = false;
	useGLSL = false;
	bGeometryShader = false;
	bGPUShader4 = false;

	ShaderData shaderData;
	shaderData.vp_filename = vsFile;
	shaderData.fp_filename = fsFile;

	//	// Verifica se a compilação teve sucesso
	GLint compilacaoOK;

	InitOpenGLExtensions();

	// Abre o arquivo de log e acrescenta dados a partir do fim do arquivo
	outLogFile.open("LogFile.txt", ios::out | ios::app);

	try 
	{
		shaderData.shader_vp = glCreateShader(GL_VERTEX_SHADER);
	}
	catch (const exception& error) 
	{
		outLogFile << "VERTEX EXCEPTION: " << error.what() << endl;
	}

	try 
	{
		shaderData.shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
	}
	catch (const exception& error) 
	{
		outLogFile << "SHADER EXCEPTION: " << error.what() << endl;
	}	


	// Fecha o arquivo de log
	outLogFile.close();

	const char* vsText = textFileRead(vsFile);
	const char* fsText = textFileRead(fsFile);	

	if (vsText == NULL || fsText == NULL) 
	{
		MessageBox(NULL, "Não foi possível localizar arquivos de shader", "Arquivo não encontrado",  MB_ICONERROR | MB_OK);

		return;
	}

	glShaderSource(shaderData.shader_vp, 1, &vsText, 0);
	glShaderSource(shaderData.shader_fp, 1, &fsText, 0);

	glCompileShader(shaderData.shader_vp);
	validateShader(shaderData.shader_vp, vsFile);	

	glGetShaderiv(shaderData.shader_vp, GL_COMPILE_STATUS, &compilacaoOK);
	if(!compilacaoOK)
		MessageBox(NULL, "Não foi possível compilar o vertex shader", "Vertex CShaderManager",  MB_ICONERROR | MB_OK);

	glCompileShader(shaderData.shader_fp);
	validateShader(shaderData.shader_fp, fsFile);

	glGetShaderiv(shaderData.shader_fp, GL_COMPILE_STATUS, &compilacaoOK);
	if(!compilacaoOK)
		MessageBox(NULL, "Não foi possível compilar o fragment shader", "Fragment CShaderManager",  MB_ICONERROR | MB_OK);

	shaderData.shader_id = glCreateProgram();
	glAttachShader(shaderData.shader_id, shaderData.shader_fp);
	glAttachShader(shaderData.shader_id, shaderData.shader_vp);
	glLinkProgram(shaderData.shader_id);
	validateProgram(shaderData.shader_id);

	this->aShaders[this->iShadersCont++] = shaderData;
}



unsigned int CShaderManager::id(int shaderID) 
{
	return this->aShaders[shaderID].shader_id;
}

unsigned int CShaderManager::VP_id(int shaderID) 
{
	return this->aShaders[shaderID].shader_vp;
}

unsigned int CShaderManager::FP_id(int shaderID) 
{
	return this->aShaders[shaderID].shader_fp;
}

void CShaderManager::bind(int shaderID) 
{
	glUseProgram(this->aShaders[shaderID].shader_id);
}

void CShaderManager::unbind() 
{
	glUseProgram(0);
}
