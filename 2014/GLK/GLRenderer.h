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
	void DrawCube(float a, float x, float y, float z, float count);
	void DrawRubikCube(double a, int count);
	void DrawAxis();
	void SetWhiteLight();
	void SetMaterial(float r, float g, float b);
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	void CalculateView();
	UINT LoadTexture(char* fileName);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	float eyex, eyey, eyez;
	float centerx, centery, centerz;
	float upx, upy, upz;
	UINT openGL;

public:
	float dist;
	float alphaRot;
	float betaRot;
	float lightON;

	float rotation[3];
	
};
