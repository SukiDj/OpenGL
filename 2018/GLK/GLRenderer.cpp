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
	eyex = 10;
	eyey = 10;
	eyez = 0;
	centerx = 0;
	centery = 0;
	centerz = 0;
	upx = 0;
	upy = 1;
	upz = 0;
	alphaRot = 0;
	betaRot = 0;
	light = false;
	dist = 0;
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
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

	shipTex = LoadTexture("res/ShipT1.png");
	spaceTex[0] = LoadTexture("res/front.jpg");
	spaceTex[1] = LoadTexture("res/left.jpg");
	spaceTex[2] = LoadTexture("res/right.jpg");
	spaceTex[3] = LoadTexture("res/back.jpg");
	spaceTex[4] = LoadTexture("res/top.jpg");
	spaceTex[5] = LoadTexture("res/bot.jpg");

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
	glMatrixMode(GL_MODELVIEW);


	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	{
		glRotated(betaRot, 1.0, 0.0, 0.0);
		glRotated(alphaRot, 0.0, 1.0, 0.0);
		DrawSpaceCube(1.0);
	}
	glPopMatrix();



	float ligth_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	float ligth_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float ligth_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ligth_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ligth_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ligth_specular);

	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };

	
	glEnable(GL_DEPTH_TEST);
	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);

	glTranslatef(dist, dist, 0);
	glRotated(betaRot, 1.0, 0.0, 0.0);
	glRotated(alphaRot, 0.0, 1.0, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	if (light)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	
	DrawAxis();

	double d = sqrt(5.8 * 5.8 + 2.15 * 2.15);
	glTranslatef(-d / 2., 0, 0);
	glRotatef(90, 1, 0, 0);
	DrawShip();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawAxis()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glLineWidth(1);
	glPointSize(10);

	glBegin(GL_LINES);
	{
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(10, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 10, 0);
		
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 10);
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawTriangle(float d1, float d2, float rep)
{
	double alpha = atan2(d2, d1);
	double d3 = sqrt(d1 * d1 + d2 * d2);
	double y = d1 * cos(alpha) / d3;
	double x = d1 * sin(alpha) / d3;

	glBegin(GL_TRIANGLES);
	{
		//glColor3f(0.5, 0.5, 0.5);

		glNormal3f(0, 0, 1.0);

		glTexCoord2f(0.5 * rep, 0.0);
		glVertex3f(0, 0, 0);
		glTexCoord2f((0.5 + x) * rep, y * rep);
		glVertex3f(d1, 0, 0);
		glTexCoord2f(0.5 * rep, 1.0 * rep);
		glVertex3f(d1, d2, 0);

	}
	glEnd();
}

void CGLRenderer::DrawShip()
{
	//glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, shipTex);
	float alpha = atan2(2.15, 5.8);
	float alphaDeg = alpha * 180 / 3.14159;
	float rep = 1.0;
	// gore levo
	glPushMatrix();
	glRotated(-4.75, 0, 1, 0);
	glRotated(13, 1, 0, 0);
	glRotated(-alphaDeg, 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();
	// gore desno
	glPushMatrix();
	glScaled(1, -1, 1);
	glRotated(-4.75, 0, 1, 0);
	glRotated(13, 1, 0, 0);
	glRotated(-alphaDeg, 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();

	// Donji deo
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	// Levo
	glPushMatrix();
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-alphaDeg, 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();
	// Desno
	glPushMatrix();
	glScalef(1, -1, 1);
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-alphaDeg, 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();
	glPopMatrix();
	//glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawSpaceCube(double a)
{
	//glCullFace(GL_FRONT);
	glDisable(GL_LIGHTING);
	//left
	glBindTexture(GL_TEXTURE_2D, spaceTex[0]);
	glBegin(GL_QUADS);
	{
		//glNormal3f(0, 0, -1);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-a / 2, a / 2, a / 2);

		glTexCoord2f(0.0, 0.0);
		glVertex3f(-a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0, 0.0);
		glVertex3f(a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0, 1.0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();
	//back
	glBindTexture(GL_TEXTURE_2D, spaceTex[1]);
	glBegin(GL_QUADS);
	{
		//glNormal3f(1, 0, 0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-a / 2, a / 2, a / 2);

		glTexCoord2f(0.0, 0.0);
		glVertex3f(-a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0, 0.0);
		glVertex3f(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(1.0, 1.0);
		glVertex3f(-a / 2, a / 2, -a / 2);
	}
	glEnd();
	//front
	glBindTexture(GL_TEXTURE_2D, spaceTex[2]);
	glBegin(GL_QUADS);
	{
		//glNormal3f(-1, 0, 0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(a / 2, a / 2, a / 2);

		glTexCoord2f(0.0, 0.0);
		glVertex3f(a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0, 0.0);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(1.0, 1.0);
		glVertex3f(a / 2, a / 2, -a / 2);
	}
	glEnd();
	//right
	glBindTexture(GL_TEXTURE_2D, spaceTex[3]);
	glBegin(GL_QUADS);
	{
		//glNormal3f(0, 0, 1);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(0.0, 0.0);
		glVertex3f(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(1.0, 0.0);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(1.0, 1.0);
		glVertex3f(a / 2, a / 2, -a / 2);
	}
	glEnd();
	//top
	glBindTexture(GL_TEXTURE_2D, spaceTex[4]);
	glBegin(GL_QUADS);
	{
		//glNormal3f(0, -1, 0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(0.0, 0.0);
		glVertex3f(-a / 2, a / 2, a / 2);

		glTexCoord2f(1.0, 0.0);
		glVertex3f(a / 2, a / 2, a / 2);

		glTexCoord2f(1.0, 1.0);
		glVertex3f(a / 2, a / 2, -a / 2);
	}
	glEnd();
	//bottom
	glBindTexture(GL_TEXTURE_2D, spaceTex[5]);
	glBegin(GL_QUADS);
	{
		//glNormal3f(0, 1, 0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(0.0, 0.0);
		glVertex3f(-a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0, 0.0);
		glVertex3f(a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0, 1.0);
		glVertex3f(a / 2, -a / 2, -a / 2);
	}
	glEnd();
	//glCullFace(GL_BACK);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc); 
	glDeleteTextures(1, &shipTex);
	glDeleteTextures(6, spaceTex);
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
