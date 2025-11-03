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
	void DrawPatch(double R, int n);
	void DrawEarth(double R, int tes);
	void DrawMoon(double R, int tes);
	void DrawSpace(double R, int tes);
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	UINT LoadTexture(char* fileName);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	UINT T[6];
	UINT S[6];
	UINT M[6];
public:
	float m_alpha, m_beta;
	bool m_bLight;
	float m_dist;
	float m_moonRot;
};
