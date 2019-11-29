#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "helpers.h"
#include "Node.h";
#include "Shader.h"
#include "Mesh.h"

struct LightInstanceData : public InstanceData
{
	mathfu::vec3 position = mathfu::vec3(0, 0, 0);
	mathfu::vec3 direction = mathfu::vec3(0, 0, 1);
	mathfu::vec3 color = mathfu::vec3(1, 1, 1);
	float power = 1.0;
	float radius = 10.0f;
};

class Light : public Node
{
public:
	enum Type {
		Directional,
		Point
	};

public:
	Light(Type _type = Type::Point);

	Light::Type getType() const;
	Light* setType(Light::Type _newType);

	mathfu::vec3 getColor() const;
	Light* setColor(mathfu::vec3 _newColor);

	Light* setPower(float power);
	float getPower() const;

	Light* setRadius(float radius);
	float getRadius() const;

	void setShaderParameters(Shader* lightShader) const;

	virtual std::string getClass() const;

	LightInstanceData toLightData() const;

	virtual void postRender(Camera* camera = nullptr);

	bool booted = false;
protected:
	Type mType;
	mathfu::vec3 mColor = mathfu::vec3(1, 1, 1);
	float mPower = 1.0;
	float mRadius = 10.0f;
};

#endif // _LIGHT_H_