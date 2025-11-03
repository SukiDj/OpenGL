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
	cabineRot = 0;
	arm1Rot = 0;
	arm2Rot = 0;
	forkRot = 0;
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

	excavator = LoadTexture("res/excavator.png");
	bckgr[0] = LoadTexture("res/front.jpg");
	bckgr[1] = LoadTexture("res/left.jpg");
	bckgr[2] = LoadTexture("res/right.jpg");
	bckgr[3] = LoadTexture("res/back.jpg");
	bckgr[4] = LoadTexture("res/top.jpg");
	bckgr[5] = LoadTexture("res/bot.jpg");

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
		glTranslated(2.5, 40, 2.5);
		DrawEnvCube(100.0);
	}
	glPopMatrix();

	glTranslated(0, -10, 0);
	DrawAxis();
	DrawExcavator();

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

void CGLRenderer::DrawPolygon(POINTF* points, POINTF* texCoords, int n)
{
	glBindTexture(GL_TEXTURE_2D, excavator);
	glBegin(GL_TRIANGLE_FAN);
	{
		for (int i = 0; i < n; i++)
		{
			glTexCoord2f(texCoords[i].x, texCoords[i].y);
			glVertex3f(points[i].x, points[i].y, 0);
		}
	}
	glEnd();
}

void CGLRenderer::DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b)
{
	glNormal3f(0, 0, -1);
	DrawPolygon(points, texCoords, n);
	//omotac
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUAD_STRIP);
	{
		glColor3f(r, g, b);
		for (int i = 0; i < n; i++)
		{
			glVertex3f(points[i].x, points[i].y, zh);
			glVertex3f(points[i].x, points[i].y, 0);
		}
		glVertex3f(points[0].x, points[0].y, zh);
		glVertex3f(points[0].x, points[0].y, 0);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glTranslated(0, 0, zh);
	glNormal3f(0, 0, 1);
	DrawPolygon(points, texCoords, n);
}

void CGLRenderer::DrawBase()
{
	float height = 5;
	POINTF* points1 = new POINTF[8];

	points1[0].x = 0.5;
	points1[0].y = 0;
	points1[1].x = 7.5;
	points1[1].y = 0;
	points1[2].x = 8;
	points1[2].y = 0.5;
	points1[3].x = 8;
	points1[3].y = 1.5;
	points1[4].x = 7.5;
	points1[4].y = 2;
	points1[5].x = 0.5;
	points1[5].y = 2;
	points1[6].x = 0;
	points1[6].y = 1.5;
	points1[7].x = 0;
	points1[7].y = 0.5;

	float seg = 1.0 / 16.0;

	POINTF* texP1 = new POINTF[8];

	texP1[0].x = 1 * seg;
	texP1[0].y = 16 * seg;
	texP1[1].x = 15 * seg;
	texP1[1].y = 16 * seg;
	texP1[2].x = 16 * seg;
	texP1[2].y = 15 * seg;
	texP1[3].x = 16 * seg;
	texP1[3].y = 13 * seg;
	texP1[4].x = 15 * seg;
	texP1[4].y = 12 * seg;
	texP1[5].x = 1 * seg;
	texP1[5].y = 12 * seg;
	texP1[6].x = 0;
	texP1[6].y = 13 * seg;
	texP1[7].x = 0;
	texP1[7].y = 15 * seg;

	DrawExtrudedPolygon(points1, texP1, 8, height, 0.0, 0.0, 0.0);
}

void CGLRenderer::DrawBody()
{
	POINTF* points1 = new POINTF[5];

	points1[0].x = 0;
	points1[0].y = 0;
	points1[1].x = 4;
	points1[1].y = 0;
	points1[2].x = 4;
	points1[2].y = 2;
	points1[3].x = 2;
	points1[3].y = 4;
	points1[4].x = 0;
	points1[4].y = 4;

	float seg = 1.0 / 16.0;

	POINTF* texP1 = new POINTF[5];

	texP1[0].x = 8 * seg;
	texP1[0].y = 8 * seg;
	texP1[1].x = 1;
	texP1[1].y = 8 * seg;
	texP1[2].x = 1;
	texP1[2].y = 4 * seg;
	texP1[3].x = 12 * seg;
	texP1[3].y = 0;
	texP1[4].x = 8 * seg;
	texP1[4].y = 0;

	DrawExtrudedPolygon(points1, texP1, 5, 4, 0.96, 0.5, 0.12);
}

