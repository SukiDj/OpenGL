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

	this->viewR = 20;
	this->viewAngleXY = 0;
	this->viewAngleXZ = 0;

	grassMat.SetAmbient(0.2, 0.2, 0.2, 1);
	grassMat.SetDiffuse(0.6, 0.6, 0.6, 1);
	grassMat.SetEmission(0, 0, 0, 1);

	truckMat.SetAmbient(0.2, 0.2, 0.2, 1);
	truckMat.SetDiffuse(0.6, 0.6, 0.6, 1);
	truckMat.SetEmission(0, 0, 0, 1);

	grassTex = new GLTexture();
	truckTex = new GLTexture();

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
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	AmbientLight();

	grassTex->LoadTexture(CString("res/grass.jpeg"));

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);


	glEnable(GL_LIGHTING);

	float light_position[] = { 7, 10, -10, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);

	DrawGrass(20);

	DrawTruck();

	DrawAxis(100);

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawAxis(double width)
{
	bool lightingEnabled = glIsEnabled(GL_LIGHTING);
	if (lightingEnabled)
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
	if (lightingEnabled)
		glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawGrass(double a)
{
	grassMat.SetFace(GL_FRONT_AND_BACK);
	grassTex->PrepareTexture(glIsEnabled(GL_LIGHTING));
	grassTex->Select();
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	{
		glNormal3f(0.0, 1.0, 0.0);

		glTexCoord2f(0.0, 2.0);
		glVertex3f(a, -2, -a);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-a, -2, -a);
		glTexCoord2f(2.0, 0.0);
		glVertex3f(-a, -2, a);
		glTexCoord2f(2.0, 2.0);
		glVertex3f(a, -2, a);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawTruck()
{
	DrawQuads();
	DrawSide(true);
	DrawSide(false);
	DrawWheels();
	glPushMatrix();
	glTranslated(2, 5, 0);
	glScaled(2.2, 1, 1);
	DrawSphere();
	glPopMatrix();
}

void CGLRenderer::DrawSide(bool left)
{
	truckMat.SetFace(GL_FRONT);
	truckTex->PrepareTexture(glIsEnabled(GL_LIGHTING));
	truckTex->LoadTexture(CString("res/truck.png"));
	truckTex->Select();
	glEnable(GL_TEXTURE_2D);
	
	float width = 5;
	float height = 3;
	float seg = 1.0 / 16.0;
	if (left)
	{
		glNormal3f(0.0, 0.0, 1.0);
		glBegin(GL_POLYGON);
		{
			glTexCoord2f(4 * seg, 4 * seg);
			glVertex3f(-2.5, height, width / 2);
			glTexCoord2f(4 * seg, 0);
			glVertex3f(-2.5, height + height + 1, width / 2);
			glTexCoord2f(2 * seg, 0);
			glVertex3f(-4.5, height + height + 1, width / 2);
			glTexCoord2f(0, 2 * seg);
			glVertex3f(-6.5, height + 2, width / 2);
			glTexCoord2f(0, 4 * seg);
			glVertex3f(-6.5, height, width / 2);
		}
		glEnd();
		glBegin(GL_QUADS);
		{
			glTexCoord2f(13 * seg, 5 * seg);
			glVertex3f(6.5, 2, width / 2);
			glTexCoord2f(13 * seg, 4 * seg);
			glVertex3f(6.5, height, width / 2);
			glTexCoord2f(0, 4 * seg);
			glVertex3f(-6.5, height, width / 2);
			glTexCoord2f(0, 5 * seg);
			glVertex3f(-6.5, 2, width / 2);

			glTexCoord2f(13 * seg, 7 * seg);
			glVertex3f(6.5, 0, width / 2);
			glTexCoord2f(13 * seg, 5 * seg);
			glVertex3f(6.5, 2, width / 2);
			glTexCoord2f(9 * seg, 5 * seg);
			glVertex3f(2.5, 2, width / 2);
			glTexCoord2f(9 * seg, 7 * seg);
			glVertex3f(2.5, 0, width / 2);

			glTexCoord2f(6 * seg, 7 * seg);
			glVertex3f(-0.5, 0, width / 2);
			glTexCoord2f(6 * seg, 5 * seg);
			glVertex3f(-0.5, 2, width / 2);
			glTexCoord2f(4 * seg, 5 * seg);
			glVertex3f(-2.5, 2, width / 2);
			glTexCoord2f(4 * seg, 7 * seg);
			glVertex3f(-2.5, 0, width / 2);

			glTexCoord2f(seg, 7 * seg);
			glVertex3f(-5.5, 0, width / 2);
			glTexCoord2f(seg, 5 * seg);
			glVertex3f(-5.5, 2, width / 2);
			glTexCoord2f(0, 5 * seg);
			glVertex3f(-6.5, 2, width / 2);
			glTexCoord2f(0, 7 * seg);
			glVertex3f(-6.5, 0, width / 2);
		}
		glEnd();
		glBegin(GL_TRIANGLES);
		{
			glTexCoord2f(9 * seg, 6 * seg);
			glVertex3f(2.5, 1, width / 2);
			glTexCoord2f(9 * seg, 5 * seg);
			glVertex3f(2.5, 2, width / 2);
			glTexCoord2f(8 * seg, 5 * seg);
			glVertex3f(1.5, 2, width / 2);

			glTexCoord2f(6 * seg, 6 * seg);
			glVertex3f(-0.5, 1, width / 2);
			glTexCoord2f(7 * seg, 5 * seg);
			glVertex3f(0.5, 2, width / 2);
			glTexCoord2f(6 * seg, 5 * seg);
			glVertex3f(-0.5, 2, width / 2);

			glTexCoord2f(4 * seg, 6 * seg);
			glVertex3f(-2.5, 1, width / 2);
			glTexCoord2f(4 * seg, 5 * seg);
			glVertex3f(-2.5, 2, width / 2);
			glTexCoord2f(3 * seg, 5 * seg);
			glVertex3f(-3.5, 2, width / 2);

			glTexCoord2f(1 * seg, 6 * seg);
			glVertex3f(-5.5, 1, width / 2);
			glTexCoord2f(2 * seg, 5 * seg);
			glVertex3f(-4.5, 2, width / 2);
			glTexCoord2f(1 * seg, 5 * seg);
			glVertex3f(-5.5, 2, width / 2);
		}
		glEnd();
	}
	else
	{
		glNormal3f(0.0, 0.0, -1.0);
		width = -width;
		glBegin(GL_POLYGON);
		{
			glTexCoord2f(0, 4 * seg);
			glVertex3f(-6.5, 3, width / 2);
			glTexCoord2f(0, 2 * seg);
			glVertex3f(-6.5, height + 2, width / 2);
			glTexCoord2f(2 * seg, 0);
			glVertex3f(-4.5, height + height + 1, width / 2);
			glTexCoord2f(4 * seg, 0);
			glVertex3f(-2.5, height + height + 1, width / 2);
			glTexCoord2f(4 * seg, 4 * seg);
			glVertex3f(-2.5, height, width / 2);
		}
		glEnd();
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 5 * seg);
			glVertex3f(-6.5, 2, width / 2);
			glTexCoord2f(0, 4 * seg);
			glVertex3f(-6.5, height, width / 2);
			glTexCoord2f(13 * seg, 4 * seg);
			glVertex3f(6.5, height, width / 2);
			glTexCoord2f(13 * seg, 5 * seg);
			glVertex3f(6.5, 2, width / 2);

			glTexCoord2f(9 * seg, 7 * seg);
			glVertex3f(2.5, 0, width / 2);
			glTexCoord2f(9 * seg, 5 * seg);
			glVertex3f(2.5, 2, width / 2);
			glTexCoord2f(13 * seg, 5 * seg);
			glVertex3f(6.5, 2, width / 2);
			glTexCoord2f(13 * seg, 7 * seg);
			glVertex3f(6.5, 0, width / 2);

			glTexCoord2f(4 * seg, 7 * seg);
			glVertex3f(-2.5, 0, width / 2);
			glTexCoord2f(4 * seg, 5 * seg);
			glVertex3f(-2.5, 2, width / 2);
			glTexCoord2f(6 * seg, 5 * seg);
			glVertex3f(-0.5, 2, width / 2);
			glTexCoord2f(6 * seg, 7 * seg);
			glVertex3f(-0.5, 0, width / 2);

			glTexCoord2f(0, 7 * seg);
			glVertex3f(-6.5, 0, width / 2);
			glTexCoord2f(0, 5 * seg);
			glVertex3f(-6.5, 2, width / 2);
			glTexCoord2f(seg, 5 * seg);
			glVertex3f(-5.5, 2, width / 2);
			glTexCoord2f(seg, 7 * seg);
			glVertex3f(-5.5, 0, width / 2);
		}
		glEnd();
		glBegin(GL_TRIANGLES);
		{
			glTexCoord2f(9 * seg, 6 * seg);
			glVertex3f(2.5, 1, width / 2);
			glTexCoord2f(8 * seg, 5 * seg);
			glVertex3f(1.5, 2, width / 2);
			glTexCoord2f(9 * seg, 5 * seg);
			glVertex3f(2.5, 2, width / 2);

			glTexCoord2f(6 * seg, 6 * seg);
			glVertex3f(-0.5, 1, width / 2);
			glTexCoord2f(6 * seg, 5 * seg);
			glVertex3f(-0.5, 2, width / 2);
			glTexCoord2f(7 * seg, 5 * seg);
			glVertex3f(0.5, 2, width / 2);

			glTexCoord2f(4 * seg, 6 * seg);
			glVertex3f(-2.5, 1, width / 2);
			glTexCoord2f(3 * seg, 5 * seg);
			glVertex3f(-3.5, 2, width / 2);
			glTexCoord2f(4 * seg, 5 * seg);
			glVertex3f(-2.5, 2, width / 2);

			glTexCoord2f(1 * seg, 6 * seg);
			glVertex3f(-5.5, 1, width / 2);
			glTexCoord2f(1 * seg, 5 * seg);
			glVertex3f(-5.5, 2, width / 2);
			glTexCoord2f(2 * seg, 5 * seg);
			glVertex3f(-4.5, 2, width / 2);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawQuads()
{
	truckMat.SetFace(GL_FRONT);
	float width = 5;
	float height = 3;

	//glBegin(GL_QUAD_STRIP);
	//{
	//	// zadnja
	//	glNormal3f(1.0, 0.0, 0.0);
	//	glVertex3f(6.5, 0, width/2);
	//	glVertex3f(6.5, 0, -width/2);
	//	glVertex3f(6.5, height, width/2);
	//	glVertex3f(6.5,  height, -width/2);

	//	//gornji deo prikolica
	//	glNormal3f(0.0, 1.0, 0.0);
	//	glVertex3f(-2.5, height, width / 2);
	//	glVertex3f(-2.5, height, -width / 2);

	//	//zadnji deo kabina
	//	glNormal3f(1.0, 0.0, 0.0);
	//	glVertex3f(-2.5, height + height + 1, width / 2);
	//	glVertex3f(-2.5, height + height + 1, -width / 2);

	//	// krov kabine
	//	glNormal3f(0.0, 1.0, 0.0);
	//	glVertex3f(-4.5, height + height + 1, width / 2);
	//	glVertex3f(-4.5, height + height + 1, -width / 2);

	//	// soferka
	//	glNormal3f(-1.0, 1.0, 0.0);
	//	glVertex3f(-6.5, height + 2, width / 2);
	//	glVertex3f(-6.5, height + 2, -width / 2);

	//	// prednja strana
	//	glNormal3f(-1.0, 0.0, 0.0);
	//	glVertex3f(-6.5, 0, width / 2);
	//	glVertex3f(-6.5, 0, -width / 2);

	//	// donja strana
	//	glNormal3f(0.0, -1.0, 0.0);
	//	glVertex3f(-5.5, 0, width / 2);
	//	glVertex3f(-5.5, 0, -width / 2);

	//	glVertex3f(-5.5, 1, width / 2);
	//	glVertex3f(-5.5, 1, -width / 2);

	//	glVertex3f(-4.5, 2, width / 2);
	//	glVertex3f(-4.5, 2, -width / 2);

	//	glVertex3f(-3.5, 2, width / 2);
	//	glVertex3f(-3.5, 2, -width / 2);

	//	glVertex3f(-2.5, 1, width / 2);
	//	glVertex3f(-2.5, 1, -width / 2);

	//	glVertex3f(-2.5, 0, width / 2);
	//	glVertex3f(-2.5, 0, -width / 2);

	//	glVertex3f(-0.5, 0, width / 2);
	//	glVertex3f(-0.5, 0, -width / 2);

	//	glVertex3f(-0.5, 1, width / 2);
	//	glVertex3f(-0.5, 1, -width / 2);

	//	glVertex3f(0.5, 2, width / 2);
	//	glVertex3f(0.5, 2, -width / 2);

	//	glVertex3f(1.5, 2, width / 2);
	//	glVertex3f(1.5, 2, -width / 2);

	//	glVertex3f(2.5, 1, width / 2);
	//	glVertex3f(2.5, 1, -width / 2);

	//	glVertex3f(2.5, 0, width / 2);
	//	glVertex3f(2.5, 0, -width / 2);

	//	glVertex3f(6.5, 0, width / 2);
	//	glVertex3f(6.5, 0, -width / 2);
	//}
	glBegin(GL_QUADS);
	{
		// zadnja
		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(6.5, 0, width / 2);
		glVertex3f(6.5, 0, -width / 2);
		glVertex3f(6.5, height, -width / 2);
		glVertex3f(6.5, height, width / 2);

		//gornji deo prikolica
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(6.5, height, width / 2);
		glVertex3f(6.5, height, -width / 2);
		glVertex3f(-2.5, height, -width / 2);
		glVertex3f(-2.5, height, width / 2);

		//zadnji deo kabina
		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(-2.5, height, width / 2);
		glVertex3f(-2.5, height, -width / 2);
		glVertex3f(-2.5, height + height + 1, -width / 2);
		glVertex3f(-2.5, height + height + 1, width / 2);

		// krov kabine
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(-2.5, height + height + 1, width / 2);
		glVertex3f(-2.5, height + height + 1, -width / 2);
		glVertex3f(-4.5, height + height + 1, -width / 2);
		glVertex3f(-4.5, height + height + 1, width / 2);

		// soferka
		glNormal3f(-1.0, 1.0, 0.0);
		glVertex3f(-4.5, height + height + 1, width / 2);
		glVertex3f(-4.5, height + height + 1, -width / 2);
		glVertex3f(-6.5, height + 2, -width / 2);
		glVertex3f(-6.5, height + 2, width / 2);

		// prednja strana
		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(-6.5, height + 2, width / 2);
		glVertex3f(-6.5, height + 2, -width / 2);
		glVertex3f(-6.5, 0, -width / 2);
		glVertex3f(-6.5, 0, width / 2);

		// donja strana
		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(-6.5, 0, width / 2);
		glVertex3f(-6.5, 0, -width / 2);
		glVertex3f(-5.5, 0, -width / 2);
		glVertex3f(-5.5, 0, width / 2);

		glVertex3f(-5.5, 0, width / 2);
		glVertex3f(-5.5, 0, -width / 2);
		glVertex3f(-5.5, 1, -width / 2);
		glVertex3f(-5.5, 1, width / 2);

		glVertex3f(-5.5, 1, width / 2);
		glVertex3f(-5.5, 1, -width / 2);
		glVertex3f(-4.5, 2, -width / 2);
		glVertex3f(-4.5, 2, width / 2);

		glVertex3f(-4.5, 2, width / 2);
		glVertex3f(-4.5, 2, -width / 2);
		glVertex3f(-3.5, 2, -width / 2);
		glVertex3f(-3.5, 2, width / 2);

		glVertex3f(-3.5, 2, width / 2);
		glVertex3f(-3.5, 2, -width / 2);
		glVertex3f(-2.5, 1, -width / 2);
		glVertex3f(-2.5, 1, width / 2);

		glVertex3f(-2.5, 1, width / 2);
		glVertex3f(-2.5, 1, -width / 2);
		glVertex3f(-2.5, 0, -width / 2);
		glVertex3f(-2.5, 0, width / 2);

		glVertex3f(-2.5, 0, width / 2);
		glVertex3f(-2.5, 0, -width / 2);
		glVertex3f(-0.5, 0, -width / 2);
		glVertex3f(-0.5, 0, width / 2);

		glVertex3f(-0.5, 0, width / 2);
		glVertex3f(-0.5, 0, -width / 2);
		glVertex3f(-0.5, 1, -width / 2);
		glVertex3f(-0.5, 1, width / 2);

		glVertex3f(-0.5, 1, width / 2);
		glVertex3f(-0.5, 1, -width / 2);
		glVertex3f(0.5, 2, -width / 2);
		glVertex3f(0.5, 2, width / 2);

		glVertex3f(0.5, 2, width / 2);
		glVertex3f(0.5, 2, -width / 2);
		glVertex3f(1.5, 2, -width / 2);
		glVertex3f(1.5, 2, width / 2);

		glVertex3f(1.5, 2, width / 2);
		glVertex3f(1.5, 2, -width / 2);
		glVertex3f(2.5, 1, -width / 2);
		glVertex3f(2.5, 1, width / 2);

		glVertex3f(2.5, 1, width / 2);
		glVertex3f(2.5, 1, -width / 2);
		glVertex3f(2.5, 0, -width / 2);
		glVertex3f(2.5, 0, width / 2);

		glVertex3f(2.5, 0, width / 2);
		glVertex3f(2.5, 0, -width / 2);
		glVertex3f(6.5, 0, -width / 2);
		glVertex3f(6.5, 0, width / 2);
	}
	glEnd();
}

void CGLRenderer::DrawWheels()
{
	truckMat.SetFace(GL_FRONT);
	float center;
	float z1;
	float z2;

	center = 1;
	z1 = 2.5;
	z2 = 1.5;
	DrawWheel(center, z1, z2);
	center = 1;
	z1 = -2.5;
	z2 = -1.5;
	DrawWheel(center, z1, z2);
	center = -4;
	z1 = 2.5;
	z2 = 1.5;
	DrawWheel(center, z1, z2);
	center = -4;
	z1 = -2.5;
	z2 = -1.5;
	DrawWheel(center, z1, z2);
	
}

void CGLRenderer::DrawWheel(float center, float z1, float z2)
{
	float seg = 1.0 / 16.0;
	truckTex->PrepareTexture(glIsEnabled(GL_LIGHTING));
	truckTex->LoadTexture(CString("res/truck.png"));
	truckTex->Select();
	glEnable(GL_TEXTURE_2D);
	int alphaStep = 360 / 36;
	float r = 1.5;

	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(6 * seg, 1.5 * seg);
		glVertex3f(center, 0, z1);
		for (int alpha = 360; alpha >= 0; alpha -= alphaStep)
		{
			float x = r * cos(toRad(alpha));
			float y = r * sin(toRad(alpha));
			glTexCoord2f(6 * seg + 1.5 * seg * cos(toRad(alpha)), 1.5 * seg + 1.5 * seg * sin(toRad(alpha)));
			glVertex3f(x + center, y, z1);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(6 * seg, 1.5 * seg);
		glVertex3f(center, 0, z2);
		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			float x = r * cos(toRad(alpha));
			float y = r * sin(toRad(alpha));
			glTexCoord2f(6 * seg + 1.5 * seg * cos(toRad(alpha)), 1.5 * seg + 1.5 * seg * sin(toRad(alpha)));
			glVertex3f(x + center, y, z2);
		}
	}
	glEnd();
	glBegin(GL_QUAD_STRIP);
	{
		for (int alpha = 360; alpha >= 0; alpha -= alphaStep) {
			double x1 = r * cos(toRad(alpha));
			double y1 = r * sin(toRad(alpha));

			double x2 = r * cos(toRad(alpha));
			double y2 = r * sin(toRad(alpha));

			glVertex3d(x1 + center, y1, z1);
			glVertex3d(x2 + center, y2, z2);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawSphere()
{
	float seg = 1.0 / 16.0;
	int alphaStep = 360 / 32;
	int betaStep = 360 / 32;
	float r = 2;
	double nx = 0.0;
	double ny = 0.0;
	double nz = 0.0;
	truckTex->PrepareTexture(glIsEnabled(GL_LIGHTING));
	truckTex->LoadTexture(CString("res/truck.png"));
	truckTex->Select();
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUAD_STRIP);
	for(int alpha = 90; alpha > -90; alpha -= alphaStep)
	{
		for(int beta = 0; beta < 360; beta += betaStep)
		{
			double x1 = r * cos(toRad(alpha)) * cos(toRad(beta));
			double y1 = r * sin(toRad(alpha)); 
			double z1 = r * cos(toRad(alpha)) * sin(toRad(beta)); 
			
			nx = x1 / r;
			ny = y1 / r;
			nz = z1 / r;
			double tx1 = atan2(nx, nz) / (2. * PI) + 0.5;
			double ty1 = asin(ny) / PI + .5;
			glNormal3d(nx, ny, nz);
			glTexCoord2f(tx1, (16 - ty1) * 0.5);
			glVertex3d(x1, y1, z1);

			double x2 = r * cos(toRad(alpha) - toRad(alphaStep)) * cos(toRad(beta));
			double y2 = r * sin(toRad(alpha) - toRad(alphaStep));
			double z2 = r * cos(toRad(alpha) - toRad(alphaStep)) * sin(toRad(beta));
			nx = x2 / r;
			ny = y2 / r;
			nz = z2 / r;
			double tx = atan2(nx, nz) / (2. * PI) + 0.5;
			double ty = asin(ny) / PI + .5;
			if (tx < 0.75 && tx1 > 0.75)
				tx += 1.0;
			else if (tx > 0.75 && tx1 < 0.75)
				tx -= 1.0;
			glNormal3d(nx, ny, nz);
			glTexCoord2f(tx, (16 - ty) * 0.5);
			glVertex3d(x2, y2, z2);

		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / (double)h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	grassTex->ReleaseTex();
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::CalculatePosition()
{
	double dWXY = toRad(this->viewAngleXY);
	double dWXZ = toRad(this->viewAngleXZ);

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
