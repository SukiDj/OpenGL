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
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawCube(double dSize);

	void DrawGrid(double meshsize);
	void DrawCilindar(double radius, double height, int n);
	void DrawKupa(double radius, double height, int n);
	void DrawZarubljenaKupa(double baseRadius, double topRadius, double height, int n);
	void DrawSfera(double radius);

	void DrawPoluSfera(double radius);

	void DrawVaza(int n);
	void DrawTruck();
	void DrawGround();
	UINT LoadTexture(char* fileName);
	void ResetMaterials();

	void SetMaterial(float ambientR, float ambientG, float ambientB, float diffuseR, float diffuseG, float diffuseB, float specularR, float specularG, float specularB, float shininess);

	void DrawWall(double size, int n);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context


	// camera
public:
	double cameraX = 0.0, cameraY = 0.0, cameraZ = 40.0;
	double angle = 10.0, angle2 = 10.0;

	bool normaleFleg = true;

	bool redLightFleg = true;
	bool greenLightFleg = true;
	bool blueLightFleg = true;
	// rotacije
public:
	int alfa = -45.0;
	int beta = 0;

};
