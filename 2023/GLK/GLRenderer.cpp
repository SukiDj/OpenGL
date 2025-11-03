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
	arm1Rot = 0;
	arm2Rot = 0;
	headRot = 0;
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

	lamp = LoadTexture("res/lamp.jpg");
	bckgr[0] = LoadTexture("res/side.jpg");
	bckgr[1] = LoadTexture("res/top.jpg");
	bckgr[2] = LoadTexture("res/bot.jpg");

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

	glPushMatrix();
	{
		glTranslated(0, 50, 0);
		DrawEnvCube(100.0);
	}
	glPopMatrix();

	DrawAxis();

	DrawLamp();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawAxis()
{
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_LIGHTING);
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
	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawEnvCube(double a)
{
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, bckgr[0]);
	glBegin(GL_QUADS);
	{
		//front
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);
		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);
		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, -a / 2);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, bckgr[0]);
	glBegin(GL_QUADS);
	{
		//back
		glTexCoord2f(0, 0);
		glVertex3f(a / 2, a / 2, a / 2);
		glTexCoord2f(0, 1);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 1);
		glVertex3f(-a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 0);
		glVertex3f(-a / 2, a / 2, a / 2);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, bckgr[0]);
	glBegin(GL_QUADS);
	{
		//left
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, a / 2);
		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 1);
		glVertex3f(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(1, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, bckgr[0]);
	glBegin(GL_QUADS);
	{
		//right
		glTexCoord2f(0, 0);
		glVertex3f(a / 2, a / 2, -a / 2);
		glTexCoord2f(0, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);
		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, bckgr[1]);
	glBegin(GL_QUADS);
	{
		//top
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, a / 2);
		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, a / 2, -a / 2);
		glTexCoord2f(1, 1);
		glVertex3f(a / 2, a / 2, -a / 2);
		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, bckgr[2]);
	glBegin(GL_QUADS);
	{
		//bot
		glTexCoord2f(0, 1);
		glVertex3f(a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);
		glTexCoord2f(1, 0);
		glVertex3f(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, -a / 2, a / 2);
	}
	glEnd();

	glDisable(GL_CULL_FACE);
}

void CGLRenderer::DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen)
{
	float offset;
	glBindTexture(GL_TEXTURE_2D, lamp);
	float alpha = 2 * 3.14159;
	float alphaStep = 2 * 3.14159 / nSeg;
	if (!bIsOpen)
	{
		offset = 0;
		glBegin(GL_TRIANGLE_FAN);
		{
			glTexCoord2f(0.5, 0.25);
			glVertex3f(0, 0, 0);
			for (int i = 0; i <= nSeg; i++)
			{
				float x = r1 * cos(alpha);
				float z = r1 * sin(alpha);
				glTexCoord2f(1 - (x / r1 * 0.5 + 0.5), z / r1 * 0.25 + 0.25);
				glVertex3f(x, 0, z);
				alpha -= alphaStep;
			}
		}
		glEnd();

		alpha = 0;
		glBegin(GL_TRIANGLE_FAN);
		{
			glTexCoord2f(0.5, 0.25);
			glVertex3f(0, h, 0);
			for (int i = 0; i <= nSeg; i++)
			{
				float x = r2 * cos(alpha);
				float z = r2 * sin(alpha);
				glTexCoord2f(x / r2 * 0.5 + 0.5, z / r2 * 0.25 + 0.25);
				glVertex3f(x, h, z);
				alpha += alphaStep;
			}
		}
		glEnd();
	}
	if (texMode == 1)
		offset = 0.5;
	else
		offset = 0;
	glBegin(GL_QUAD_STRIP);
	{
		float seg = 1.0 / nSeg;
		alpha = 0;
		for (int i = 0; i <= nSeg; i++)
		{
			float x1 = r1 * cos(alpha);
			float z1 = r1 * sin(alpha);

			glTexCoord2f(1.0 - i * seg, 0.5 + offset);
			glVertex3f(x1, 0, z1);

			float x2 = r2 * cos(alpha);
			float z2 = r2 * sin(alpha);

			glTexCoord2f(1.0 - i * seg, 0 + offset);
			glVertex3f(x2, h, z2);

			alpha += alphaStep;
		}
	}
	glEnd();
}

void CGLRenderer::DrawLampBase()
{
	DrawCylinder(8, 7, 2, 10, 1, false);
}

void CGLRenderer::DrawLampArm()
{
	glPushMatrix();
	{
		glTranslated(0, 1, 0);
		glRotated(90, 1, 0, 0);
		glTranslated(0, -1, 0);
		DrawCylinder(3, 3, 2, 20, 1, false);
	}
	glPopMatrix();

	glTranslated(0, 3, 0);
	DrawCylinder(1, 1, 15, 20, 1, false);

}

void CGLRenderer::DrawLampHead()
{
	glPushMatrix();
	{
		glTranslated(0, 1, 0);
		glRotated(90, 1, 0, 0);
		glTranslated(0, -1, 0);
		DrawCylinder(2, 2, 1, 20, 1, false);
	}
	glPopMatrix();
	glTranslated(0, 0, -0.5);
	glPushMatrix();
	{
		glTranslated(-3, -4, 0);
		glTranslated(0, 1.5, 0);
		glRotated(180, 1, 0, 0);
		glTranslated(0, -1.5, 0);
		DrawCylinder(3, 2, 1, 20, 1, false);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslated(-3, -1, 0);
		DrawCylinder(3, 3, 5, 20, 1, false);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslated(-3, 3.5, 0);
		DrawCylinder(3, 6, 5, 20, 0, true);
	}
	glPopMatrix();
}

void CGLRenderer::DrawLamp()
{
	DrawLampBase();
	glTranslated(0, 1, 0);
	glRotated(30 + arm1Rot, 0, 0, 1);
	glTranslated(0, -1, 0);
	DrawLampArm();
	glTranslated(0, 15, 0);
	glTranslated(0, 1, 0);
	glRotated(-60 + arm2Rot, 0, 0, 1);
	glTranslated(0, -1, 0);
	DrawLampArm();
	glTranslated(-1, 15, 0.5);
	glTranslated(1, 0, 0);
	glRotated(-60 + headRot, 0, 0, 1);
	glTranslated(-1, 0, 0);
	DrawLampHead();
}

void CGLRenderer::UpdateView()
{
	if (dist < 8)
		dist = 8;
	if (dist > 50)
		dist = 50;
	glTranslated(0, 0, -dist);
	glRotated(alphaRot, 0, 1, 0);
	glRotated(betaRot, 0, 0, 1);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / (double)h, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glDeleteTextures(1, &lamp);
	glDeleteTextures(3, bckgr);
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}