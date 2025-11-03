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
	glClearColor(1.0, 1.0, 1.0, 0.0);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	gluLookAt(cameraX, cameraY, cameraZ, 0.0, 2.5, 0.0, 0.0, 1.0, 0.0);


	// Grid
	DrawGrid(20);


	/*GLfloat light_position[] = { 8.0, 15.0, 6.5, 0.0 };
	GLfloat light_ambient[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat ambcolor[] = { 0.2, 0.2, 0.2 };

	glEnable(GL_LIGHTING);                                // enable lighting
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambcolor);     // ambient light

	glEnable(GL_LIGHT0);                                  // enable light source
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);    // config light source
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);*/

	
	DrawTruck();
	glTranslatef(0, -0.5, 0);
	DrawGround();
	

	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	double aspect = (double)w / (double)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 100);
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

void CGLRenderer::DrawCube(double dSize)
{
	double a = dSize;
	glBegin(GL_QUAD_STRIP);

		// Prednja leva vertikalna ivica
		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(-a / 2, a / 2, a / 2);
		glVertex3d(-a / 2, -a / 2, a / 2);

		// Prednja desna vertikalna ivica
		glColor3d(0.0, 1.0, 0.0);
		glVertex3d(a / 2, a / 2, a / 2);
		glVertex3d(a / 2, -a / 2, a / 2);

		// Zadnja desna vertikalna ivica
		glColor3d(0.0, 0.0, 1.0);
		glVertex3d(a / 2, a / 2, -a / 2);
		glVertex3d(a / 2, -a / 2, -a / 2);

		// Zadnja leva vertikalna ivica
		glColor3d(1.0, 1.0, 0.0);
		glVertex3d(-a / 2, a / 2, -a / 2);
		glVertex3d(-a / 2, -a / 2, -a / 2);

		// Prednja leva vertikalna ivica
		glColor3d(0.5, 0.5, 0.0);
		glVertex3d(-a / 2, a / 2, a / 2);
		glVertex3d(-a / 2, -a / 2, a / 2);

	glEnd();

	glBegin(GL_QUADS);

		// Gornja stranica
		glColor3d(0.0, 1.0, 1.0);
		glVertex3d(-a / 2, a / 2, a / 2);
		glVertex3d(a / 2, a / 2, a / 2);
		glVertex3d(a / 2, a / 2, -a / 2);
		glVertex3d(-a / 2, a / 2, -a / 2);

		// Donja stranica
		glVertex3d(-a / 2, -a / 2, -a / 2);
		glVertex3d(a / 2, -a / 2, -a / 2);
		glVertex3d(a / 2, -a / 2, a / 2);
		glVertex3d(-a / 2, -a / 2, a / 2);

	glEnd();
}

void CGLRenderer::DrawGrid(double meshsize)
{
	GLfloat mat_amb[] = { 1.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);
	GLfloat mat_dif[] = { 0.0, 0.0, 0.0, 0.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 0.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	GLfloat mat_shininess = 10.0;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);

	//X osa
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.01, 0.0);
	glVertex3f(meshsize + 5, 0.01, 0.0);
	glEnd();

	GLfloat mat_amb1[] = { 0.0, 1.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb1);

	//Y osa
	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, meshsize + 5, 0.0);
	glEnd();

	GLfloat mat_amb2[] = { 0.0, 0.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb2);

	//Z osa
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.01, 0.0);
	glVertex3f(0.0, 0.01, meshsize + 5);
	glEnd();
}

