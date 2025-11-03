#pragma once
#include <vector>
#include <Windows.h>
#include "DImage.h"
#include "GL\gl.h"

struct Particle {
	float x, y, z;
	float vx, vy, vz;
	float life;
	float maxLife;
	float size;
	bool isSmoke;
	float twist;
};

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
	void DrawAxis();
	void CalculateView();
	void InitFire();
	void UpdateFire(float dt);
	void RenderFire();
	GLuint LoadTexture(const char* fileName);

protected:
	HGLRC	 m_hrc;
	std::vector<Particle> particles;
	GLuint fireTex;
	int maxParticles;
	float emitRate;
	float emitAccumulator;
	float emitterX, emitterY, emitterZ;
	DWORD lastTimeMs;


public:
	float eyex, eyey, eyez;
	float centerx, centery, centerz;
	float upx, upy, upz;
	float dist;
	float alphaRot;
	float betaRot;
};
