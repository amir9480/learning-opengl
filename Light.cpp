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
	mScale = mathfu::vec3(1.0, 1.0, 1.0) * radius;
	return this;
}

float Light::getRadius() const
{
	return mRadius;
}

void Light::setShaderParameters(Shader* lightShader) const
{
	lightShader->setInt("lightType", (int)this->getType());
	lightShader->setFloat3("lightColor", this->getColor());
	lightShader->setFloat3("lightDirection", this->getForward());
	lightShader->setFloat("lightPower", this->getPower());
	lightShader->setFloat("lightRadius", this->getRadius());
	lightShader->setFloat3("lightPosition", this->getPosition());
}

std::string Light::getClass() const
{
	return "Light";
}

LightInstanceData Light::toLightData() const
{
	static LightInstanceData out;
	out.position = mPosition;
	out.color = mColor;
	out.power = mPower;
	out.radius = mRadius;
	return out;
}
