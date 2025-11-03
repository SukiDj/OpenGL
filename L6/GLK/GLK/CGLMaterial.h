#pragma once
#include "GL/GL.H"
#include "GL/GLU.h"

class CGLMaterial
{
public:
	CGLMaterial();
	~CGLMaterial();

	void SetFace(GLenum face = GL_FRONT_AND_BACK);
	void SetAmbient(float r, float g, float b, float a);
	void SetDiffuse(float r, float g, float b, float a);
	void SetSpecular(float r, float g, float b, float a);
	void SetEmission(float r, float g, float b, float a);
	void SetShininess(float s);

protected:
	float m_vAmbient[4];
	float m_vDiffuse[4];
	float m_vSpecular[4];
	float m_vEmission[4];
	float m_fShininess;
};

