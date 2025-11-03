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
	this->viewR = 10;

	this->viewAngleXY = 0;
	this->viewAngleXZ = 0;

	eyex = 0, eyey = 0, eyez = 0;
	centerx = 0, centery = 0, centerz = 0;
	upx = 0, upy = 1, upz = 0;

	this->CalculatePosition();
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
	{
		glClearColor(POZADINA, 1.0);
	}
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	{
		// brisanje prethodnog sadrzaja: boju-pozadinu i informacije od dubini
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();// resetuje trenutnu matricu transformacije na identicnu

		// pozicioniranje kamere
		gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);

		// skaliranje i translacija cele scene
		glScaled(0.75, 0.75, 0.75);
		glTranslated(0, -3.33, 0);// na dole se pomera po y osi
		DrawAxis(100);// iscrtavanje koordinatnih osa

		// iscrtavanje mreze
		glPushMatrix();// izolacija transformacije-promene u koordinatnom pocetku ne uticu na druge objekte
		{
			double gridSize = 10;
			glTranslated(-gridSize / 2, 0, -gridSize / 2);// pomera mrezu u centar scene
			DrawGrid(gridSize, gridSize, 10, 10);// mreza 10x10 sa 10 segmenata po osi
		}
		glPopMatrix();

		glPushMatrix();
		{
			DrawFigure(this->angle);
		}
		glPopMatrix();

	}
	glFlush();// forsira izvrsenje svih prethodnih komandi
	SwapBuffers(pDC->m_hDC);// sprecava treperenje slike
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	{
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		gluPerspective(40, (double)w / h, 1, 100);
		glMatrixMode(GL_MODELVIEW);
	}
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

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	int alphaStep = 360 / nSegAlpha;
	int betaStep = 360 / nSegBeta;


	glBegin(GL_QUAD_STRIP);
	{
		for (int beta = betaStep; beta <= 180; beta += betaStep) {
			for (int alpha = 0; alpha <= 360; alpha += alphaStep) {

				double x1 = r * cos(toRad(alpha)) * cos(toRad(beta));
				double y1 = r * sin(toRad(alpha));
				double z1 = r * cos(toRad(alpha)) * sin(toRad(beta));

				double x2 = r * cos(toRad(alpha)) * cos(toRad(beta) + toRad(betaStep));
				double y2 = r * sin(toRad(alpha));
				double z2 = r * cos(toRad(alpha)) * sin(toRad(beta) + toRad(betaStep));

				glVertex3d(x1, y1, z1);
				glVertex3d(x2, y2, z2);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg)
{
	int alphaStep = 360 / nSeg;
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3d(0, h, 0);

		for (int alpha = 0; alpha <= 360; alpha += alphaStep) {

			double x = r1 * cos(toRad(alpha));
			double z = r1 * sin(toRad(alpha));

			glVertex3d(x, h, z);
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3d(0, 0, 0);

		for (int alpha = 0; alpha <= 360; alpha += alphaStep) {

			double x = r2 * cos(toRad(alpha));
			double z = r2 * sin(toRad(alpha));

			glVertex3d(x, 0, z);
		}
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	{
		for (int alpha = 0; alpha <= 360; alpha += alphaStep) {
			double x1 = r1 * cos(toRad(alpha));
			double z1 = r1 * sin(toRad(alpha));

			double x2 = r2 * cos(toRad(alpha));
			double z2 = r2 * sin(toRad(alpha));

			glVertex3d(x1, h, z1);
			glVertex3d(x2, 0, z2);
		}
	}
	glEnd();
}

void CGLRenderer::DrawCone(double h, double r, int nSeg)
{
	int alphaStep = 360 / nSeg;
	glBegin(GL_TRIANGLE_FAN);
	{
		glColor3d(DEO_KAKTUSA);
		glVertex3d(0, h, 0);

		for (int alpha = 0; alpha <= 360; alpha += alphaStep) {
			double x = r * cos(toRad(alpha));
			double z = r * sin(toRad(alpha));

			glVertex3d(x, 0, z);
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3d(0, 0, 0);

		for (int alpha = 0; alpha <= 360; alpha += alphaStep) {

			double x = r * cos(toRad(alpha));
			double z = r * sin(toRad(alpha));

			glVertex3d(x, 0, z);
		}
	}
	glEnd();
}

void CGLRenderer::DrawAxis(double width)
{
	glLineWidth(1);
	glPointSize(10);

	glBegin(GL_LINES);
	{
		glColor3d(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(width, 0, 0);

		glColor3d(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, width, 0);

		glColor3d(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, width);
	}
	glEnd();
}

void CGLRenderer::DrawGrid(double width, double height, int nSegW, int nSegH)
{
	double wStep = width / nSegW;
	double hStep = height / nSegH;

	glBegin(GL_LINES);
	{
		glColor3d(GRID);
		for (double w = 0; w <= width; w += wStep) {
			glVertex3d(w, 0, 0);
			glVertex3d(w, 0, height);
		}
		for (double h = 0; h <= height; h += hStep) {
			glVertex3d(0, 0, h);
			glVertex3d(width, 0, h);
		}
	}
	glEnd();
}

void CGLRenderer::DrawFigure(double angle)
{
	double height = 1.25;
	double cactusCylinderR = 0.33;
	double sphereR = 0.25;
	double cylinderR = 0.33;
	double coneR = 0.4;

	glColor3d(VAZA);
	DrawCylinder(height, height, 1, 8);
	glTranslated(0, height, 0);
	DrawCylinder(cactusCylinderR, 1.5, 1.4, 8);

	glTranslated(0, 0.4, 0);
	glColor3d(DEO_KAKTUSA);
	DrawCylinder(height, 0.4, 0.4, 8);

	glTranslated(0, height, 0);

	glTranslated(0, sphereR, 0);
	glColor3d(SFERA);
	DrawSphere(sphereR, 100, 100);

	// Gornja grana
	glPushMatrix();
	{
		glRotated(0, 1, 0, 0);
		glTranslated(0, sphereR, 0);

		glColor3d(DEO_KAKTUSA);
		DrawCylinder(height, cylinderR, cylinderR, 8);

		glTranslated(0, height + sphereR, 0);
		glColor3d(SFERA);
		DrawSphere(sphereR, 100, 100);

		// gore desno
		glPushMatrix();
		{
			glRotated(-45, 1, 0, 0);
			glTranslated(0, sphereR, 0);
			glColor3d(DEO_KAKTUSA);
			DrawCylinder(height, cylinderR, cylinderR, 8);

			glTranslated(0, height + sphereR, 0);
			glColor3d(SFERA);
			DrawSphere(sphereR, 100, 100);

			glTranslated(0, sphereR, 0);
			glColor3d(DEO_KAKTUSA);
			DrawCylinder(height, cylinderR, cylinderR, 8);

			glTranslated(0, height + sphereR, 0);
			glColor3d(SFERA);
			DrawSphere(sphereR, 100, 100);
		}
		glPopMatrix();

		// gore levo
		glPushMatrix();
		{
			glRotated(angle + 45, 1, 0, 0);
			glTranslated(0, sphereR, 0);
			glColor3d(ZUTI_DEO_KAKTUSA);
			DrawCylinder(height, cylinderR, cylinderR, 8);

			glTranslated(0, height + sphereR, 0);
			glColor3d(SFERA);
			DrawSphere(sphereR, 100, 100);

			glTranslated(0, sphereR, 0);
			glColor3d(DEO_KAKTUSA);
			DrawCone(height, coneR, 8);

			glTranslated(0, height + sphereR, 0);
			glColor3d(SFERA);
			DrawSphere(sphereR, 100, 100);
		}
		glPopMatrix();
	}
	glPopMatrix();

	// Desna grana
	glPushMatrix();
	{
		glRotated(-45, 1, 0, 0);

		glTranslated(0, sphereR, 0);
		glColor3d(DEO_KAKTUSA);
		DrawCylinder(height, cylinderR, cylinderR, 8);

		glTranslated(0, height + sphereR, 0);
		glColor3d(SFERA);
		DrawSphere(sphereR, 100, 100);

		glTranslated(0, sphereR, 0);
		glColor3d(DEO_KAKTUSA);
		DrawCone(height, coneR, 8);

		glTranslated(0, height + sphereR, 0);
		glColor3d(SFERA);
		DrawSphere(sphereR, 100, 100);

		glTranslated(0, sphereR, 0);
		glColor3d(DEO_KAKTUSA);
		DrawCone(height, coneR, 8);

		glTranslated(0, height + sphereR, 0);
		glColor3d(SFERA);
		DrawSphere(sphereR, 100, 100);
	}
	glPopMatrix();

	// Leva grana
	glPushMatrix();
	{
		glRotated(45, 1, 0, 0);

		glTranslated(0, sphereR, 0);
		glColor3d(DEO_KAKTUSA);
		DrawCylinder(height, cylinderR, cylinderR, 8);

		glTranslated(0, height + sphereR, 0);
		glColor3d(SFERA);
		DrawSphere(sphereR, 100, 100);

		glTranslated(0, sphereR, 0);
		glColor3d(DEO_KAKTUSA);
		DrawCone(height, coneR, 8);

		glTranslated(0, height + sphereR, 0);
		glColor3d(SFERA);
		DrawSphere(sphereR, 100, 100);
	}
	glPopMatrix();
}

void CGLRenderer::RotateView(double dXY, double dXZ)
{
	this->viewAngleXY += dXY;
	viewAngleXY = min(90, viewAngleXY);
	viewAngleXY = max(-90, viewAngleXY);

	this->viewAngleXZ += dXZ;
	this->CalculatePosition();
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
