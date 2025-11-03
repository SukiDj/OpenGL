#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

#define PI 3.14159265358979323846
#define toRad(x) (x * PI) / 180

CGLRenderer::CGLRenderer(void)
{
	m_hrc = NULL;
	eyex = 0, eyey = 0, eyez = 0;
	centerx = 0, centery = 0, centerz = 0;
	upx = 0, upy = 1, upz = 0;
	light0 = true;
	light1 = true;
	light2 = true;
	normals = false;
	this->angle_v = 0;
	this->angle_h = 0;

	this->viewR = 20;
	this->viewAngleXY = 0;
	this->viewAngleXZ = 0;

	light1Mat.SetAmbient(SVETLO_1, 1);
	light1Mat.SetDiffuse(SVETLO_1, 1);
	light1Mat.SetEmission(SVETLO_1, 1);
	light1Mat.SetShininess(128);

	light2Mat.SetAmbient(SVETLO_2, 1);
	light2Mat.SetDiffuse(SVETLO_2, 1);
	light2Mat.SetEmission(SVETLO_2, 1);
	light2Mat.SetShininess(128);

	light3Mat.SetAmbient(SVETLO_3, 1);
	light3Mat.SetDiffuse(SVETLO_3, 1);
	light3Mat.SetEmission(SVETLO_3, 1);
	light3Mat.SetShininess(128);

	vaseMat1.SetAmbient(VAZA_BOJA_1_AMBIENT, 1.0);
	vaseMat1.SetDiffuse(VAZA_BOJA_1, 1.0);
	vaseMat1.SetSpecular(VAZA_BOJA_1_SPECULAR, 1);

	vaseMat2.SetAmbient(VAZA_BOJA_2_AMBIENT, 1.0);
	vaseMat2.SetDiffuse(VAZA_BOJA_2, 1.0);
	vaseMat2.SetSpecular(VAZA_BOJA_2_SPECULAR, 1);

	wallMat.SetAmbient(0.2, 0.2, 0.2, 1);
	wallMat.SetDiffuse(0.6, 0.6, 0.6, 1);
	wallMat.SetEmission(0, 0, 0, 1);

	CalculatePosition();
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1, 1, 1, 0);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	AmbientLight();
	LampLight();

	glEnable(GL_LIGHTING);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);

	float light_position[] = { 0.5, 1, 0.5, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);

	ViewPointLight();


	glPushMatrix();
	{
		glEnable(GL_CULL_FACE);
		DrawRoom();
		glDisable(GL_CULL_FACE);
		DrawBase();
		glTranslated(0, 5.5, 0);
		DrawVase();
	}
	glPopMatrix();
	DrawAxis(100);


	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / (double)h, 10, 100);
	//gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxis(double width)
{
	glDisable(GL_LIGHTING);

	glLineWidth(1);
	glPointSize(10);

	glBegin(GL_LINES);
		glColor3d(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(width, 0, 0);

		glColor3d(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, width, 0);

		glColor3d(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, width);
	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::CalculatePosition()
{
	double dWXY = this->viewAngleXY * 3.14 / 180,
		dWXZ = this->viewAngleXZ * 3.14 / 180;

	this->eyex = this->viewR * cos(dWXY) * cos(dWXZ);
	this->eyey = this->viewR * sin(dWXY);
	this->eyez = this->viewR * cos(dWXY) * sin(dWXZ);

	this->upy = signbit(cos(dWXY)) ? -1 : 1;
}

void CGLRenderer::RotateView(double dXY, double dXZ)
{
	viewAngleXY += dXY;
	viewAngleXY = min(90, viewAngleXY);
	viewAngleXY = max(-90, viewAngleXY);

	viewAngleXZ += dXZ;
	CalculatePosition();
}

void CGLRenderer::ZoomCamera(bool direction)
{
	viewR += direction ? 1 : -1;

	viewR = max(1, viewR);
	viewR = min(viewR, 1000);

	CalculatePosition();
}

void CGLRenderer::DrawRoom()
{
	wallMat.SetFace(GL_FRONT);
	double a = 20;
	float step = 1;

	glBegin(GL_QUADS);
	{
		//down
		glNormal3f(0, 1, 0);
		for (float i = -a / 2; i < a / 2; i += step)
		{
			for (float j = -a / 2; j < a / 2; j += step)
			{
				glVertex3f(i, 0, j);
				glVertex3f(i, 0, j + step);
				glVertex3f(i + step, 0, j + step);
				glVertex3f(i + step, 0, j);
			}
		}
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		//up
		glNormal3f(0, -1, 0);
		for (float i = -a / 2; i < a / 2; i += step)
		{
			for (float j = -a / 2; j < a / 2; j += step)
			{
				glVertex3f(i + step, a, j);
				glVertex3f(i + step, a, j + step);
				glVertex3f(i, a, j + step);
				glVertex3f(i, a, j);
			}
		}
	}
	glEnd();

	//front
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(a / 2, i, j);
			glVertex3f(a / 2, i, j + step);
			glVertex3f(a / 2, i + step, j + step);
			glVertex3f(a / 2, i + step, j);
		}
	}
	glEnd();

	//back
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(-a / 2, i + step, j);
			glVertex3f(-a / 2, i + step, j + step);
			glVertex3f(-a / 2, i, j + step);
			glVertex3f(-a / 2, i, j);
		}
	}
	glEnd();

	//right
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(j, i, -a / 2);
			glVertex3f(j + step, i, -a / 2);
			glVertex3f(j + step, i + step, -a / 2);
			glVertex3f(j, i + step, -a / 2);
		}
	}
	glEnd();

	//left
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(j, i + step, a / 2);
			glVertex3f(j + step, i + step, a / 2);
			glVertex3f(j + step, i, a / 2);
			glVertex3f(j, i, a / 2);
		}
	}
	glEnd();
}

