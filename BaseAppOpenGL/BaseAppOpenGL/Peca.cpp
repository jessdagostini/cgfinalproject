#include "Peca.h"


Peca::Peca(float pX, float pY, float pZ, float pC)
{
	x = pX;
	y = pY;
	z = pZ;
	color = pC;

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(color, color, color);
	glRotatef(-90.0f, 1.0f, .0f, 0.0f);
	DrawCircle(0.0f, 0.0f, 1.0f, 1.0f, 25);
	glPopMatrix();
}

void Peca::showPeca(float pY, float pC)
{
	color = pC;
	glPushMatrix();
	glTranslatef(x, 0.5f, z);
	glColor3f(color, color, color);
	glRotatef(-90.0f, 1.0f, .0f, 0.0f);
	DrawCircle(0.0f, 0.0f, 1.0f, 1.0f, 25);
	glPushMatrix();
	glTranslatef(-0.4f, 0.4f, 0.0f);
	if (color == 0.0f) {
		glColor3f(1.0f, 1.0f, 1.0f);
	}
	else {
		glColor3f(0.0f, 0.0f, 0.0f);
	}
	
	glutSolidTorus(.7f, 0.75f, 25, 25);
	glPopMatrix();
	glPopMatrix();
}

void Peca::DrawCircle(GLdouble x, GLdouble y, GLdouble z, GLdouble raio, int num_linhas)
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
