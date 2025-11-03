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
	void DrawEnvCube(double a);
	void DrawSphere(double r, int nSeg, double texU, double texV, double texR);
	void DrawCone(double r, double h, int nSeg, double texU, double texV, double texR);
	void DrawLegSegment(double r, double h, int nSeg);
	void DrawLeg();
	void DrawSpiderBody();
	void DrawSpider();
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	void UpdateCamera();
	UINT LoadTexture(char* fileName);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	UINT envCube[6];
	UINT spider;
public:
	float dist;
	float angle1;
	float angle2;
	bool light;

};
