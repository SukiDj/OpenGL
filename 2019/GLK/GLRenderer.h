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
	void DrawPolygon(POINTF* points, POINTF* texCoords, int n);
	void DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b);
	void DrawBase();
	void DrawBody();
	void DrawArm(double zh);
	void DrawFork();
	void DrawExcavator();
	void DrawEnvCube(double a);
	void UpdateView();
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	UINT LoadTexture(char* fileName);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	UINT excavator;
	UINT bckgr[6];
public:
	float dist;
	float alphaRot;
	float betaRot;
	float cabineRot;
	float arm1Rot;
	float arm2Rot;
	float forkRot;
};
