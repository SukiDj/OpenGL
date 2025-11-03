#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	m_hrc = NULL;
	fireTex = 0;
	maxParticles = 800;
	emitRate = 250.0f;
	emitAccumulator = 0.0f;
	emitterX = 0.0f; emitterY = -5.0f; emitterZ = 0.0f;
	lastTimeMs = 0;
	srand((unsigned)time(NULL));

    dist = 15;
    alphaRot = 0;
    betaRot = 0;
    eyex = eyey = eyez = 0;
    centerx = centery = centerz = 0;
    upx = 0;
    upy = 1;
    upz = 0;
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    InitFire();
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    CalculateView();

    //DrawAxis();

    DWORD now = GetTickCount64();
    float dt = 0.016f; // fallback ~60fps
    if (lastTimeMs != 0) {
        dt = (now - lastTimeMs) / 1000.0f;
        if (dt > 0.1f) dt = 0.1f; // clamp
    }
    lastTimeMs = now;

    UpdateFire(dt);   // menja particles
    RenderFire();     // iscrtava ih

    glFlush();
    SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50, (double)w / (double)h, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	
    if (fireTex) {
        glDeleteTextures(1, &fireTex);
        fireTex = 0;
    }

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxis()
{
    glLineWidth(2);
    glBegin(GL_LINES);
    {
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(10, 0, 0);

        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 10, 0);

        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 10);
    }
    glEnd();
}

