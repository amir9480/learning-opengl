#ifndef _SCENE_H_
#define _SCENE_H_

#include "Node.h"
#include "helpers.h"
#include "Camera.h"

class Scene
{
public:
	Scene(std::string _name);
	virtual ~Scene();

	void addNode(Node* _newNode);

	void setMainCamera(Camera* _camera);

	void preRender();

	void render();

	void renderCallback(Camera* _mainCamera);

	void postRender();
protected:
	std::string				mName;
	std::vector<Node*>		mNodes;
	Camera*					mMainCamera;
};

#endif // _SCENE_H_