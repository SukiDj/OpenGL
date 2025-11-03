#include "pch.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES
#include <math.h>
#include "GLMaterial.h"

CGLRenderer::CGLRenderer()
{
	// camera info
	firstMouse = true;
	kameraX = 5.4;
	kameraY = 10.5;
	kameraZ = 0.0;
	kameraYaw = 0.0f;
	cameraDistanceFromCoordinateOrigin = sqrt(pow(kameraX, 2) + pow(kameraY, 2));
	kameraPitch = asin(kameraY / cameraDistanceFromCoordinateOrigin) * 180 / M_PI;
	kameraY *= 1.4;
	kameraX *= 0.9;

	// textures
	
	terrain = new GLTexture();

	// materials
	
	
}

CGLRenderer::~CGLRenderer()
{
	// textures
	
	
	terrain->Release();
	delete terrain;

	// materials
	
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(204 / 255.0, 204 / 255.0, 204 / 255.0, 1.0);

	// textures
	terrain->LoadTexture(CString("res/index.jpg"));

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::StopMovingCamera()
{
	firstMouse = true;
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(kameraX, kameraY, kameraZ,
		0.0, 6.0, 0.0,
		0.0, 1.0, 0.0);

	
	CrtajKoordinate();
	CrtajPodlogu();

	glPushMatrix();

	glPopMatrix();



	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	double aspect = (double)w / (double)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::MoveCamera(CPoint cursorPoint) // https://learnopengl.com/Getting-started/Camera
{
	if (firstMouse)
	{
		lastPoint.x = cursorPoint.x;
		lastPoint.y = cursorPoint.y;
		firstMouse = false;
	}

	float xoffset = lastPoint.x - cursorPoint.x; // moving left moves camera to the right
	float yoffset = cursorPoint.y - lastPoint.y; // moving up moves camera down (mouse y grows going down)
	lastPoint.x = cursorPoint.x;
	lastPoint.y = cursorPoint.y;

	float sensitivity = 0.5f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	kameraYaw += xoffset;
	kameraPitch += yoffset;

	if (kameraPitch > 89.0f)
		kameraPitch = 89.0f;
	if (kameraPitch < -89.0f)
		kameraPitch = -89.0f;

	double xPos = cos(kameraYaw * M_PI / 180.0) * cos(kameraPitch * M_PI / 180.0);
	double yPos = sin(kameraPitch * M_PI / 180.0);
	double zPos = -sin(kameraYaw * M_PI / 180.0) * cos(kameraPitch * M_PI / 180.0);
	kameraX = 2.1 * cameraDistanceFromCoordinateOrigin * xPos; 
	
	kameraY = 1.4 * cameraDistanceFromCoordinateOrigin * yPos; 
	kameraZ = 2.1 * cameraDistanceFromCoordinateOrigin * zPos;

}



void CGLRenderer::CrtajPodlogu()
{
	terrain->PrepareTexture(false);
	terrain->Select();

	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	{
		glNormal3f(0.0f, 1.0f, 0.0f);

		glTexCoord2f(0.0f, 4.0f);
		glVertex3f(-20.0f, -1.5f, 20.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(20.0f, -1.5f, 20.0f);

		glTexCoord2f(2.0f, 0.0f);
		glVertex3f(20.0f, -1.5f, -20.0f);

		glTexCoord2f(2.0f, 4.0f);
		glVertex3f(-20.0f, -1.5f, -20.0f);

	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}


void CGLRenderer::CrtajKoordinate()
{
	glLineWidth(2.0);
	bool lightingEnabled = glIsEnabled(GL_LIGHTING);
	if (lightingEnabled);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 10.0);
	}
	glEnd();
	if (lightingEnabled)
		glEnable(GL_LIGHTING);
}






