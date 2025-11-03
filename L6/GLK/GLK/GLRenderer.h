#pragma once
#include "CGLMaterial.h"
#include "GLTexture.h"

class CGLRenderer
{
public:
	double eyex, eyey, eyez;// pozicija kamere
	double centerx, centery, centerz;// tacka prema kojoj kamera gleda
	double upx, upy, upz;// definise smer kamere za orjentaciju
	double viewAngleXY;
	double viewAngleXZ;
	double viewR;
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DrawAxis(double width);
	void DrawGrass(double a);
	void DrawTruck();
	void DrawSide(bool left);
	void DrawQuads();
	void DrawWheels();
	void DrawWheel(float center, float z1, float z2);
	void DrawSphere();
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void CalculatePosition();
	void RotateView(double dXY, double dXZ);
	void ZoomCamera(bool direction);
	void AmbientLight();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	CGLMaterial grassMat;
	CGLMaterial truckMat;
	CGLMaterial tireMat;
	CGLMaterial elipseMat;

	GLTexture* grassTex;
	GLTexture* truckTex;
	GLTexture* tireTex;
	GLTexture* elipseTex;
};
