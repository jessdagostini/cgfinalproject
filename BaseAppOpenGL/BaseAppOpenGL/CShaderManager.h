#pragma once

#include <windows.h>	
#include <glew.h>
#include <fstream> // file stream   
#include <string>
#include <iostream>

using namespace std;


struct ShaderData
{
	unsigned int shader_id;
	unsigned int shader_vp;
	unsigned int shader_fp;

	std::string vp_filename;
	std::string fp_filename;
};

class CShaderManager {
public:
	CShaderManager();
	~CShaderManager();
	
    void InitShaderManager(const char *vsFile, const char *fsFile);
    
	void bind(int shaderID);
	void unbind();

	unsigned int id(int shaderID);
	unsigned int VP_id(int shaderID);
	unsigned int FP_id(int shaderID);

	bool HasGLSLSupport(void);
	bool InitOpenGLExtensions(void);
	bool HasGeometryShaderSupport(void);
	bool HasOpenGL2Support(void);
	bool HasShaderModel4(void);

	void validateShader(GLuint shader, const char* file);
	void validateProgram(GLuint program);
	char* textFileRead(const char *fileName);

	
private:
	

	ofstream outLogFile;
	ShaderData aShaders[10];
	int iShadersCont;

	bool extensions_init;
	bool useGLSL;
	bool bGeometryShader;
	bool bGPUShader4;

};

