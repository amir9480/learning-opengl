#include "Node.h"


Node::Node()
{
	mRotation = mathfu::quat::identity;
	mName = "random_" + ::toString<int>(randomNumber(1, INT_MAX));
}

Node::~Node()
{
}

mathfu::vec3 Node::getUp() const
{
	return (mRotation * mathfu::vec3(0, 1, 0)).Normalized();
}

mathfu::vec3 Node::getRight() const
{
	return (mRotation * mathfu::vec3(1, 0, 0)).Normalized();
}

mathfu::vec3 Node::getForward() const
{
	return (mRotation * mathfu::vec3(0, 0, 1)).Normalized();
}

mathfu::vec3 Node::getDown() const
{
	return -getUp();
}

mathfu::vec3 Node::getLeft() const
{
	return -getRight();
}

mathfu::vec3 Node::getBackward() const
{
	return -getForward();
}

mathfu::vec3 Node::getPosition() const
{
	return mPosition;
}

mathfu::quat Node::getRotation() const
{
	return mRotation;
}

mathfu::vec3 Node::getScale() const
{
	return mScale;
}

mathfu::mat4 Node::getTransformMatrix() const
{
	return mathfu::mat4::Transform(mPosition, mRotation.ToMatrix(), mScale).Transpose();
}

Node* Node::move(mathfu::vec3 _axis)
{
	mPosition += _axis;
	return this;
}

Node* Node::rotate(mathfu::vec3 _axis, f32 _angle)
{
	if (_angle != 0) {
		mRotation = mRotation * mathfu::quat::FromAngleAxis(_angle, _axis * mathfu::kRadiansToDegrees);
	}
	return this;
}

Node* Node::setPosition(mathfu::vec3 _position)
{
	mPosition = _position;
	return this;
}

Node* Node::setScale(mathfu::vec3 _scale)
{
	mScale = _scale;
	return this;
}

Node* Node::setName(std::string _name)
{
	mName = _name;
	return this;
}

Node* Node::addTag(std::string _tag)
{
	mTags.push_back(_tag);
	return this;
}

std::string Node::getName() const
{
	return mName;
}

const std::vector<std::string>& Node::getTags() const
{
	return mTags;
}

void Node::preRender()
{
}

void Node::render()
{
}

void Node::postRender()
{
}

std::string Node::toString() const
{
	return std::string("Node(\n    Position:(") +
		::toString<f32>(mPosition.x) + "," + ::toString<f32>(mPosition.y) + "," + ::toString<f32>(mPosition.z) + ")\n    Rotation:(" +
		::toString<f32>(mRotation.ToEulerAngles().x * mathfu::kRadiansToDegrees) + "," + ::toString<f32>(mRotation.ToEulerAngles().y * mathfu::kRadiansToDegrees) + "," + ::toString<f32>(mRotation.ToEulerAngles().z * mathfu::kRadiansToDegrees) + ")\n    Scale:(" +
		::toString<f32>(mScale.x) + "," + ::toString<f32>(mScale.y) + "," + ::toString<f32>(mScale.z) + ")\n)\n";
}
