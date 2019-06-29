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


