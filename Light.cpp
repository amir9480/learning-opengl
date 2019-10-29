#include "Light.h"

Light::Light(Type _type)
	:Node()
{
	mType = _type;
}

Light::Type Light::getType() const
{
	return mType;
}

Light* Light::setType(Light::Type _newType)
{
	mType = _newType;
	return this;
}

mathfu::vec3 Light::getColor() const
{
	return mColor;
}

Light* Light::setColor(mathfu::vec3 _newColor)
{
	mColor = _newColor;
	return this;
}

Light* Light::setPower(float power)
{
	mPower = power;
	return this;
}

float Light::getPower() const
{
	return mPower;
}

Light* Light::setRadius(float radius)
{
	mRadius = radius;
	return this;
}

float Light::getRadius() const
{
	return mRadius;
}

std::string Light::getClass() const
{
	return "Light";
}
