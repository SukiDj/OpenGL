#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	m_hrc = NULL;

	dist = 0;
	alphaRot = 0;
	betaRot = 0;

	lightON = true;
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

	brick = LoadTexture("res/brick.png");
	env = LoadTexture("res/env.png");

	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	UpdateView();
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	DrawCube(50);

	AmbientLight();
	if (lightON)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	GLfloat light_pos[] = { 1.0, 1.0, 0.5, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	DrawAxis();

	glPushMatrix();
	glPushMatrix();
	glTranslated(0, -10, -20);
	DrawTube(2.5, 3.5, 10, 32);
	glTranslated(0, 10, 0);
	DrawCone(3.8, 2, 32);
	glPopMatrix();
	glTranslated(0, 0, -16.2);
	glRotated(-90, 0, 1, 0);
	DrawPaddle(8, 1.5);
	glRotated(-90, 1, 0, 0);
	DrawPaddle(8, 1.5);
	glRotated(-90, 1, 0, 0);
	DrawPaddle(8, 1.5);
	glRotated(-90, 1, 0, 0);
	DrawPaddle(8, 1.5);

	glPopMatrix();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawAxis()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glLineWidth(2);
	glBegin(GL_LINES);
	{
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(10, 0, 0);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 10, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 10);
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawCube(double a)
{
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	float segx = 1.0 / 4.0;
	float segy = 1.0 / 3.0;
	glBindTexture(GL_TEXTURE_2D, env);
	// left
	glBegin(GL_QUADS);
	{
		glTexCoord2f(segx, 2 * segy);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(segx, segy);
		glVertex3f(-a / 2, -a / 2, a / 2);
		glTexCoord2f(0, segy);
		glVertex3f(-a / 2, a / 2, a / 2);
		glTexCoord2f(0, 2 * segy);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();
	// bot
	glBegin(GL_QUADS);
	{
		glTexCoord2f(segx, 2 * segy);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(2 * segx,  2 * segy);
		glVertex3f(a / 2, -a / 2, -a / 2);
		glTexCoord2f(2 * segx, segy);
		glVertex3f(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(segx, segy);
		glVertex3f(-a / 2, -a / 2, a / 2);
	}
	glEnd();
	// right
	glBegin(GL_QUADS);
	{
		glTexCoord2f(2 * segx, 2 * segy);
		glVertex3f(a / 2, -a / 2, -a / 2);
		glTexCoord2f(3 * segx, 2 * segy);
		glVertex3f(a / 2, a / 2, -a / 2);
		glTexCoord2f(3 * segx, segy);
		glVertex3f(-a / 2, a / 2, -a / 2);
		glTexCoord2f(2 * segx, segy);
		glVertex3f(-a / 2, -a / 2, -a / 2);
	}
	glEnd();
	// top
	glBegin(GL_QUADS);
	{
		glTexCoord2f(3 * segx, 2 * segy);
		glVertex3f(a / 2, a / 2, -a / 2);
		glTexCoord2f(4 * segx, 2 * segy);
		glVertex3f(a / 2, a / 2, a / 2);
		glTexCoord2f(4 * segx, segy);
		glVertex3f(-a / 2, a / 2, a / 2);
		glTexCoord2f(3 * segx, segy);
		glVertex3f(-a / 2, a / 2, -a / 2);
	}
	glEnd();
	// front
	glBegin(GL_QUADS);
	{
		glTexCoord2f(segx, segy);
		glVertex3f(-a / 2, -a / 2, a / 2);
		glTexCoord2f(2 * segx, segy);
		glVertex3f(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(2 * segx, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);
		glTexCoord2f(segx, 0);
		glVertex3f(-a / 2, a / 2, a / 2);
	}
	glEnd();
	// back
	glBegin(GL_QUADS);
	{
		glTexCoord2f(segx, 2 * segy);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(segx, 1);
		glVertex3f(a / 2, a / 2, a / 2);
		glTexCoord2f(2 * segx, 1);
		glVertex3f(a / 2, a / 2, -a / 2);
		glTexCoord2f(2 * segx, 2 * segy);
		glVertex3f(a / 2, -a / 2, -a / 2);
	}
	glEnd();

	glDisable(GL_CULL_FACE);
}

void CGLRenderer::DrawTube(double r1, double r2, double h, int n)
{
	glBindTexture(GL_TEXTURE_2D, brick);
	float alpha = 2 * 3.14159;
	float alphaStep = 2 * 3.14159 / n;
	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, -1, 0);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= n; i++)
		{
			float x = r2 * cos(alpha);
			float z = r2 * sin(alpha);
			glVertex3f(x, 0, z);
			alpha -= alphaStep;
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	{
		alpha = 0;

		glNormal3f(0, 1, 0);
		glVertex3f(0, h, 0);
		for (int i = 0; i <= n; i++)
		{
			float x = r1 * cos(alpha);
			float z = r1 * sin(alpha);
			glVertex3f(x, h, z);
			alpha += alphaStep;
		}
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	{
		alpha = 0;
		for (int i = 0; i <= n; i++)
		{
			float x1 = r2 * cos(alpha);
			float z1 = r2 * sin(alpha);

			float x2 = r1 * cos(alpha);
			float z2 = r1 * sin(alpha);

			glNormal3f(x1 / r2, 0, z1 / r2);
			glTexCoord2f((float)i / n, 0);
			glVertex3f(x1, 0, z1);
			glNormal3f(x2 / r1, 0, z2 / r1);
			glTexCoord2f((float)i / n, 1);
			glVertex3f(x2, h, z2);

			alpha += alphaStep;
		}
	}
	glEnd();
}

void CGLRenderer::DrawCone(double r, double h, int n)
{
	glDisable(GL_TEXTURE_2D);
	float alpha = 0;
	float alphaStep = 2 * 3.14159 / n;
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, 1, 0);
		glVertex3f(0, h, 0);
		for (int i = 0; i <= n; i++)
		{
			float x = r * cos(alpha);
			float z = r * sin(alpha);

			glNormal3f(x / r, 1, z / r);
			glVertex3f(x, 0, z);

			alpha += alphaStep;
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, -1, 0);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= n; i++)
		{
			float x = r * cos(alpha);
			float z = r * sin(alpha);

			glNormal3f(x / r, -1, z / r);
			glVertex3f(x, 0, z);
			alpha += alphaStep;
		}
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawPaddle(double length, double width)
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.8, 0.8, 0.8);
	glNormal3f(1, 0, 0);
	glBegin(GL_QUADS);
	{
		glVertex3f(0, 0, 0);
		glVertex3f(0, -1.0 / 8.0 * width, 0);
		glVertex3f(0, -1.0 / 8.0 * width, -1.0 / 8.0 * length);
		glVertex3f(0, 0, -1.0 / 8.0 * length);
	}
	glEnd();
	glBegin(GL_POLYGON);
	{
		glVertex3f(0, 0, -1.0 / 8.0 * length);
		glVertex3f(0, -1.0 / 8.0 * width, -1.0 / 8.0 * length);
		glVertex3f(0, -width, -2.0 / 8.0 * length);
		glVertex3f(0, -width, -6.0 / 8.0 * length);
		glVertex3f(0, 0, -6.0 / 8.0 * length);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::UpdateView()
{
	if (dist < 8)
		dist = 8;
	if (dist > 25)
		dist = 25;
	glTranslated(0, 0, -dist);
	glRotated(alphaRot, 0, 1, 0);
	glRotated(betaRot, 1, 0, 0);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / (double)h, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glDeleteTextures(1, &brick);
	glDeleteTextures(1, &env);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());


	return texID;
}

void CGLRenderer::AmbientLight()
{
	float light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}
