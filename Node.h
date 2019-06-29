#ifndef _NODE_H_
#define _NODE_H_

#include "helpers.h"

class Node
{
public:
	Node();
	virtual ~Node();

	mathfu::vec3 getUp() const;
	mathfu::vec3 getRight() const;
	mathfu::vec3 getForward() const;

	mathfu::vec3 getDown() const;
	mathfu::vec3 getLeft() const;
	mathfu::vec3 getBackward() const;

	mathfu::vec3 getPosition() const;
	mathfu::quat getRotation() const;
	mathfu::vec3 getScale() const;

	mathfu::mat4 getTransformMatrix() const;

	void move(mathfu::vec3 _axis);

	void rotate(mathfu::vec3 _axis, f32 _angle);

	void setPosition(mathfu::vec3 _position);


	std::string toString() const;
protected:
	mathfu::vec3 mPosition = mathfu::vec3(0.0f, 0.0f, 0.0f);
	mathfu::quat mRotation;
	mathfu::vec3 mScale = mathfu::vec3(1.0f, 1.0f, 1.0f);
};


#endif // _NODE_H_
