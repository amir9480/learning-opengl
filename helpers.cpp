#include "helpers.h"

#include "mesh.h"
#include "shader.h"

extern "C"
{
	__declspec(dllexport) int NvOptimusEnablement = 1;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

std::string getFileContent(std::string name)
{
	std::ifstream t(name.c_str());
	if (t.fail()) {
		std::cout << "ERROR: File " << name << " Not found\n";
		exit(-1);
	}
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}

u32 maxAnisotropy()
{
	f32 maxAnis = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnis);
	return maxAnis;
}

int randomNumber(int _min, int _max)
{
	static bool randSeeded = false;
	if (!randSeeded) {
		srand(time(0));
		randSeeded = true;
	}
	return (rand() % (_max - _min)) + _min;
}


