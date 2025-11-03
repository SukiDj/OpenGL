#pragma once
class GLTexture
{
public:
	UINT texID;

	GLTexture();
	~GLTexture();

	void PrepareTexture(bool lightEnabled);
	void LoadTexture(CString fileName);
	void Select();
	void ReleaseTex();
};

