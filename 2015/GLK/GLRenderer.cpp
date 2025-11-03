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
	m_bLight = true;
	m_alpha = 0;
	m_beta = 0;
	m_dist = 0;
	m_moonRot = 0;
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
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	T[0] = LoadTexture("res/TSC0.jpg");
	T[1] = LoadTexture("res/TSC1.jpg");
	T[2] = LoadTexture("res/TSC2.jpg");
	T[3] = LoadTexture("res/TSC3.jpg");
	T[4] = LoadTexture("res/TSC4.jpg");
	T[5] = LoadTexture("res/TSC5.jpg");

	S[0] = LoadTexture("res/S0.jpg");
	S[1] = LoadTexture("res/S1.jpg");
	S[2] = LoadTexture("res/S2.jpg");
	S[3] = LoadTexture("res/S3.jpg");
	S[4] = LoadTexture("res/S4.jpg");
	S[5] = LoadTexture("res/S5.jpg");

	M[0] = LoadTexture("res/M0.jpg");
	M[1] = LoadTexture("res/M1.jpg");
	M[2] = LoadTexture("res/M2.jpg");
	M[3] = LoadTexture("res/M3.jpg");
	M[4] = LoadTexture("res/M4.jpg");
	M[5] = LoadTexture("res/M5.jpg");
	
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
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glRotated(m_beta, 1.0, 0.0, 0.0);
	glRotated(m_alpha, 0.0, 1.0, 0.0);
	DrawSpace(1.0, 20);
	glPopMatrix();

	if (m_bLight)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	float ligth_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	float ligth_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float ligth_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ligth_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ligth_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ligth_specular);

	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };

	glEnable(GL_DEPTH_TEST);

	glTranslated(0, 0, -m_dist);
	glRotated(m_beta, 1.0, 0.0, 0.0);
	glRotated(m_alpha, 0.0, 1.0, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	DrawEarth(3.0, 20);

	glTranslated(-50.0, 0.0, 0.0);
	glRotated(m_moonRot, 0.0, 1.0, 0.0);
	DrawMoon(1.0, 20);

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawPatch(double R, int n)
{
	double delta = 2. / (double)n;
	for (double y = 1.0; y > -1.0; y -= delta)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (double x = -1.0; x <= 1.0; x += delta)
		{
			double phi, theta;
			phi = atan(x);
			theta = atan(y * cos(phi));

			double x1, y1, z1;
			x1 = R * cos(theta) * sin(phi);
			y1 = R * sin(theta);
			z1 = R * cos(theta) * cos(phi);

			glNormal3f(x1 / R, y1 / R, z1 / R);
			glTexCoord2f((x + 1.0) / 2.0, (-y + 1.0) / 2.0);
			glVertex3f(x1, y1, z1);

			phi = atan(x);
			theta = atan((y - delta) * cos(phi));

			double x2, y2, z2;
			x2 = R * cos(theta) * sin(phi);
			y2 = R * sin(theta);
			z2 = R * cos(theta) * cos(phi);

			glNormal3f(x2 / R, y2 / R, z2 / R);
			glTexCoord2f((x + 1.0) / 2.0, (-y + delta + 1.0) / 2.0);
			glVertex3f(x2, y2, z2);
		}
		glEnd();
	}
}

void CGLRenderer::DrawEarth(double R, int tes)
{
	glPushMatrix();
	for (int i = 0; i < 4; i++)
	{
		glBindTexture(GL_TEXTURE_2D, T[i]);
		DrawPatch(R, tes);
		glRotated(90, 0, 1, 0);
	}
	glPopMatrix();

	glPushMatrix();
	glRotated(-90, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, T[4]);
	DrawPatch(R, tes);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, T[5]);
	DrawPatch(R, tes);
	glPopMatrix();
}

void CGLRenderer::DrawMoon(double R, int tes)
{
	glPushMatrix();
	for (int i = 0; i < 4; i++)
	{
		glBindTexture(GL_TEXTURE_2D, M[i]);
		DrawPatch(R, tes);
		glRotated(90, 0, 1, 0);
	}
	glPopMatrix();

	glPushMatrix();
	glRotated(-90, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, M[4]);
	DrawPatch(R, tes);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, M[5]);
	DrawPatch(R, tes);
	glPopMatrix();
}

void CGLRenderer::DrawSpace(double R, int tes)
{
	glCullFace(GL_FRONT);
	glPushMatrix();
	for (int i = 0; i < 4; i++)
	{
		glBindTexture(GL_TEXTURE_2D, S[i]);
		DrawPatch(R, tes);
		glRotated(90, 0, 1, 0);
	}
	glPopMatrix();

	glPushMatrix();
	glRotated(-90, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, S[4]);
	DrawPatch(R, tes);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, S[5]);
	DrawPatch(R, tes);
	glPopMatrix();
	glCullFace(GL_BACK);
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
	
	glDeleteTextures(6, T);
	glDeleteTextures(6, S);
	glDeleteTextures(6, M);

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

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}
