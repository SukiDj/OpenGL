#pragma once
#include "CGLMaterial.h"

#define	VAZA_BOJA_1 1.00, 0.00, 1.00
#define VAZA_BOJA_1_AMBIENT 0.25, 0, 0.25
#define VAZA_BOJA_1_SPECULAR 0.5, 0, 0.5

#define VAZA_BOJA_2 0.00, 1.00, 1.00
#define VAZA_BOJA_2_AMBIENT 0.00, 0.25, 0.25
#define VAZA_BOJA_2_SPECULAR 0, 0.5, 0.5

#define SVETLO_1 0.00, 1.00, 1.00
#define SVETLO_1_SPECULAR 0.0, 0.4, 0.4

#define SVETLO_2 1.00, 0.00, 1.00
#define SVETLO_2_SPECULAR 0.4, 0, 0.4

#define SVETLO_3 1.00, 0.00, 0.00
#define SVETLO_3_SPECULAR 0.4, 0, 0

#define LIGHT1_POS -10, 10, 0
#define LIGHT2_POS 10, 10, 0
#define LIGHT3_POS 0, 20, 0

constexpr double R_R1 = 3;
constexpr double R_R2 = 2;
constexpr double R_R3 = 1;
constexpr double R_H = 0.6;

struct Vertex {
	float x, y, z;
};


class CGLRenderer
{
public:
	double eyex, eyey, eyez;// pozicija kamere
	double centerx, centery, centerz;// tacka prema kojoj kamera gleda
	double upx, upy, upz;// definise smer kamere za orjentaciju
	double viewAngleXY;
	double viewAngleXZ;
	double viewR;
	bool light0;
	bool light1;
	bool light2;
	bool normals;

public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	void DrawAxis(double width);
	void CalculatePosition();
	void RotateView(double dXY, double dXZ);
	void ZoomCamera(bool direction);
	void DrawRoom();
	void DrawBase();
	void DrawHalfSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r1, double r2, CGLMaterial& mat, int nSeg = 40, bool includeBase = false);
	void DrawPlate(double h, double w, double d, int nSegH, int nSegW, int nSegD, CGLMaterial& mat);
	void DrawSideGrid(double h, double w, int nSegH, int nSegW, bool invert);
	void DrawVase();
	void DrawRing(double h, double r1, double r2, CGLMaterial& mat, int nSeg = 40, bool includeBase = false);
	void AmbientLight();
	void LampLight();
	void SetLightPositionAndDirection(GLenum light, double posX, double posY, double posZ, double dirX, double dirY, double dirZ, bool lightOn);
	void DrawLight(double posX, double posY, double posZ, double r, CGLMaterial& mat, bool lightOn);
	void ViewPointLight();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	float angle_v;
	float angle_h;


	CGLMaterial vaseMat1, vaseMat2, light1Mat, light2Mat, light3Mat, tableMat, wallMat;
};
