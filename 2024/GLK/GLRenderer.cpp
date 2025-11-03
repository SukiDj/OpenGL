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
	flowerRot = 0;

	lightON = true;
	eyex = eyey = eyez = 0;
	centerx = centery = centerz = 0;
	upx = 0;
	upy = 1;
	upz = 0;

	m_vAmbient[0] = 0.2; m_vAmbient[1] = 0.2;
	m_vAmbient[2] = 0.2; m_vAmbient[3] = 1.0;
	m_vDiffuse[0] = 0.2; m_vDiffuse[1] = 0.8;
	m_vDiffuse[2] = 0.2; m_vDiffuse[3] = 1.0;

	m_vSpecular[0] = 1.0; m_vSpecular[1] = 1.0;
	m_vSpecular[2] = 1.0; m_vSpecular[3] = 1.0;
	m_vEmission[0] = 0.0; m_vEmission[1] = 0.0;
	m_vEmission[2] = 0.0; m_vEmission[3] = 1.0;

	m_fShininess = 64.0;
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
	
	env = LoadTexture("res/Env.jpg");

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

	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glDisable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);


	TurnOnLight();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

	float light_position1[] = { 0.0, 0.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	CalculateView();

	DrawAxis();
	glPushMatrix();
	glRotated(90, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	DrawSphere(100, 20, 10);
	glPopMatrix();


	glEnable(GL_LIGHT0);
	float light_position[] = { 0.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	DrawFlower();
	glTranslated(-9, 0, 9);
	DrawFlower();
	glTranslated(0, 0, -18);
	DrawFlower();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawSphere(float R, int n, int m)
{
	float alpha = -3.14159 / 2;
	float alphaStep = 3.14159 / m;
	float beta = 0;
	float betaStep = 2 * 3.14159 / n;

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	
	glBindTexture(GL_TEXTURE_2D, env);
	for (int i = 0; i <= n; i++)
	{
		glBegin(GL_QUAD_STRIP);
		{
			alpha = -3.14159 / 2;
			for (int j = 0; j <= m; j++)
			{
				double x1, y1, z1;
				PolarToCartesian(R, alpha, beta, x1, y1, z1);
				glTexCoord2f(0.5 + x1 / R * 0.5, 0.5 + z1 / R * 0.5);
				glVertex3f(x1, y1, z1);

				double x2, y2, z2;
				PolarToCartesian(R, alpha, beta + betaStep, x2, y2, z2);
				glTexCoord2f(0.5 + x2 / R * 0.5, 0.5 + z2 / R * 0.5);
				glVertex3f(x2, y2, z2);

				alpha += alphaStep;
			}
		}
		glEnd();
		beta += betaStep;
	}
	glDisable(GL_CULL_FACE);
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

void CGLRenderer::CalcRotAxis(double x1, double y1, double z1, double x2, double y2, double z2, double& x3, double& y3, double& z3)
{
	double vx = x2 - x1;
	double vy = y2 - y1;
	double vz = z2 - z1;

	double len = sqrt(vx * vx + vy * vy + vz * vz);

	if (len == 0)
	{
		x3 = y3 = z3 = 0;
		return;
	}

	x3 = vx / len;
	y3 = vy / len;
	z3 = vz / len;
}

void CGLRenderer::DrawSphFlower(float R, int n, int m, float factor, unsigned char R1, unsigned char G1, unsigned char B1, unsigned char R2, unsigned char G2, unsigned char B2)
{
	glDisable(GL_TEXTURE_2D);
	float alpha = -3.14159 / 2;
	float alphaStep = 3.14159 / m;
	float beta = 0;
	float betaStep = 2 * 3.14159 / n;
	double x1 = 0, y1 = 0, z1 = 0;
	double x2 = 0, y2 = 0, z2 = 0;
	double x3 = 0, y3 = 0, z3 = 0;
	double x4 = 0, y4 = 0, z4 = 0;
	double x5 = 0, y5 = 0, z5 = 0;

	// SPOLJNA STRANA SFERE
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	for (int i = 0; i < n; i++)  // koristimo < n da ne prelazimo preko indeksa
	{
		glPushMatrix();

		alpha = -3.14159 / 2;
		for (int j = 0; j < m; j++)
		{
			glPushMatrix();
			FLowerMaterial(R1, G1, B1, R2, G2, B2, j, m);

			PolarToCartesian(R, alpha, beta, x1, y1, z1);
			PolarToCartesian(R, alpha, beta + betaStep, x2, y2, z2);
			PolarToCartesian(R, alpha + alphaStep, beta, x3, y3, z3);
			PolarToCartesian(R, alpha + alphaStep, beta + betaStep, x4, y4, z4);

			CalcRotAxis(x1, y1, z1, x2, y2, z2, x5, y5, z5);

			/*float dx = R * sin(beta) * cos(alpha);
			float dy = R * cos(beta);
			float dz = R * cos(beta) * cos(alpha);*/
			//glTranslated(-dx, -dy - R, -dz);
			glRotated(flowerRot * factor, x5, y5, z5);
			//glTranslated(dx, dy + R, dz);

			glBegin(GL_QUAD_STRIP);
			{
				glNormal3f(x1 / R, y1 / R, z1 / R);
				glVertex3f(x1, y1 + R, z1);
				glNormal3f(x2 / R, y2 / R, z2 / R);
				glVertex3f(x2, y2 + R, z2);
				glNormal3f(x3 / R, y3 / R, z3 / R);
				glVertex3f(x3, y3 + R, z3);
				glNormal3f(x4 / R, y4 / R, z4 / R);
				glVertex3f(x4, y4 + R, z4);
			}
			glEnd();

			glPopMatrix();

			alpha += alphaStep;
		}

		glPopMatrix();
		beta += betaStep;
	}

	//---------------------------------------------------------------------------
	// UNUTRASNJA STRANA SFERE

	beta = 0;
	x1 = 0, y1 = 0, z1 = 0;
	x2 = 0, y2 = 0, z2 = 0;
	x3 = 0, y3 = 0, z3 = 0;
	x4 = 0, y4 = 0, z4 = 0;
	x5 = 0, y5 = 0, z5 = 0;

	//glCullFace(GL_BACK);
	for (int i = 0; i < n; i++)  // koristimo < n da ne prelazimo preko indeksa
	{
		glPushMatrix();

		alpha = 3.14159 / 2;
		for (int j = 0; j < m; j++)
		{
			glPushMatrix();
			FLowerMaterial(R1, G1, B1, R2, G2, B2, j, m);

			PolarToCartesian(R, alpha, beta, x1, y1, z1);
			PolarToCartesian(R, alpha, beta + betaStep, x2, y2, z2);
			PolarToCartesian(R, alpha - alphaStep, beta, x3, y3, z3);
			PolarToCartesian(R, alpha - alphaStep, beta + betaStep, x4, y4, z4);

			CalcRotAxis(x1, y1, z1, x2, y2, z2, x5, y5, z5);

			/*float dx = R * sin(beta) * cos(alpha);
			float dy = R * cos(beta);
			float dz = R * cos(beta) * cos(alpha);*/
			//glTranslated(-dx, -dy - R, -dz);
			glRotated(flowerRot * factor, x5, y5, z5);
			//glTranslated(dx, dy + R, dz);

			glBegin(GL_QUAD_STRIP);
			{
				glNormal3f(-x1 / R, -y1 / R, -z1 / R);
				glVertex3f(x1, y1 + R, z1);
				glNormal3f(-x2 / R, -y2 / R, -z2 / R);
				glVertex3f(x2, y2 + R, z2);
				glNormal3f(-x3 / R, -y3 / R, -z3 / R);
				glVertex3f(x3, y3 + R, z3);
				glNormal3f(-x4 / R, -y4 / R, -z4 / R);
				glVertex3f(x4, y4 + R, z4);
			}
			glEnd();

			glPopMatrix();

			alpha -= alphaStep;
		}

		glPopMatrix();
		beta += betaStep;
	}
	glDisable(GL_CULL_FACE);
}


void CGLRenderer::FLowerMaterial(unsigned char R1, unsigned char G1, unsigned char B1, unsigned char R2, unsigned char G2, unsigned char B2, int j, int m)
{
	float segR = R2 / 255.0 - R1 / 255.0;
	float segG = G2 / 255.0 - G1 / 255.0;//  192, 255 => 255-192 = 63
	float segB = B2 / 255.0 - B1 / 255.0;

	m_vAmbient[0] = 0.2 * R1 / 255.0;
	m_vAmbient[1] = 0.2 * G1 / 255.0;
	m_vAmbient[2] = 0.2 * B1 / 255.0;
	m_vAmbient[3] = 1.0;

	m_vDiffuse[0] = R1 / 255.0 + j * segR / m;
	m_vDiffuse[1] = G1 / 255.0 + j * segG / m;// 192 + j * 63 = 192 + 0 ; 192 + 1 * 63/m ... 192 + m * 63 / m = 192 + 63 = 255
	m_vDiffuse[2] = B1 / 255.0 + j * segB / m;
	m_vDiffuse[3] = 1.0;

	m_vSpecular[0] = 1.0;
	m_vSpecular[1] = 1.0;
	m_vSpecular[2] = 1.0;
	m_vSpecular[3] = 1.0;

	m_vEmission[0] = 0.0;
	m_vEmission[1] = 0.0;
	m_vEmission[2] = 0.0;
	m_vEmission[3] = 1.0;

	m_fShininess = 64.0;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_vAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_vDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_vSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_vEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_fShininess);
}

void CGLRenderer::DrawFlower()
{
	DrawSphFlower(2, 36, 18, 1, 0, 192, 0, 0, 255, 0);
	//glTranslated(0, 0.5, 0);
	DrawSphFlower(1.5, 36, 18, 0.5, 0, 0, 255, 255, 0, 0);
	//glTranslated(0, 0.5, 0);
	DrawSphFlower(1, 36, 18, 0.25, 192, 192, 0, 192, 192, 192);
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
	glDeleteTextures(1, &env);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::PolarToCartesian(double R, double phi, double theta, double& x, double& y, double& z)
{
	x = R * cos(phi) * cos(theta);
	y = R * sin(phi);
	z = R * cos(phi) * sin(theta);
}

void CGLRenderer::CalculateView()
{
	eyex = dist * cos(alphaRot * 3.14159 / 180) * cos(betaRot * 3.14159 / 180);
	eyey = dist * sin(alphaRot * 3.14159 / 180);
	eyez = dist * cos(alphaRot * 3.14159 / 180) * sin(betaRot * 3.14159 / 180);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());


	return texID;
}

void CGLRenderer::TurnOnLight()
{
	float lmodel_ambient[] = { 0.3, 0.3, 0.3 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	float ambient_light[] = { 0.5, 0.5, 0.5, 1.0 };
	float diffuse_light[] = { 1.0, 1.0, 1.0, 1.0 };
	float specular_light[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light);
}