// Napomena: moze se koristiti i za crtanje kocke (n=4)
void CGLRenderer::DrawCilindar(double radius, double height, int n)
{
	glEnable(GL_TEXTURE_2D);
	UINT texID = LoadTexture("images/tekstura.png");
	glBindTexture(GL_TEXTURE_2D, texID);

	double x = 0.0;
	double z = 0.0;
	double nx = 0.0;
	double ny = 0.0;
	double nz = 0.0;
	double angle = 0.0;
	double angle_step = 2 * 3.1415926535898 / n;

	/* Omotac valjka */
	glBegin(GL_QUAD_STRIP);
		angle = 0.0;
		while (angle < 2 * 3.1415926535898) {
			x = radius * cos(angle);
			z = radius * sin(angle);
			nx = cos(angle);
			ny = 0.0;
			nx = sin(angle);
			glNormal3d(nx, ny, nz);
			glVertex3d(x, height, z);
			glNormal3d(nx, ny, nz);
			glVertex3d(x, 0, z);
			angle += angle_step;
		}
		glVertex3d(radius, height, 0);
		glVertex3d(radius, 0.0, 0.0);
	glEnd();

	/* Osnova valjka */
	//glColor3d(0, 0, 1);
	glBegin(GL_POLYGON);
		angle = 0.0;
		while (angle < 2 * 3.1415926535898) {
			x = radius * cos(angle);
			nx = 0.0;
			ny = 1.0;
			nz = 0.0;
			z = radius * sin(angle);
			glNormal3d(nx, ny, nz);
			glTexCoord2f((x + 9) * 0.0625, (z + 1.5) * 0.0625);
			glVertex3d(x, height, z);
			angle = angle + angle_step;
		}
		glVertex3d(radius, height, 0);
	glEnd();

	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * 3.1415926535898) {
		x = radius * cos(angle);
		nx = 0.0;
		ny = -1.0;
		nz = 0.0;
		z = radius * sin(angle);
		glNormal3d(nx, ny, nz);
		glTexCoord2f((x + 9) * 0.0625, (z + 1.5) * 0.0625);
		glVertex3d(x, 0, z);
		angle = angle + angle_step;
	}
	glVertex3d(radius, 0, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawKupa(double radius, double height, int n)
{
	double x = 0.0;
	double z = 0.0;
	double angle = 0.0;
	double angle_step = 2 * 3.1415926535898 / n;

	/* Osnova kupe */
	//glColor3d(0, 0, 1);
	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * 3.1415926535898) {
		x = radius * cos(angle);
		z = radius * sin(angle);
		glVertex3d(x, 0, z);
		angle = angle + angle_step;
	}
	glVertex3d(radius, 0, 0);
	glEnd();

	/* Omotac kupe */
	//glColor3d(1, 0, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, height, 0); // gornje teme
	angle = 0.0;
	while (angle < 2 * 3.1415926535898) {
		x = radius * cos(angle);
		z = radius * sin(angle);
		glVertex3d(x, 0, z);
		angle += angle_step;
	}
	glVertex3d(radius, 0, 0);
	glEnd();

}

void CGLRenderer::DrawZarubljenaKupa(double baseRadius, double topRadius, double height, int n)
{
	double xt = 0.0;
	double zt = 0.0;
	double xb = 0.0;
	double zb = 0.0;
	double angle = 0.0;
	double angle_step = 2 * 3.1415 / n;

	/* Omotac kupe */
	glBegin(GL_QUAD_STRIP);
	angle = 0.0;
	while (angle < 2 * 3.1415) {
		xt = topRadius * cos(angle);
		zt = topRadius * sin(angle);
		glVertex3f(xt, height / 2, zt);

		xb = baseRadius * cos(angle);
		zb = baseRadius * sin(angle);
		glVertex3f(xb, -height / 2, zb);
		angle += angle_step;
	}
	glVertex3f(topRadius, height / 2, 0.0);
	glVertex3f(baseRadius, -height / 2, 0.0);
	glEnd();

	/* Osnova kupe */
	//glColor3d(0, 0, 1);
	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * 3.1415) {
		xt = topRadius * cos(angle);
		zt = topRadius * sin(angle);
		glVertex3f(xt, height / 2, zt);
		angle = angle + angle_step;
	}
	glVertex3f(topRadius, height / 2, 0);
	glEnd();

	/* Osnova kupe */
	//glColor3d(0, 0, 1);
	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * 3.1415) {
		xb = baseRadius * cos(angle);
		zb = baseRadius * sin(angle);
		glVertex3f(xb, -height / 2, zb);
		angle = angle + angle_step;
	}
	glVertex3f(baseRadius, -height / 2, 0);
	glEnd();
}

