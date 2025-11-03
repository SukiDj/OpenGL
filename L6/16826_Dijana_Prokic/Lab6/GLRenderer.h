#pragma once
#include "GLTexture.h"
#include "GLMaterial.h"

class CGLRenderer
{
public:
	CGLRenderer();
	virtual ~CGLRenderer();
		
	bool CreateGLContext(CDC* pDC);			
	void PrepareScene(CDC* pDC);			
	void Reshape(CDC* pDC, int w, int h);	
	void DrawScene(CDC* pDC);				
	void DestroyScene(CDC* pDC);			
	void MoveCamera(CPoint cursorPoint);
	void StopMovingCamera();

protected:
	HGLRC m_hrc; 
	short rotateX;
	short rotateY;
	double cameraDistanceFromCoordinateOrigin;

// attributes
private:

	CPoint lastPoint;
	bool firstMouse;
	float kameraX;
	float kameraY;
	float kameraZ;
	float kameraPitch;
	float kameraYaw;
	

	
	GLTexture* terrain;


// functions
private:
	// importants
	
	void CrtajKoordinate();
	void CrtajPodlogu();



};
