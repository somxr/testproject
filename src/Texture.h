#pragma once

#include <GL/glew.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();
private:
	//everything has an ID which is on the graphics card and we bind that ID to use that object (like VAOs and VBOs)
	GLuint textureID;
	int width, height, bitDepth;
	
	const char* fileLocation;
};