void CGLRenderer::DrawSfera(double radius)
{
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
	double nx = 0.0;
	double ny = 0.0;
	double nz = 0.0;
	double alfa = 3.1415926535898 / 2.0;
	double beta = 0;
	double alfa_korak = 3.1415926535898 / 15;
	double beta_korak = 3.1415926535898 / 15;
	
	glEnable(GL_TEXTURE_2D);
	UINT texID = LoadTexture("images/tekstura.png");
	glBindTexture(GL_TEXTURE_2D, texID);

	glBegin(GL_QUAD_STRIP);
	while (alfa > -3.1415926535898 / 2)
	{
		while (beta < 360)
		{
			x = radius * cos(alfa) * cos(beta);
			y = radius * sin(alfa);
			z = radius * cos(alfa) * sin(beta);
			nx = x / radius;
			ny = y / radius;
			nz = z / radius;
			double tx1 = atan2(nx, nz) / (2. * 3.1415926535898) + 0.5;
			double ty1 = asin(ny) / 3.1415926535898 + .5;
			glNormal3d(nx, ny, nz);
			glTexCoord2f(tx1, (16 - ty1) * 0.5);
			glVertex3d(x, y, z);

			alfa -= alfa_korak;
			x = radius * cos(alfa) * cos(beta);
			y = radius * sin(alfa);
			z = radius * cos(alfa) * sin(beta);
			nx = x / radius;
			ny = y / radius;
			nz = z / radius;
			double tx = atan2(nx, nz) / (2. * 3.1415926535898) + 0.5;
			double ty = asin(ny) / 3.1415926535898 + .5;
			if (tx < 0.75 && tx1 > 0.75)
				tx += 1.0;
			else if (tx > 0.75 && tx1 < 0.75)
				tx -= 1.0;
			glNormal3d(nx, ny, nz);
			glTexCoord2f(tx, (16 - ty) * 0.5);
			glVertex3d(x, y, z);
			
			alfa += alfa_korak;
			beta += beta_korak;
		}
		alfa -= alfa_korak;
		beta = 0;
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawPoluSfera(double radius)
{
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
	double nx = 0.0;
	double ny = 0.0;
	double nz = 0.0;
	double alfa = 3.1415926535898 / 2.0;
	double beta = 0;
	double alfa_korak = 3.1415926535898 / 15;
	double beta_korak = 3.1415926535898 / 15;

	glBegin(GL_QUAD_STRIP);
	while (alfa > 0)
	{
		while (beta < 360)
		{
			x = radius * cos(alfa) * cos(beta);
			y = radius * sin(alfa);
			z = radius * cos(alfa) * sin(beta);
			nx = x / radius;
			ny = y / radius;
			nz = z / radius;
			glNormal3d(nx, ny, nz);
			glVertex3d(x, y, z);

			alfa -= alfa_korak;
			x = radius * cos(alfa) * cos(beta);
			y = radius * sin(alfa);
			z = radius * cos(alfa) * sin(beta);
			nx = x / radius;
			ny = y / radius;
			nz = z / radius;
			glNormal3d(nx, ny, nz);
			glVertex3d(x, y, z);

			alfa += alfa_korak;
			beta += beta_korak;
		}
		alfa -= alfa_korak;
		beta = 0;
	}
	glEnd();
}

void CGLRenderer::DrawVaza(int n)
{
	double xt = 0.0;
	double zt = 0.0;
	double xb = 0.0;
	double zb = 0.0;
	double angle = 0.0;
	double angle_step = 2 * 3.1415 / n;
	double baseRadius = 4.0;
	double topRadius;
	double height = 0;
	int nizUglova[14] = { -1, -1, 0, 0, 1, -1, 1, 1, 1, 1, -1, 1, -1, 1 };
	double nx = 0.0;
	double ny = 0.0;
	double nz = 0.0;

	//omotac
	glBegin(GL_QUAD_STRIP);
	angle = 0.0;
	for (int i = 0; i < 14; i++)
	{
		if (nizUglova[i] == -1)
			topRadius = baseRadius - 1.0;
		else if (nizUglova[i] == 1)
			topRadius = baseRadius + 1.0;
		else
			topRadius = baseRadius;

		if (i % 2 == 0)
		{
			//glColor3f(0.5, 0, 0);
			GLfloat mat_amb[] = { 0.12, 0.09, 0.53, 1.0 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);

			GLfloat mat_dif[] = { 0.309804, 0.309804, 0.466667, 1.0 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);

			GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

			GLfloat mat_shininess = 64.0;
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

			GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 1.0 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
		}
		else
		{
			//glColor3f(0, 0, 0.5);
			GLfloat mat_amb[] = { 0.09, 0.27, 0.53, 1.0 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);

			GLfloat mat_dif[] = { 0.337255, 0.337255, 0.717647, 0.8 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);

			GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

			GLfloat mat_shininess = 64.0;
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

			GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 1.0 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
		}

		while (angle < 2 * 3.1415) {
			if (nizUglova[i] == 0) //valjak
			{
				nx = cos(angle);
				ny = 0;
				nz = sin(angle);
			}
			else if (nizUglova[i] == -1) //kupa
			{
				nx = 1 / sqrt(2) * cos(angle);
				ny = 1 / sqrt(2);
				nz = 1 / sqrt(2) * sin(angle);
			}
			else //okrenuta kupa
			{
				nx = 1 / sqrt(2) * cos(angle);
				ny = -1 / sqrt(2);
				nz = 1 / sqrt(2) * sin(angle);
			}


			xt = topRadius * cos(angle);
			zt = topRadius * sin(angle);
			glNormal3f(nx, ny, nz);
			glVertex3f(xt, height + 1.0, zt);

			xb = baseRadius * cos(angle);
			zb = baseRadius * sin(angle);
			glNormal3f(nx, ny, nz);
			glVertex3f(xb, height, zb);
			angle += angle_step;
		}
		/*glNormal3f(nx, ny, nz);
		glVertex3f(topRadius, height + 1.0, 0.0);
		glNormal3f(nx, ny, nz);
		glVertex3f(baseRadius, height, 0.0);*/

		height += 1.0;
		angle = 0.0;
		baseRadius = topRadius;
	}
	glEnd();

	//iscrtavanje---normala------------
	GLfloat mat_amb[] = { 0.0, 1.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);
	GLfloat mat_dif[] = { 0.0, 0.0, 0.0, 0.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 0.5 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	GLfloat mat_shininess = 40.0;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);

	if (normaleFleg)
	{
		baseRadius = 4.0;
		height = 0.0;
		angle = 0.0;
		double alfa = 3.14159 / 4;
		glColor3f(0, 1.0, 0);
		for (int i = 0; i < 14; i++)
		{
			if (nizUglova[i] == -1)
				topRadius = baseRadius - 1.0;
			else if (nizUglova[i] == 1)
				topRadius = baseRadius + 1.0;
			else
				topRadius = baseRadius;

			while (angle < 2 * 3.1415) {

				xt = topRadius * cos(angle);
				zt = topRadius * sin(angle);

				nx = 1 / sqrt(2) * cos(angle);
				ny = 1 / sqrt(2) * nizUglova[i] * 0.707 * -1;
				nz = 1 / sqrt(2) * sin(angle);

				glBegin(GL_LINES);
				glVertex3f(xt + nx, height + ny + 1.0, nz + zt);
				glVertex3f(xt, height + 1.0, zt);
				glEnd();

				nx = 1 / sqrt(2) * cos(angle);
				nz = 1 / sqrt(2) * sin(angle);

				xb = baseRadius * cos(angle);
				zb = baseRadius * sin(angle);
				glBegin(GL_LINES);
				glVertex3f(nx + xb, height + ny, nz + zb);
				glVertex3f(xb, height, zb);
				glEnd();

				angle += angle_step;
			}

			/*glBegin(GL_LINES);
			glVertex3f(nx + xt, height+ny, nz + zt);
			glVertex3f(topRadius, height + 1.0, 0.0);
			glEnd();
			glBegin(GL_LINES);
			glVertex3f(nx + xb, height + ny, nz + zb);
			glVertex3f(xb, height, zb);
			glEnd();*/

			height += 1.0;
			angle = 0.0;
			baseRadius = topRadius;
		}
	}
}

void CGLRenderer::DrawTruck()
{
	float texSec = 0.0625;

	GLfloat mat_amb[] = { 0.09, 0.27, 0.53, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);

	GLfloat mat_dif[] = { 0.337255, 0.337255, 0.717647, 0.8 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);

	GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

	GLfloat mat_shininess = 64.0;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);

	float z = 3.0; //polusirina kamiona
	
	// Gornji, zadnji, donji i prednji deo kamiona
	glColor3f(0.7, 0.7, 0.7);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(-3.5, 3, z);
	glVertex3f(-3.5, 3, -z);
	glVertex3f(7.5, 3, z);
	glVertex3f(7.5, 3, -z);
	glVertex3f(7.5, 0, z);
	glVertex3f(7.5, 0, -z);
	glVertex3f(3.5, 0, z);
	glVertex3f(3.5, 0, -z);
	glVertex3f(3.5, 1, z);
	glVertex3f(3.5, 1, -z);
	glVertex3f(2.5, 2, z);
	glVertex3f(2.5, 2, -z);
	glVertex3f(1.5, 2, z);
	glVertex3f(1.5, 2, -z);
	glVertex3f(0.5, 1, z);
	glVertex3f(0.5, 1, -z);
	glVertex3f(0.5, 0, z);
	glVertex3f(0.5, 0, -z);
	glVertex3f(-3.5, 0, z);
	glVertex3f(-3.5, 0, -z);
	glVertex3f(-3.5, 1, z);
	glVertex3f(-3.5, 1, -z);
	glVertex3f(-4.5, 2, z);
	glVertex3f(-4.5, 2, -z);
	glVertex3f(-5.5, 2, z);
	glVertex3f(-5.5, 2, -z);
	glVertex3f(-6.5, 1, z);
	glVertex3f(-6.5, 1, -z);
	glVertex3f(-6.5, 0, z);
	glVertex3f(-6.5, 0, -z);
	glVertex3f(-7.5, 0, z);
	glVertex3f(-7.5, 0, -z);
	glVertex3f(-7.5, 4, z);
	glVertex3f(-7.5, 4, -z);
	glVertex3f(-5.5, 6, z);
	glVertex3f(-5.5, 6, -z);
	glVertex3f(-3.5, 6, z);
	glVertex3f(-3.5, 6, -z);
	glVertex3f(-3.5, 3, z);
	glVertex3f(-3.5, 3, -z);
	glEnd();

	GLfloat mat_amb1[] = { 0.09, 0.27, 0.53, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb1);

	GLfloat mat_dif1[] = { 0.1, 0, 0, 0.8 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif1);

	GLfloat mat_specular1[] = { 0.5, 0.5, 0.5, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular1);

	GLfloat mat_shininess1 = 64.0;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess1);

	GLfloat mat_emission1[] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission1);

	// Leva strana kamiona
	glEnable(GL_TEXTURE_2D);
	UINT texID = LoadTexture("images/tekstura.png");
	glBindTexture(GL_TEXTURE_2D, texID);

	glBegin(GL_POLYGON);
	glTexCoord2f(4 * texSec, 3 * texSec);
	glVertex3f(-3.5, 3, z);
	glTexCoord2f(15 * texSec, 3 * texSec);
	glVertex3f(7.5, 3, z);
	glTexCoord2f(15 * texSec, 4 * texSec);
	glVertex3f(7.5, 2, z);
	glTexCoord2f(0 * texSec, 4 * texSec);
	glVertex3f(-7.5, 2, z);
	glTexCoord2f(0 * texSec, 2 * texSec);
	glVertex3f(-7.5, 4, z);
	glTexCoord2f(2 * texSec, 0 * texSec);
	glVertex3f(-5.5, 6, z);
	glTexCoord2f(4 * texSec, 0 * texSec);
	glVertex3f(-3.5, 6, z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0 * texSec, 6 * texSec);
	glVertex3f(-7.5, 0, z);
	glTexCoord2f(0 * texSec, 4 * texSec);
	glVertex3f(-7.5, 2, z);
	glTexCoord2f(1 * texSec, 6 * texSec);
	glVertex3f(-6.5, 0, z);
	glTexCoord2f(1 * texSec, 4 * texSec);
	glVertex3f(-6.5, 2, z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(2 * texSec, 4 * texSec);
	glVertex3f(-5.5, 2, z);
	glTexCoord2f(1 * texSec, 5 * texSec);
	glVertex3f(-6.5, 1, z);
	glTexCoord2f(1 * texSec, 4 * texSec);
	glVertex3f(-6.5, 2, z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(3 * texSec, 4 * texSec);
	glVertex3f(-4.5, 2, z);
	glTexCoord2f(4 * texSec, 5 * texSec);
	glVertex3f(-3.5, 1, z);
	glTexCoord2f(4 * texSec, 4 * texSec);
	glVertex3f(-3.5, 2, z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(4 * texSec, 6 * texSec);
	glVertex3f(-3.5, 0, z);
	glTexCoord2f(4 * texSec, 4 * texSec);
	glVertex3f(-3.5, 2, z);
	glTexCoord2f(8 * texSec, 6 * texSec);
	glVertex3f(0.5, 0, z);
	glTexCoord2f(8 * texSec, 4 * texSec);
	glVertex3f(0.5, 2, z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(9 * texSec, 4 * texSec);
	glVertex3f(1.5, 2, z);
	glTexCoord2f(8 * texSec, 5 * texSec);
	glVertex3f(0.5, 1, z);
	glTexCoord2f(8 * texSec, 4 * texSec);
	glVertex3f(0.5, 2, z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(10 * texSec, 4 * texSec);
	glVertex3f(2.5, 2, z);
	glTexCoord2f(11 * texSec, 5 * texSec);
	glVertex3f(3.5, 1, z);
	glTexCoord2f(11 * texSec, 4 * texSec);
	glVertex3f(3.5, 2, z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(11 * texSec, 6 * texSec);
	glVertex3f(3.5, 0, z);
	glTexCoord2f(11 * texSec, 4 * texSec);
	glVertex3f(3.5, 2, z);
	glTexCoord2f(15 * texSec, 6 * texSec);
	glVertex3f(7.5, 0, z);
	glTexCoord2f(15 * texSec, 4 * texSec);
	glVertex3f(7.5, 2, z);
	glEnd();

	// Desna strana kamiona

	glBegin(GL_POLYGON);
	glTexCoord2f(4 * texSec, 3 * texSec);
	glVertex3f(-3.5, 3, -z);
	glTexCoord2f(15 * texSec, 3 * texSec);
	glVertex3f(7.5, 3, -z);
	glTexCoord2f(15 * texSec, 4 * texSec);
	glVertex3f(7.5, 2, -z);
	glTexCoord2f(0 * texSec, 4 * texSec);
	glVertex3f(-7.5, 2, -z);
	glTexCoord2f(0 * texSec, 2 * texSec);
	glVertex3f(-7.5, 4, -z);
	glTexCoord2f(2 * texSec, 0 * texSec);
	glVertex3f(-5.5, 6, -z);
	glTexCoord2f(4 * texSec, 0 * texSec);
	glVertex3f(-3.5, 6, -z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0 * texSec, 6 * texSec);
	glVertex3f(-7.5, 0, -z);
	glTexCoord2f(0 * texSec, 4 * texSec);
	glVertex3f(-7.5, 2, -z);
	glTexCoord2f(1 * texSec, 6 * texSec);
	glVertex3f(-6.5, 0, -z);
	glTexCoord2f(1 * texSec, 4 * texSec);
	glVertex3f(-6.5, 2, -z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(2 * texSec, 4 * texSec);
	glVertex3f(-5.5, 2, -z);
	glTexCoord2f(1 * texSec, 5 * texSec);
	glVertex3f(-6.5, 1, -z);
	glTexCoord2f(1 * texSec, 4 * texSec);
	glVertex3f(-6.5, 2, -z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(3 * texSec, 4 * texSec);
	glVertex3f(-4.5, 2, -z);
	glTexCoord2f(4 * texSec, 5 * texSec);
	glVertex3f(-3.5, 1, -z);
	glTexCoord2f(4 * texSec, 4 * texSec);
	glVertex3f(-3.5, 2, -z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(4 * texSec, 6 * texSec);
	glVertex3f(-3.5, 0, -z);
	glTexCoord2f(4 * texSec, 4 * texSec);
	glVertex3f(-3.5, 2, -z);
	glTexCoord2f(8 * texSec, 6 * texSec);
	glVertex3f(0.5, 0, -z);
	glTexCoord2f(8 * texSec, 4 * texSec);
	glVertex3f(0.5, 2, -z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(9 * texSec, 4 * texSec);
	glVertex3f(1.5, 2, -z);
	glTexCoord2f(8 * texSec, 5 * texSec);
	glVertex3f(0.5, 1, -z);
	glTexCoord2f(8 * texSec, 4 * texSec);
	glVertex3f(0.5, 2, -z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(10 * texSec, 4 * texSec);
	glVertex3f(2.5, 2, -z);
	glTexCoord2f(11 * texSec, 5 * texSec);
	glVertex3f(3.5, 1, -z);
	glTexCoord2f(11 * texSec, 4 * texSec);
	glVertex3f(3.5, 2, -z);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(11 * texSec, 6 * texSec);
	glVertex3f(3.5, 0, -z);
	glTexCoord2f(11 * texSec, 4 * texSec);
	glVertex3f(3.5, 2, -z);
	glTexCoord2f(15 * texSec, 6 * texSec);
	glVertex3f(7.5, 0, -z);
	glTexCoord2f(15 * texSec, 4 * texSec);
	glVertex3f(7.5, 2, -z);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// Tockovi

	glPushMatrix();
	glTranslatef(2, 0, 1.5);
	glRotatef(90, 1, 0, 0);
	DrawCilindar(1.5, 1, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, 0, -2.5);
	glRotatef(90, 1, 0, 0);
	DrawCilindar(1.5, 1, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, 0, 1.5);
	glRotatef(90, 1, 0, 0);
	DrawCilindar(1.5, 1, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, 0, -2.5);
	glRotatef(90, 1, 0, 0);
	DrawCilindar(1.5, 1, 20);
	glPopMatrix();

	// Cisterna

	glPushMatrix();
	glTranslatef(2, 4.5, 0);
	glScalef(1.85, 0.5, 1);
	glRotatef(85, 0, 1, 0);
	DrawSfera(3);
	glPopMatrix();
}

void CGLRenderer::DrawGround()
{
	glEnable(GL_TEXTURE_2D);
	UINT texID = LoadTexture("images/grass.jpg");
	glBindTexture(GL_TEXTURE_2D, texID);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-25, -1, 25);
	glTexCoord2f(4, 0);
	glVertex3f(25, -1, 25);
	glTexCoord2f(4, 6);
	glVertex3f(25, -1, -25);
	glTexCoord2f(0, 6);
	glVertex3f(-25, -1, -25);
	glEnd();
	glDisable(GL_TEXTURE_2D);
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
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_BGRA_EXT, img.Width(), img.Height(),
		GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());
	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.Width(), img.Height(),
		0,GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());*/
	return texID;
}

void CGLRenderer::ResetMaterials()
{
	GLfloat mat_amb[] = { 0.2, 0.2, 0.2, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);

	GLfloat mat_dif[] = { 0.8, 0.8, 0.8, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);

	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

	GLfloat mat_shininess = 0.0;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
}

void CGLRenderer::SetMaterial(float ambientR, float ambientG, float ambientB, float diffuseR, float diffuseG, float diffuseB, float specularR, float specularG, float specularB, float shininess)
{

	GLfloat ambient[] = { ambientR, ambientG, ambientB };
	GLfloat diffuse[] = { diffuseR, diffuseG, diffuseB };
	GLfloat specular[] = { specularR, specularG, specularB };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void CGLRenderer::DrawWall(double size, int n)
{
	double resStep = size / n;
	double height = -size / 2;
	double width = -size / 2;
	//x = 0.0
	double y = height;
	//glColor3f(0.9, 0.9, 0.9);
	GLfloat mat_amb[] = { 0.4, 0.4, 0.4, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);
	GLfloat mat_dif[] = { 0.6, 0.6, 0.6, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	GLfloat mat_shininess = 64.0;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);

	glBegin(GL_QUADS);
	for (int i = 0; i < n; i++)
	{
		double yy = y + resStep;
		double z = -width;
		for (int j = 0; j < n; j++)
		{
			double zz = z - resStep;
			glNormal3f(1.0, 0.0, 0.0);
			glVertex3f(0.0, y, zz);
			glNormal3f(1.0, 0.0, 0.0);
			glVertex3f(0.0, yy, zz);
			glNormal3f(1.0, 0.0, 0.0);
			glVertex3f(0.0, yy, z);
			glNormal3f(1.0, 0.0, 0.0);
			glVertex3f(0.0, y, z);
			z = zz;
		}
		y = yy;
	}
	glEnd();
}