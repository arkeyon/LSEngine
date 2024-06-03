#include "lsepch.h"

#include "IOUtils.h"

#include <gli/gli.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
//#include <FreeImage.h>

#include <fstream>
#include <sstream>
#include <iostream>

namespace LSE {

	std::string ReadFile(const std::string& filepath)
	{
		FILE* file = fopen(filepath.c_str(), "rb");

		fseek(file, 0, SEEK_END);
		int length = ftell(file);
		std::string result(length, 0);
		fseek(file, 0, SEEK_SET);
		fread(&result[0], 1, length, file);
		fclose(file);
		return result;
	}

	//GLuint CreateTexture(char const* Filename)
	//{
	//	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(Filename);
	//
	//	if (format == FIF_UNKNOWN)
	//	{
	//		format = FreeImage_GetFIFFromFilename(Filename);
	//	}
	//
	//	FIBITMAP* img = FreeImage_Load(format, Filename);
	//	uint32_t width = FreeImage_GetWidth(img);
	//	uint32_t height = FreeImage_GetHeight(img);
	//
	//	GLuint TextureName = 0;
	//	glGenTextures(1, &TextureName);
	//	glBindTexture(GL_TEXTURE_2D, TextureName);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(img));
	//	
	//	return TextureName;
	//}
}