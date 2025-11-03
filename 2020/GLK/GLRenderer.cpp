#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
//#pragma comment(lib, "GL\\glut32.lib")

#define PI 3.14159

CGLRenderer::CGLRenderer(void)
{
	m_hrc = NULL;
	dist = 8;
	angle1 = 0;
	angle2 = 0;
	light = false;
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

	spider = LoadTexture("res/spider.png");
	envCube[0] = LoadTexture("res/front.jpg");
	envCube[1] = LoadTexture("res/back.jpg");
	envCube[2] = LoadTexture("res/left.jpg");
	envCube[3] = LoadTexture("res/right.jpg");
	envCube[4] = LoadTexture("res/top.jpg");
	envCube[5] = LoadTexture("res/bot.jpg");


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
	UpdateCamera();


	glPushMatrix();
	{
		glTranslated(0, 30, 0);
		DrawEnvCube(100);
	}
	glPopMatrix();

	glTranslated(0, -15, 0);
	DrawSpider();

	//DrawAxis();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawAxis()
{
	float width = 50;
	glLineWidth(2.0);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);
	{
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(width, 0, 0);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, width, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, width);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawEnvCube(double a)
{
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, envCube[0]);
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
	glBindTexture(GL_TEXTURE_2D, envCube[1]);
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
	glBindTexture(GL_TEXTURE_2D, envCube[2]);
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
	glBindTexture(GL_TEXTURE_2D, envCube[3]);
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
	glBindTexture(GL_TEXTURE_2D, envCube[4]);
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
	glBindTexture(GL_TEXTURE_2D, envCube[5]);
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

void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR)
{
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spider);*/
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	float alphaStep = PI / (double)nSeg;
	float betaStep = 2 * PI / (double)nSeg;
	for (float alpha = -PI/2; alpha < PI/2; alpha += alphaStep)
	{
		glBegin(GL_QUAD_STRIP);
		{
			for (float beta = 0; beta <= 2 * PI; beta += betaStep)
			{
				float x1 = r * cos(alpha) * cos(beta);
				float y1 = r * sin(alpha);
				float z1 = r * cos(alpha) * sin(beta);

				float tx1 = x1 / r * texR + texU;
				float ty1 = z1 / r * texR + texV;
				glNormal3f(x1 / r, y1 / r, z1 / r);
				glTexCoord2f(tx1, ty1);
				glVertex3f(x1, y1, z1);

				float x2 = r * cos(alpha + alphaStep) * cos(beta);
				float y2 = r * sin(alpha + alphaStep);
				float z2 = r * cos(alpha + alphaStep) * sin(beta);

				float tx2 = x2 / r * texR + texU;
				float ty2 = z2 / r * texR + texV;
				glNormal3f(x2 / r, y2 / r, z2 / r);
				glTexCoord2f(tx2, ty2);
				glVertex3f(x2, y2, z2);
			}
		}
		glEnd();
	}
	glDisable(GL_CULL_FACE);
}

void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	float alpha = 0;
	float alphaStep = 2 * PI / (double)nSeg;
	// omotac
	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(texU, texV);
		glVertex3f(0, h, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			float x = r * cos(alpha);
			float z = r * sin(alpha);

			float tx = x / r * texR + texU;
			float ty = z / r * texR + texV;
			glTexCoord2f(tx, ty);
			glVertex3f(x, 0, z);

			alpha += alphaStep;
		}
	}
	glEnd();
}

void CGLRenderer::DrawLegSegment(double r, double h, int nSeg)
{
	DrawSphere(r, 2 * nSeg, 0.25, 0.25, 0.24);
	DrawCone(r, h, nSeg, 0.75, 0.75, 0.24);
}

void CGLRenderer::DrawLeg()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spider);
	glPushMatrix();
	{
		DrawLegSegment(1, 10, 5);
		glTranslated(0, 10, 0);
		glRotated(85, 1, 0, 0);
		DrawLegSegment(1, 15, 5);
	}
	glPopMatrix();
}

void CGLRenderer::DrawSpiderBody()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spider);
	glPushMatrix();
	{
		glTranslated(-4, 0, 0);
		glScaled(1, 0.5, 1);
		DrawSphere(2, 10, 0.75, 0.25, 0.24);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glScaled(1, 0.5, 1);
		DrawSphere(3, 10, 0.25, 0.25, 0.24);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslated(7, 0, 0);
		glScaled(1, 0.8, 1);
		DrawSphere(5, 10, 0.25, 0.25, 0.24);
	}
	glPopMatrix();
}

void CGLRenderer::DrawSpider()
{
	DrawSpiderBody();

	// levo
	glRotated(-45, 0, 1, 0);
	glPushMatrix();
	{
		glRotated(45, 1, 0, 0);
		DrawLeg();
	}
	glPopMatrix();
	glRotated(30, 0, 1, 0);
	glPushMatrix();
	{
		glRotated(45, 1, 0, 0);
		DrawLeg();
	}
	glPopMatrix();
	glRotated(30, 0, 1, 0);
	glPushMatrix();
	{
		glRotated(45, 1, 0, 0);
		DrawLeg();
	}
	glPopMatrix();
	glRotated(30, 0, 1, 0);
	glPushMatrix();
	{
		glRotated(45, 1, 0, 0);
		DrawLeg();
	}
	glPopMatrix();
	
	// desno
	glRotated(90, 0, 1, 0);
	glPushMatrix();
	{
		glRotated(45, 1, 0, 0);
		DrawLeg();
	}
	glPopMatrix();
	glRotated(30, 0, 1, 0);
	glPushMatrix();
	{
		glRotated(45, 1, 0, 0);
		DrawLeg();
	}
	glPopMatrix();
	glRotated(30, 0, 1, 0);
	glPushMatrix();
	{
		glRotated(45, 1, 0, 0);
		DrawLeg();
	}
	glPopMatrix();
	glRotated(30, 0, 1, 0);
	glPushMatrix();
	{
		glRotated(45, 1, 0, 0);
		DrawLeg();
	}
	glPopMatrix();
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
	glDeleteTextures(1, &spider);
	glDeleteTextures(6, envCube);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::UpdateCamera()
{
	if (dist < 8)
		dist = 8;
	if (dist > 50)
		dist = 50;
	glTranslated(0, 0, -dist);
	glRotated(angle1, 1, 0, 0);
	glRotated(angle2, 0, 1, 0);

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

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());


	return texID;
}
