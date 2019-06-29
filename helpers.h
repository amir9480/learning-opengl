#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <vector>
#include <map>
#include <string>
#include <list>
#include <initializer_list>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mathfu/vector.h>
#include <mathfu/matrix.h>
#include <mathfu/constants.h>
#include <mathfu/glsl_mappings.h>
#include "stb_image.h"

#define ABORT(MSG) std::cout<<"ERROR "<<MSG<<"\n\nFile:"<<__FILE__<<"("<<__LINE__<<")"<<std::endl;exit(-1);
#define extends : public

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned int u32;
typedef unsigned long long int u64;
typedef float f32;
typedef double f64;
typedef long double f96;


std::string getFileContent(std::string name);

template<typename T>
std::string toString(T _num)
{
	std::stringstream ss;
	ss << _num;
	return ss.str();
}

#endif // _HELPERS_H_
