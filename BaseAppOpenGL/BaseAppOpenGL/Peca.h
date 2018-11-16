#pragma once
#include "SceneBaseClass.h"
#include <cmath>
#define M_PI 3.14159265358979323846	

class Peca
{
public:
	Peca(float pX, float pY, float pZ, float pC);
	~Peca(void);

	void Peca::showPeca(float pY, float pC);
	void Peca::DrawCircle(GLdouble x, GLdouble y, GLdouble z, GLdouble raio, int num_linhas);


private:
	float x, y, z;
	float color;

	float angulo;
};