void CGLRenderer::DrawBase()
{
	wallMat.SetFace(GL_FRONT);
	glPushMatrix();
	{
		DrawHalfSphere(2.5, 100, 100);
		glTranslated(0, 2, 0);
		DrawCylinder(2.5, 1, 1, wallMat, 8, true);
		glTranslated(0, 2.5, 0);
		DrawPlate(1, 8, 8, 2, 100, 100, wallMat);
	}
	glPopMatrix();
}

void CGLRenderer::DrawHalfSphere(double r, int nSegAlpha, int nSegBeta)
{
	glEnable(GL_CULL_FACE);

	int alphaStep = 360 / nSegAlpha;
	int betaStep = 360 / nSegBeta;


	glBegin(GL_QUAD_STRIP);
	{
		for (int beta = 0; beta < 180; beta += betaStep) {
			for (int alpha = 0; alpha <= 90; alpha += alphaStep) {

				double x1 = r * cos(toRad(alpha)) * cos(toRad(beta));
				double y1 = r * sin(toRad(alpha));
				double z1 = r * cos(toRad(alpha)) * sin(toRad(beta));

				double x2 = r * cos(toRad(alpha)) * cos(toRad(beta) + toRad(betaStep));
				double y2 = r * sin(toRad(alpha));
				double z2 = r * cos(toRad(alpha)) * sin(toRad(beta) + toRad(betaStep));

				glNormal3d(x2 / r, y2 / r, z2 / r);
				glVertex3d(x2, y2, z2);
				glNormal3d(x1 / r, y1 / r, z1 / r);
				glVertex3d(x1, y1, z1);
			}
		}
	}
	glEnd();
	glBegin(GL_QUAD_STRIP);
	{
		for (int beta = 0; beta < 180; beta += betaStep) {
			for (int alpha = 90; alpha <= 180; alpha += alphaStep) {

				double x1 = r * cos(toRad(alpha)) * cos(toRad(beta));
				double y1 = r * sin(toRad(alpha));
				double z1 = r * cos(toRad(alpha)) * sin(toRad(beta));

				double x2 = r * cos(toRad(alpha)) * cos(toRad(beta) + toRad(betaStep));
				double y2 = r * sin(toRad(alpha));
				double z2 = r * cos(toRad(alpha)) * sin(toRad(beta) + toRad(betaStep));

				glNormal3d(x1 / r, y1 / r, z1 / r);
				glVertex3d(x1, y1, z1);
				glNormal3d(x2 / r, y2 / r, z2 / r);
				glVertex3d(x2, y2, z2);
				
			}
		}
	}
	glEnd();
	glDisable(GL_CULL_FACE);
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	int alphaStep = 180 / nSegAlpha;
	int betaStep = 360 / nSegBeta;

	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int alpha = -90; alpha <= 90; alpha += alphaStep) {
			for (int beta = 0; beta <= 360; beta += betaStep) {
				double x1 = cos(toRad(alpha)) * cos(toRad(beta));
				double y1 = sin(toRad(alpha));
				double z1 = cos(toRad(alpha)) * sin(toRad(beta));

				glNormal3d(x1, y1, z1);
				glVertex3d(r * x1, r * y1, r * z1);

				double x2 = cos(toRad(alpha + alphaStep)) * cos(toRad(beta));
				double y2 = sin(toRad(alpha + alphaStep));
				double z2 = cos(toRad(alpha + alphaStep)) * sin(toRad(beta));

				glNormal3d(x2, y2, z2);
				glVertex3d(r * x2, r * y2, r * z2);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, CGLMaterial& mat, int nSeg, bool includeBase)
{
	int alphaStep = 360 / nSeg;

	mat.SetFace(GL_FRONT);
	if (includeBase) {
		glNormal3d(0, 1, 0);

		glBegin(GL_TRIANGLE_FAN);
		{
			glVertex3d(0, h, 0);

			for (int alpha = 360; alpha >= 0; alpha -= alphaStep) {

				double x = r2 * cos(toRad(alpha));
				double z = r2 * sin(toRad(alpha));

				glVertex3d(x, h, z);
			}
		}
		glEnd();

		glNormal3d(0, -1, 0);

		glBegin(GL_TRIANGLE_FAN);
		{
			glVertex3d(0, 0, 0);

			for (int alpha = 0; alpha <= 360; alpha += alphaStep) {

				double x = r1 * cos(toRad(alpha));
				double z = r1 * sin(toRad(alpha));

				glVertex3d(x, 0, z);
			}
		}
		glEnd();		
	}

	double x = (r1 - r2) / 2;
	double L = sqrt(pow(x, 2) + pow(h, 2));
	double ny = x / L;
	double nr = h / L;
	glBegin(GL_QUAD_STRIP);
	{
		for (int alpha = 0; alpha <= 360; alpha += alphaStep) {
			double x1 = r1 * cos(toRad(alpha));
			double z1 = r1 * sin(toRad(alpha));

			double x2 = r2 * cos(toRad(alpha));
			double z2 = r2 * sin(toRad(alpha));

			glNormal3d(cos(toRad(alpha)), 0, sin(toRad(alpha)));
			glVertex3d(x2, 0, z2);
			glVertex3d(x1, h, z1);
		}
	}
	glEnd();
	if (!normals) {
		return;
	}

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	{
		glColor3d(0, 1, 0);

		for (int alpha = 0; alpha <= 360; alpha += alphaStep) {
			double x1 = r1 * cos(toRad(alpha));
			double z1 = r1 * sin(toRad(alpha));

			double x2 = r2 * cos(toRad(alpha));
			double z2 = r2 * sin(toRad(alpha));


			glVertex3d(x1, 0, z1);
			glVertex3d(x1 + nr * cos(toRad(alpha)), ny, z1 + nr * sin(toRad(alpha)));

			glVertex3d(x2, h, z2);
			glVertex3d(x2 + nr * cos(toRad(alpha)), h + ny, z2 + nr * sin(toRad(alpha)));
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawPlate(double h, double w, double d, int nSegH, int nSegW, int nSegD, CGLMaterial& mat)
{

	mat.SetFace(GL_FRONT);

	// left
	glPushMatrix();
	{
		glTranslated(-w / 2, 0, d / 2);
		DrawSideGrid(h, w, nSegH, nSegW, false);
	}
	glPopMatrix();

	// right
	glPushMatrix();
	{
		glRotated(180, 0, 1, 0);
		glTranslated(-w / 2, 0, d / 2);
		DrawSideGrid(h, w, nSegH, nSegW, false);
	}
	glPopMatrix();

	// Up
	glPushMatrix();
	{
		glRotated(90, 1, 0, 0);
		glTranslated(-w / 2, -d / 2, -h);
		DrawSideGrid(w, d, nSegH, nSegW, true);
	}
	glPopMatrix();

	// Down
	glPushMatrix();
	{
		glRotated(-90, 1, 0, 0);
		glTranslated(-w / 2, -d / 2, 0);
		DrawSideGrid(w, d, nSegH, nSegW, true);
	}
	glPopMatrix();

	// front
	glPushMatrix();
	{
		glRotated(90, 0, 1, 0);
		glTranslated(-d / 2, 0, -w / 2);
		DrawSideGrid(h, d, nSegH, nSegW, true);
	}
	glPopMatrix();

	// back
	glPushMatrix();
	{
		glRotated(-90, 0, 1, 0);
		glTranslated(-d / 2, 0, -w / 2);
		DrawSideGrid(h, d, nSegH, nSegW, true);
	}
	glPopMatrix();

}
void CGLRenderer::DrawSideGrid(double h, double w, int nSegH, int nSegW, bool invert)
{
	double segW = w / nSegW;
	double segH = h / nSegH;

	double nz = invert ? -1 : 1;

	glNormal3d(0, 0, nz);

	glBegin(GL_QUADS);
	{
		for (double i = 0; i < h; i += segH) {
			for (double j = 0; j < w; j += segW) {
				if (!invert)
				{
					glVertex3d(j, i, 0);
					glVertex3d(j + segW, i, 0);
					glVertex3d(j + segW, i + segH, 0);
					glVertex3d(j, i + segH, 0);
				}
				else
				{
					glVertex3d(j, i + segH, 0);
					glVertex3d(j + segW, i + segH, 0);
					glVertex3d(j + segW, i, 0);
					glVertex3d(j, i, 0);
				}
			}
		}
	}
	glEnd();
	if (!normals) {
		return;
	}

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	{
		glColor3d(0, 1, 0);

		// dužina linije normale (malo manja od pola segmenta)
		double nLen = min(segW, segH);

		for (double i = 0; i < h; i += segH) {
			for (double j = 0; j < w; j += segW) {
				double cx = j + segW * 0.5;
				double cy = i + segH * 0.5;
				double cz = 0.0;

				glVertex3d(cx, cy, cz);
				glVertex3d(cx + 0 * nLen, cy + 0 * nLen, cz + nz * nLen);
			}
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawVase()
{
	glPushMatrix();
	{
		DrawRing(R_H, R_R1, R_R2, vaseMat1);
		glTranslated(0, R_H, 0);
		DrawRing(R_H, R_R2, R_R3, vaseMat2);
		glTranslated(0, R_H, 0);
		DrawRing(R_H, R_R3, R_R3, vaseMat1);
		glTranslated(0, R_H, 0);
		DrawRing(R_H, R_R3, R_R3, vaseMat2);
		glTranslated(0, R_H, 0);
		DrawRing(R_H, R_R3, R_R2, vaseMat1);
		glTranslated(0, R_H, 0);
		DrawRing(R_H, R_R2, R_R3, vaseMat2);
		glTranslated(0, R_H, 0);
		DrawRing(R_H, R_R3, R_R2, vaseMat1);
		glTranslated(0, R_H, 0);
		DrawRing(R_H, R_R2, R_R1, vaseMat2);
		glTranslated(0, R_H, 0);
		DrawRing(R_H, R_R1, R_R2, vaseMat1);
		glTranslated(0, R_H, 0);
		DrawRing(R_H, R_R2, R_R3, vaseMat2);
		glTranslated(0, R_H, 0);
		DrawRing(R_H, R_R3, R_R2, vaseMat1);
		glTranslated(0, R_H, 0);
		DrawRing(R_H, R_R2, R_R3, vaseMat2);
		glTranslated(0, R_H, 0);
		DrawRing(R_H, R_R3, R_R2, vaseMat1);
		glTranslated(0, R_H, 0);
		DrawRing(R_H, R_R2, R_R1, vaseMat2);
	}
	glPopMatrix();
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);// moze a i ne mora
}

void CGLRenderer::DrawRing(double h, double r1, double r2, CGLMaterial& mat, int nSeg, bool includeBase)
{
	mat.SetFace(GL_FRONT_AND_BACK);

	int alphaStep = 360 / nSeg;

	double x = (r1 - r2) / 2;
	double L = sqrt(pow(x, 2) + pow(h, 2));
	double ny = x / L;
	double nr = h / L;

	// Omotac.
	glBegin(GL_TRIANGLE_STRIP);
	{

		for (int alpha = 0; alpha <= 360; alpha += alphaStep) {
			glNormal3d(nr * cos(toRad(alpha)), ny, nr * sin(toRad(alpha)));
			glVertex3d(r1 * cos(toRad(alpha)), 0, r1 * sin(toRad(alpha)));
			glVertex3d(r2 * cos(toRad(alpha)), h, r2 * sin(toRad(alpha)));
		}
	}
	glEnd();

	if (!normals) {
		return;
	}

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	{
		glColor3d(0, 1, 0);

		for (int alpha = 0; alpha <= 360; alpha += alphaStep) {
			double x1 = r1 * cos(toRad(alpha));
			double z1 = r1 * sin(toRad(alpha));

			double x2 = r2 * cos(toRad(alpha));
			double z2 = r2 * sin(toRad(alpha));


			glVertex3d(x1, 0, z1);
			glVertex3d(x1 + nr * cos(toRad(alpha)), ny, z1 + nr * sin(toRad(alpha)));

			glVertex3d(x2, h, z2);
			glVertex3d(x2 + nr * cos(toRad(alpha)), h + ny, z2 + nr * sin(toRad(alpha)));
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::AmbientLight()
{
	GLfloat lmodel_ambient[] = { 0.9, 0.9, 0.9, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

	GLfloat light_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light0_diffuse[] = { 0.6, 0.6, 0.6, 0 };
	GLfloat light0_specular[] = { 0.3, 0.3, 0.3, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
}

void CGLRenderer::LampLight()
{
	GLfloat light_ambient[] = { 0.8, 0.8, 0.8, 1.0 };

	GLfloat light1_diffuse[] = { SVETLO_1, 1 };
	GLfloat light1_specular[] = { SVETLO_1_SPECULAR, 1 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.2);

	GLfloat light2_diffuse[] = { SVETLO_2, 1 };
	GLfloat light2_specular[] = { SVETLO_2_SPECULAR, 1 };
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 1.2);

	GLfloat light3_diffuse[] = { SVETLO_3, 1 };
	GLfloat light3_specular[] = { SVETLO_3_SPECULAR, 1 };
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 25);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 1.2);
}

void CGLRenderer::SetLightPositionAndDirection(GLenum light, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, bool lightOn)
{
	if (!lightOn) {
		glDisable(light);
		return;
	}

	GLfloat light_pos[] = { posX, posY, posZ, 1 };
	GLfloat spot_dir[] = { dirX, dirY, dirZ, 1 };

	glLightfv(light, GL_POSITION, light_pos);
	glLightfv(light, GL_SPOT_DIRECTION, spot_dir);

	glEnable(light);
}

void CGLRenderer::DrawLight(double posX, double posY, double posZ, double r, CGLMaterial& mat, bool lightOn)
{
	if (!lightOn) {
		return;
	}

	mat.SetFace(GL_FRONT);

	glPushMatrix();
	{
		glTranslated(posX, posY, posZ);
		DrawSphere(r, 10, 10);
	}
	glPopMatrix();
}

void CGLRenderer::ViewPointLight()
{
	SetLightPositionAndDirection(GL_LIGHT1, LIGHT1_POS, 1, 0, 0, light0);
	DrawLight(LIGHT1_POS, 1, light1Mat, light0);

	SetLightPositionAndDirection(GL_LIGHT2, LIGHT2_POS, -1, 0, 0, light1);
	DrawLight(LIGHT2_POS, 1, light2Mat, light1);

	SetLightPositionAndDirection(GL_LIGHT3, LIGHT3_POS, 0, -1, 0, light2);
	DrawLight(LIGHT3_POS, 1, light3Mat, light2);
}
