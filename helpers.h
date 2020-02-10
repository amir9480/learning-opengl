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
#include <random>
#include <math.h>
#include <time.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mathfu/vector.h>
#include <mathfu/matrix.h>
#include <mathfu/constants.h>
#include <mathfu/glsl_mappings.h>
#include <imgui.h>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "3rdparty/includes/stb_image.h"

#define ABORT(MSG) std::cout<<"ERROR "<<MSG<<"\n\nFile:"<<__FILE__<<"("<<__LINE__<<")"<<std::endl;exit(-1);
#define extends : public

#define WINDOW_WIDTH 1360
#define WINDOW_HEIGHT 768
#define WINDOW_FULL_SCREEN false

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned int u32;
typedef unsigned long long int u64;
typedef float f32;
typedef double f64;
typedef long double f96;


std::string getFileContent(std::string name);

u32 maxAnisotropy();
int randomNumber(int _min, int _max);

template<typename T>
std::string toString(T _num)
{
	std::stringstream ss;
	ss << _num;
	return ss.str();
}

#endif // _HELPERS_H_
