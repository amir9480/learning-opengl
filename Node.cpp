#include "Node.h"
#include "Scene.h"


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
	return (mathfu::vec3(0, 1, 0) * getGlobalRotation().ToMatrix()).Normalized();
}

mathfu::vec3 Node::getRight() const
{
	return (mathfu::vec3(1, 0, 0) * getGlobalRotation().ToMatrix()).Normalized();
}

mathfu::vec3 Node::getForward() const
{
	return (mathfu::vec3(0, 0, 1) * getGlobalRotation().ToMatrix()).Normalized();
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

mathfu::vec3 Node::getGlobalPosition() const
{
	if (mParent) {
		return mParent->getGlobalPosition() + mPosition;
	}
	return mPosition;
}

mathfu::quat Node::getGlobalRotation() const
{
	if (mParent) {
		return mParent->getGlobalRotation() * mRotation;
	}
	return mRotation;
}

mathfu::vec3 Node::getGlobalScale() const
{
	if (mParent) {
		return mParent->getGlobalPosition() * mScale;
	}
	return mScale;
}

mathfu::mat4 Node::getTransformMatrix() const
{
	if (mParent) {
		return mathfu::mat4::Transform(mPosition, mRotation.ToMatrix(), mScale).Transpose() * mParent->getTransformMatrix();
	}
	return mathfu::mat4::Transform(mPosition, mRotation.ToMatrix(), mScale).Transpose();
}

Node* Node::move(mathfu::vec3 _axis)
{
	mPosition += _axis;
	this->onEvent("move");
	return this;
}

Node* Node::rotate(mathfu::vec3 _axis, f32 _angle)
{
	if (_angle != 0) {
		mRotation = mRotation * mathfu::quat::FromAngleAxis(_angle * mathfu::kDegreesToRadians, _axis);
		this->onEvent("rotate");
	}
	return this;
}

Node* Node::setRotation(const mathfu::quat& _rotation)
{
	this->mRotation = _rotation;
	return this;
}

Node* Node::lookAt(const mathfu::vec3& _pos)
{
	this->mRotation = mathfu::quat::LookAt((_pos - mPosition).Normalized(), this->getUp());
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

Node* Node::setParent(Node* _node)
{
	if (mParent) {
		std::list<Node*>::iterator me = std::find(mChildren.begin(), mChildren.end(), this);
		if (me != mChildren.end()) {
			mChildren.erase(me);
		}
	}
	if (_node) {
		_node->mChildren.push_back(this);
	}
	mParent = _node;
	return this;
}

Node* Node::getParent()
{
	return mParent;
}

Node* Node::addChild(Node* _newChild)
{
	_newChild->setParent(this);
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

void Node::preRender(Camera* camera)
{
}

void Node::render(Camera* camera)
{
}

void Node::postRender(Camera* camera)
{
}

std::string Node::getClass() const
{
	return std::string("Node");
}

std::string Node::toString() const
{
	return std::string("Node(\n    Position:(") +
		::toString<f32>(mPosition.x) + "," + ::toString<f32>(mPosition.y) + "," + ::toString<f32>(mPosition.z) + ")\n    Rotation:(" +
		::toString<f32>(mRotation.ToEulerAngles().x * mathfu::kRadiansToDegrees) + "," + ::toString<f32>(mRotation.ToEulerAngles().y * mathfu::kRadiansToDegrees) + "," + ::toString<f32>(mRotation.ToEulerAngles().z * mathfu::kRadiansToDegrees) + ")\n    Scale:(" +
		::toString<f32>(mScale.x) + "," + ::toString<f32>(mScale.y) + "," + ::toString<f32>(mScale.z) + ")\n)\n";
}

void Node::onEvent(const std::string& name)
{
}

Node* Node::findChild(std::string name)
{
	for (auto& child : mChildren) {
		if (child->getName() == name) {
			return child;
		}
	}
	Node* out = nullptr;
	for (auto& child : mChildren) {
		if (out = child->findChild(name)) {
			return out;
		}
	}
	return nullptr;
}

std::list<Node*>& Node::getChildren()
{
	return mChildren;
}

Node* Node::setTicking(bool _val)
{
	mTicking = true;
	return this;
}

bool Node::getTicking() const
{
	return mTicking;
}

Scene* Node::getScene() const
{
	return mScene;
}
