#include "stdafx.h"
#include "GLTexture.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"

GLTexture::GLTexture()
{
    texID = 0;
}

GLTexture::~GLTexture()
{
    this->ReleaseTex();
}

void GLTexture::PrepareTexture(bool lightEnabled)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    if (lightEnabled)
    {
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
    else
    {
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    }
}

void GLTexture::LoadTexture(CString fileName)
{
    if (texID)
        this->ReleaseTex();

    DImage img;
    img.Load(CString(fileName));

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());

}

void GLTexture::Select()
{
    if (texID)
    {
        glBindTexture(GL_TEXTURE_2D, texID);
    }
}

void GLTexture::ReleaseTex()
{
    if (texID)
    {
        glDeleteTextures(1, &texID);
        texID = 0;
    }
}
