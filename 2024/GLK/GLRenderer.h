#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DrawSphere(float R, int n, int m);
	void DrawAxis();
	void CalcRotAxis(double x1, double y1, double z1, double x2, double y2, double z2, double& x3, double& y3, double& z3);
	void DrawSphFlower(float R, int n, int m, float factor, unsigned char R1, unsigned char G1, unsigned char B1, unsigned char R2, unsigned char G2, unsigned char B2);
	void FLowerMaterial(unsigned char R1, unsigned char G1, unsigned char B1, unsigned char R2, unsigned char G2, unsigned char B2, int j, int m);
	void DrawFlower();
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	void PolarToCartesian(double R, double phi, double theta, double& x, double& y, double& z);
	void CalculateView();
	UINT LoadTexture(char* fileName);
	void TurnOnLight();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	UINT env;
public:
	float eyex, eyey, eyez;
	float centerx, centery, centerz;
	float upx, upy, upz;
	float dist;
	float alphaRot;
	float betaRot;
	float flowerRot;

	bool lightON;

	float m_vAmbient[4];
	float m_vDiffuse[4];
	float m_vSpecular[4];
	float m_vEmission[4];
	float m_fShininess;
};