void CGLRenderer::DrawArm(double zh)
{
	POINTF* points1 = new POINTF[8];

	points1[0].x = 0.5;
	points1[0].y = 0;
	points1[1].x = 2;
	points1[1].y = 0;
	points1[2].x = 8;
	points1[2].y = 0.5;
	points1[3].x = 8;
	points1[3].y = 1.5;
	points1[4].x = 2;
	points1[4].y = 2;
	points1[5].x = 0.5;
	points1[5].y = 2;
	points1[6].x = 0;
	points1[6].y = 1.5;
	points1[7].x = 0;
	points1[7].y = 0.5;

	float seg = 1.0 / 16.0;

	POINTF* texP1 = new POINTF[8];

	texP1[0].x = 1 * seg;
	texP1[0].y = 12 * seg;
	texP1[1].x = 4 * seg;
	texP1[1].y = 12 * seg;
	texP1[2].x = 16 * seg;
	texP1[2].y = 11 * seg;
	texP1[3].x = 16 * seg;
	texP1[3].y = 9 * seg;
	texP1[4].x = 4 * seg;
	texP1[4].y = 8 * seg;
	texP1[5].x = 1 * seg;
	texP1[5].y = 8 * seg;
	texP1[6].x = 0;
	texP1[6].y = 9 * seg;
	texP1[7].x = 0;
	texP1[7].y = 11 * seg;

	DrawExtrudedPolygon(points1, texP1, 8, zh, 0.96, 0.5, 0.12);
}

void CGLRenderer::DrawFork()
{
	float height = 1.0;
	POINTF* points1 = new POINTF[6];

	points1[0].x = 0;
	points1[0].y = 0;
	points1[1].x = 4;
	points1[1].y = 0;
	points1[2].x = 4;
	points1[2].y = 2.5;
	points1[3].x = 3.5;
	points1[3].y = 3;
	points1[4].x = 0.5;
	points1[4].y = 3;
	points1[5].x = 0;
	points1[5].y = 2.5;

	float seg = 1.0 / 16.0;

	POINTF* texP1 = new POINTF[6];

	texP1[0].x = 0 * seg;
	texP1[0].y = 6 * seg;
	texP1[1].x = 8 * seg;
	texP1[1].y = 6 * seg;
	texP1[2].x = 8 * seg;
	texP1[2].y = 1 * seg;
	texP1[3].x = 7 * seg;
	texP1[3].y = 0 * seg;
	texP1[4].x = 1 * seg;
	texP1[4].y = 0 * seg;
	texP1[5].x = 0 * seg;
	texP1[5].y = 1 * seg;

	DrawExtrudedPolygon(points1, texP1, 6, height, 0.7, 0.7, 0.7);
}

void CGLRenderer::DrawExcavator()
{
	DrawBase();
	
	glTranslated(2, 2, -4.5);
	glTranslated(2, 0, 2);
	glRotated(cabineRot, 0, 1, 0);
	glTranslated(-2, 0, -2);
	DrawBody();
	
	glPushMatrix();
	{
		glTranslated(1, 1, -5);
		glRotated(90 + arm1Rot, 0, 0, 1);
		glTranslated(-1, -1, 0);

		DrawArm(1);
		glTranslated(7, 1, 0);
		glRotated(-90 + arm2Rot, 0, 0, 1);
		glTranslated(-1, -1, 0);
		DrawArm(1.5);
		glTranslated(7.5, 1, -1.25);
		glRotated(-90 + forkRot, 0, 0, 1);
		glTranslated(-1, -1, 0);
		DrawFork();
	}
	glPopMatrix();
	
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
	glBindTexture(GL_TEXTURE_2D, bckgr[1]);
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
	glBindTexture(GL_TEXTURE_2D, bckgr[2]);
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
	glBindTexture(GL_TEXTURE_2D, bckgr[3]);
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
	glBindTexture(GL_TEXTURE_2D, bckgr[4]);
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
	glBindTexture(GL_TEXTURE_2D, bckgr[5]);
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
	gluPerspective(55, (double)w / (double)h, 0.1, 1000);
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
