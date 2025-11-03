#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
//#pragma comment(lib, "GL\\glut32.lib")

#define toRad(x) x * 3.14159 / 180

CGLRenderer::CGLRenderer(void)
{
	m_hrc = NULL;

	dist = 20;
	alphaRot = 45;
	betaRot = 45;
	lightON = false;

	rotation[0] = rotation[1] = rotation[2] = 0;

	eyex = eyey = eyez = 0;
	centerx = centery = centerz = 0;
	upx = 0, upy = 1, upz = 0;
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
	glClearColor(0.7, 0.7, 0.7, 1.0);
	glEnable(GL_DEPTH_TEST);
	
	openGL = LoadTexture("res/OpenGL.bmp");

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	SetWhiteLight();

	float position1[] = { 1.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, position1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

	CalculateView();


	float position[] = { 5.0, 20.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);

	DrawAxis();
	//DrawCube(10);
	DrawRubikCube(5, 3);

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawCube(float a, float x1, float y1, float z1, float count)
{
	float segment = 1.0 / count;
	float x = x1 * segment;
	float y = (count - y1 - 1) * segment;
	float z = (count - z1 - 1) * segment;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, openGL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	float b = a / 2.0;
	//front
	glBegin(GL_QUADS);
	{
		glNormal3f(1, 0, 0);
		SetMaterial(1.0, 0.0, 0.0);
		glTexCoord2f(z, y + segment);
		glVertex3f(b, -b, b);
		glTexCoord2f(z + segment, y + segment);
		glVertex3f(b, -b, -b);
		glTexCoord2f(z + segment, y);
		glVertex3f(b, b, -b);
		glTexCoord2f(z, y);
		glVertex3f(b, b, b);
	}
	glEnd();
	//back
	glBegin(GL_QUADS);
	{
		glNormal3f(-1, 0, 0);
		SetMaterial(1.0, 0.6, 0.3);
		glTexCoord2f(z, y);
		glVertex3f(-b, b, b);
		glTexCoord2f(z + segment, y);
		glVertex3f(-b, b, -b);
		glTexCoord2f(z + segment, y + segment);
		glVertex3f(-b, -b, -b);
		glTexCoord2f(z, y + segment);
		glVertex3f(-b, -b, b);
	}
	glEnd();
	//left
	glBegin(GL_QUADS);
	{
		glNormal3f(0, 0, 1);
		SetMaterial(0.0, 1.0, 0.0);
		glTexCoord2f(x, y + segment);
		glVertex3f(-b, -b, b);
		glTexCoord2f(x + segment, y + segment);
		glVertex3f(b, -b, b);
		glTexCoord2f(x + segment, y);
		glVertex3f(b, b, b);
		glTexCoord2f(x, y);
		glVertex3f(-b, b, b);
	}
	glEnd();
	//right
	glBegin(GL_QUADS);
	{
		glNormal3f(0, 0, -1);
		SetMaterial(0.0, 0.0, 1.0);
		glTexCoord2f(x, y);
		glVertex3f(-b, b, -b);
		glTexCoord2f(x + segment, y);
		glVertex3f(b, b, -b);
		glTexCoord2f(x + segment, y + segment);
		glVertex3f(b, -b, -b);
		glTexCoord2f(x, y + segment);
		glVertex3f(-b, -b, -b);
	}
	glEnd();
	//top
	glBegin(GL_QUADS);
	{
		glNormal3f(0, 1, 0);
		SetMaterial(1.0, 1.0, 1.0);
		glTexCoord2f(x, z + segment);
		glVertex3f(b, b, b);
		glTexCoord2f(x + segment, z + segment);
		glVertex3f(b, b, -b);
		glTexCoord2f(x + segment, z);
		glVertex3f(-b, b, -b);
		glTexCoord2f(x, z);
		glVertex3f(-b, b, b);
	}
	glEnd();
	//bot
	glBegin(GL_QUADS);
	{
		glNormal3f(0, -1, 0);
		SetMaterial(1.0, 1.0, 0.0);

		glTexCoord2f(x, z);
		glVertex3f(-b, -b, b);
		glTexCoord2f(x + segment, z);
		glVertex3f(-b, -b, -b);
		glTexCoord2f(x + segment, z + segment);
		glVertex3f(b, -b, -b);
		glTexCoord2f(x, z + segment);
		glVertex3f(b, -b, b);
	}
	glEnd();
	glDisable(GL_CULL_FACE);
}

void CGLRenderer::DrawRubikCube(double a, int count)
{
	glPushMatrix();
	{
		int x = 0;
		int y = 0;
		glTranslated(-a, -a, -a);
		for (int x = 0; x < count; x++)
		{
			glPushMatrix();
			{
				glTranslated(0, a, a);
				glRotated(rotation[x], 1, 0, 0);
				glTranslated(0, -a, -a);
				for (int y = 0; y < count; y++)
				{
					for (int z = 0; z < count; z++)
					{
						DrawCube(a, x, y, z, count);
						glTranslated(0, 0, a + 0.05 * a);
					}
					glTranslated(0, a + 0.05 * a, -count * (a + 0.05 * a));
				}
			}
			glPopMatrix();
			glTranslated(a + 0.05 * a, 0, 0);
		}
	}
	glPopMatrix();
}

void CGLRenderer::DrawAxis()
{
	glDisable(GL_LIGHTING);
	glLineWidth(2);
	glBegin(GL_LINES);
	{
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(100, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 100);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 100, 0);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::SetWhiteLight()
{
	float glob_amb[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glob_amb);

	float ambient[] = { 0.6, 0.6, 0.6, 1.0 };
	float diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 13);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

void CGLRenderer::SetMaterial(float r, float g, float b)
{
	float amb[] = { 0.2 * r, 0.2 * g, 0.2 * b };
	float dif[] = { r, g, b };
	float spec[] = { r, g, b };
	float shin = 64.0;

	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT, GL_SHININESS, shin);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / h, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	
	glDeleteTextures(1, &openGL);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::CalculateView()
{
	eyex = dist * cos(toRad(alphaRot)) * cos(toRad(betaRot));
	eyey = dist * sin(toRad(alphaRot));
	eyez = dist * cos(toRad(alphaRot)) * sin(toRad(betaRot));

	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}
