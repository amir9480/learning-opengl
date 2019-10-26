#include "Light.h"

Light::Light(Type _type)
{
	mType = _type;
}

std::string Light::getClass() const
{
	return "Light";
}
