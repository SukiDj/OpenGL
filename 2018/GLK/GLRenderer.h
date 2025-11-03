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
	void DrawAxis();
	void DrawTriangle(float d1, float d2, float rep);
	void DrawShip();
	void DrawSpaceCube(double a);
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	UINT LoadTexture(char* fileName);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
public:
	float eyex;
	float eyey;
	float eyez;
	float centerx;
	float centery;
	float centerz;
	float upx;
	float upy;
	float upz;
	UINT shipTex;
	UINT spaceTex[6];

	float alphaRot, betaRot;
	bool light;
	float dist;
};