GLuint CGLRenderer::LoadTexture(const char* fileName)
{
	DImage img;
	img.Load(CString(fileName));

	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGR_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::InitFire()
{
    if (fireTex) {
        glDeleteTextures(1, &fireTex);
        fireTex = 0;
    }
    fireTex = LoadTexture("res/fire.png");

    particles.clear();
    emitAccumulator = 0.0f;
    lastTimeMs = GetTickCount64();

    maxParticles = 1200;
    emitRate = 350.0f;
}


void CGLRenderer::UpdateFire(float dt)
{
    float toEmit = emitRate * dt;
    emitAccumulator += toEmit;

    int newCount = (int)emitAccumulator;
    emitAccumulator -= newCount;

    for (int k = 0; k < newCount; ++k) {
        if ((int)particles.size() >= maxParticles) break;
        Particle p;

        p.x = emitterX + ((float)rand() / RAND_MAX - 0.5f) * 0.25f;
        p.y = emitterY + ((float)rand() / RAND_MAX) * 0.18f;
        p.z = emitterZ + ((float)rand() / RAND_MAX - 0.5f) * 0.25f;

        p.vx = ((float)rand() / RAND_MAX - 0.5f) * 0.8f;
        p.vy = 1.0f + (float)rand() / RAND_MAX * 1.6f;
        p.vz = ((float)rand() / RAND_MAX - 0.5f) * 0.8f;

        p.maxLife = 1.0f + (float)rand() / RAND_MAX * 1.2f;
        p.life = p.maxLife;
        p.size = 0.25f + (float)rand() / RAND_MAX * 1.0f;

        p.isSmoke = false;
        p.twist = ((float)rand() / RAND_MAX) * 6.28318f;

        particles.push_back(p);
    }

    const float smokeTransitionThreshold = 0.1f;
    const float flameMinHeightForSmoke = 0.8f;
    const float maxVisibleHeight = 12.0f;
    for (int i = (int)particles.size() - 1; i >= 0; --i) {
        Particle& p = particles[i];

        float lifeDecay = p.isSmoke ? 0.25f : 0.9f;
        p.life -= dt * lifeDecay;

        p.vy += (0.6f + 0.4f * ((float)rand() / RAND_MAX)) * dt;
        float t = (p.maxLife - p.life) * 3.0f;
        p.x += (p.vx + 0.12f * sinf(p.twist + t)) * dt;
        p.z += (p.vz + 0.12f * cosf(p.twist + t)) * dt;
        p.y += p.vy * dt;

        if (!p.isSmoke) p.size += 0.6f * dt;
        else p.size += 1.2f * dt;

        float lifeFrac = p.life / p.maxLife;
        if (!p.isSmoke && lifeFrac < smokeTransitionThreshold && (p.y - emitterY) > flameMinHeightForSmoke) {
            p.isSmoke = true;
            p.vx *= 0.22f;
            p.vz *= 0.22f;
            p.vy = 0.7f + (float)rand() / RAND_MAX * 0.6f;
            p.size *= 1.2f;
            p.life += 0.9f;
            if (p.life > p.maxLife * 2.0f) p.life = p.maxLife * 2.0f;
        }

        if (p.life <= 0.0f || p.y > emitterY + maxVisibleHeight) {
            particles[i] = particles.back();
            particles.pop_back();
        }
    }
}



void CGLRenderer::RenderFire()
{
    if (particles.empty() || fireTex == 0) return;

    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fireTex);

    glDepthMask(GL_FALSE);
    glDisable(GL_LIGHTING);

    GLfloat mv[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);
    float rx = mv[0], ry = mv[4], rz = mv[8];
    float ux = mv[1], uy = mv[5], uz = mv[9];

    const float maxHeight = 6.5f;
    const float taperAmount = 0.85f;

    // smoke
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    for (size_t i = 0; i < particles.size(); ++i) {
        Particle& p = particles[i];
        if (!p.isSmoke) continue;

        float lifeFrac = p.life / (p.maxLife * 1.0f);
        lifeFrac = max(0.0f, min(1.0f, lifeFrac));

        float alpha = 0.35f * lifeFrac;
        float r = 0.58f * (0.7f + 0.3f * (1.0f - lifeFrac));
        float g = 0.58f * (0.7f + 0.3f * (1.0f - lifeFrac));
        float b = 0.65f * (0.7f + 0.3f * (1.0f - lifeFrac));

        float insideFlameFactor = 1.0f;
        float flameCutHeight = emitterY + (maxHeight * 0.6f);
        if (p.y < flameCutHeight) {
            float fh = (p.y - emitterY) / (flameCutHeight - emitterY);
            fh = max(0.0f, min(1.0f, fh));
            insideFlameFactor = fh;
            alpha *= insideFlameFactor * 0.85f;
        }

        float heightFrac = (p.y - emitterY) / (maxHeight * 1.3f);
        if (heightFrac < 0.0f) heightFrac = 0.0f;
        if (heightFrac > 1.0f) heightFrac = 1.0f;
        float taper = 1.0f - powf(heightFrac, 1.2f) * (taperAmount * 0.3f);

        float half = p.size * 0.5f;
        float halfRight = half * taper;
        float halfUp = half;

        float rxs = rx * halfRight, rys = ry * halfRight, rzs = rz * halfRight;
        float uxs = ux * halfUp, uys = uy * halfUp, uzs = uz * halfUp;

        float vx0 = p.x - rxs - uxs;
        float vy0 = p.y - rys - uys;
        float vz0 = p.z - rzs - uzs;

        float vx1 = p.x + rxs - uxs;
        float vy1 = p.y + rys - uys;
        float vz1 = p.z + rzs - uzs;

        float vx2 = p.x + rxs + uxs;
        float vy2 = p.y + rys + uys;
        float vz2 = p.z + rzs + uzs;

        float vx3 = p.x - rxs + uxs;
        float vy3 = p.y - rys + uys;
        float vz3 = p.z - rzs + uzs;

        glColor4f(r, g, b, alpha);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(vx0, vy0, vz0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(vx1, vy1, vz1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(vx2, vy2, vz2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(vx3, vy3, vz3);
    }
    glEnd();

    // fire
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBegin(GL_QUADS);
    for (size_t i = 0; i < particles.size(); ++i) {
        Particle& p = particles[i];
        if (p.isSmoke) continue;

        float lifeFrac = p.life / p.maxLife;
        lifeFrac = max(0.0f, min(1.0f, lifeFrac));

        float alpha = lifeFrac;
        float r = 1.0f;
        float g = 0.6f * lifeFrac + 0.25f;
        float b = 0.15f * lifeFrac;

        // ako je pri kraju
        if (lifeFrac < 0.35f) {
            float t = lifeFrac / 0.35f;
            r = 0.8f + 0.2f * t;
            g = 0.2f * t;
            b = 0.05f * t;
            alpha = 0.5f * lifeFrac;
        }

        float heightFrac = (p.y - emitterY) / maxHeight;
        if (heightFrac < 0.0f) heightFrac = 0.0f;
        if (heightFrac > 1.0f) heightFrac = 1.0f;
        float taper = 1.0f - powf(heightFrac, 1.8f) * taperAmount;

        float half = p.size * 0.5f;
        float halfRight = half * taper;
        float halfUp = half;

        float rxs = rx * halfRight, rys = ry * halfRight, rzs = rz * halfRight;
        float uxs = ux * halfUp, uys = uy * halfUp, uzs = uz * halfUp;

        float vx0 = p.x - rxs - uxs;
        float vy0 = p.y - rys - uys;
        float vz0 = p.z - rzs - uzs;

        float vx1 = p.x + rxs - uxs;
        float vy1 = p.y + rys - uys;
        float vz1 = p.z + rzs - uzs;

        float vx2 = p.x + rxs + uxs;
        float vy2 = p.y + rys + uys;
        float vz2 = p.z + rzs + uzs;

        float vx3 = p.x - rxs + uxs;
        float vy3 = p.y - rys + uys;
        float vz3 = p.z - rzs + uzs;

        glColor4f(r, g, b, alpha);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(vx0, vy0, vz0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(vx1, vy1, vz1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(vx2, vy2, vz2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(vx3, vy3, vz3);
    }
    glEnd();

    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glPopAttrib();
}

void CGLRenderer::CalculateView()
{
    eyex = dist * cos(alphaRot * 3.14159 / 180) * cos(betaRot * 3.14159 / 180);
    eyey = dist * sin(alphaRot * 3.14159 / 180);
    eyez = dist * cos(alphaRot * 3.14159 / 180) * sin(betaRot * 3.14159 / 180);
    gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}