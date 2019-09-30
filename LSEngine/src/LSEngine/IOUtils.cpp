#include "lsepch.h"

#include "IOUtils.h"

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

}