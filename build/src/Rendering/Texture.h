#pragma once
#include "../pch.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

/*
Needs rule of 5 eventually
*/

//texture id is used wrong ... need to separate gpu id and cpu id

class Texture
{
	GLuint m_textureID = -1;
	GLuint m_gpuTextureID = -1;
	std::string m_filePath;
	std::string m_name;

	//<Image Data>
	bool isBMP = false;
	bool isDDS = false;
	unsigned char header[124];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	unsigned int linearSize;
	unsigned int mipMapCount;
	unsigned int fourCC;
	unsigned int components;
	unsigned int format;
	unsigned int dataSize;
	// Actual RGB data
	unsigned char* data = nullptr;
	//</Image Data>

public:
	GLuint getUniform(GLuint programID, const std::string& name);
	GLuint loadIntoGPU();
	bool loadDDS(const std::string& filePath, const std::string& name);
	bool loadBMP(const std::string& filePath, const std::string& name);

	constexpr GLuint getID() { return m_textureID; }

	~Texture() 
	{ 
		delete[] data; data = nullptr;
	}

};

