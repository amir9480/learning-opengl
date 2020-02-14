#ifndef _NODE_H_
#define _NODE_H_

#include "helpers.h"

class Camera;
class Scene;

class Node
{
	friend class Scene;
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

	mathfu::vec3 getGlobalPosition() const;
	mathfu::quat getGlobalRotation() const;
	mathfu::vec3 getGlobalScale() const;

	mathfu::mat4 getTransformMatrix() const;

	Node* move(mathfu::vec3 _axis);

	Node* rotate(mathfu::vec3 _axis, f32 _angle);

	Node* setRotation(const mathfu::quat& _rotation);

	Node* lookAt(const mathfu::vec3& _pos);

	Node* setPosition(mathfu::vec3 _position);

	Node* setScale(mathfu::vec3 _scale); 

	Node* setName(std::string _name);

	Node* addTag(std::string _tag);

	Node* setParent(Node* _node);

	Node* getParent();

	Node* addChild(Node* _newChild);

	std::string getName() const;

	const std::vector<std::string>& getTags() const;

	virtual void preRender(Camera* camera = nullptr);

	virtual void render(Camera* camera = nullptr);

	virtual void postRender(Camera* camera = nullptr);

	virtual std::string getClass() const;

	virtual std::string toString() const;

	virtual void onEvent(const std::string& name);

	Node* findChild(std::string name);

	std::list<Node*>& getChildren();

	Node* setTicking(bool _val);
	bool getTicking() const;

	Scene* getScene() const; 

protected:
	Node*						mParent = nullptr;
	std::list<Node*>			mChildren;
	std::string	 				mName;
	std::vector<std::string>	mTags;
	mathfu::vec3				mPosition = mathfu::vec3(0.0f, 0.0f, 0.0f);
	mathfu::quat				mRotation;
	mathfu::vec3				mScale = mathfu::vec3(1.0f, 1.0f, 1.0f);
	bool						mTicking = true;
	Scene*						mScene = nullptr;
};


#endif // _NODE_H_
