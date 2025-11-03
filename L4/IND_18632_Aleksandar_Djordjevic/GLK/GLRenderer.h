#pragma once
#include "gl\gl.h"

#define	POZADINA 0.47, 0.00, 0.00
#define VAZA 1.00, 1.00, 1.00
#define DEO_KAKTUSA 0.50, 0.75, 1.00
#define ZUTI_DEO_KAKTUSA 1, 1, 0
#define SFERA 1.00, 0.60, 0.00
#define GRID 0.00, 0.80, 0.00


class CGLRenderer
{
public:
	float angle = 0;

	double viewAngleXY;
	double viewAngleXZ;
	double viewR;

	double eyex, eyey, eyez;// pozicija kamere
	double centerx, centery, centerz;// tacka prema kojoj kamera gleda
	double upx, upy, upz;// definise smer kamere za orjentaciju
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r1, double r2, int nSeg);
	void DrawCone(double h, double r, int nSeg);
	void DrawAxis(double width);
	void DrawGrid(double width, double height, int nSegW, int nSegH);
	void DrawFigure(double angle);

	void RotateView(double dXY, double dXZ);
	void CalculatePosition();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
