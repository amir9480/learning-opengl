#include "Light.h"

Light::Light(Type _type)
	:Node()
{
	if (_type != Type::Directional) {
		mTicking = false;
	}
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

Light* Light::setCone(float cone)
{
	mCone = cone;
	return this;
}

float Light::getCone() const
{
	return mCone;
}

void Light::setShaderParameters(Shader* lightShader) const
{
	lightShader->setBool("instancing", false);
	lightShader->setInt("lightType", (int)this->getType());
	lightShader->setFloat3("lightColor", this->getColor());
	lightShader->setFloat3("lightDirection", this->getForward());
	lightShader->setFloat("lightPower", this->getPower());
	lightShader->setFloat("lightRadius", this->getRadius());
	lightShader->setFloat("lightCone", this->getCone() * mathfu::kDegreesToRadians * 0.5f);
	lightShader->setFloat3("lightPosition", this->getPosition());
}

std::string Light::getClass() const
{
	return "Light";
}

LightInstanceData Light::toLightData() const
{
	static LightInstanceData out;
	out.world = this->getTransformMatrix();
	out.position = mPosition;
	out.direction = getForward();
	out.color = mColor;
	out.power = mPower;
	out.radius = mRadius;
	out.cone = mCone * mathfu::kDegreesToRadians * 0.5f;
	return out;
}

void Light::postRender(Camera* camera)
{
	if (mType == Type::Directional) {
		Shader* lightShader = Shader::lightShader();
		this->setShaderParameters(lightShader);
		lightShader->setMatrix("MVP", mathfu::mat4::Identity());
		camera->postProccess(lightShader, true);
	}
}
