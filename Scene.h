#ifndef _SCENE_H_
#define _SCENE_H_

#include "Node.h"
#include "helpers.h"
#include "Camera.h"
#include "Light.h"

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

	Node* find(std::string name);

	std::list<Node*>& getNodes();
protected:
	void renderLights(Node* node);

protected:
	std::string						mName;
	std::list<Node*>				mNodes;
	std::list<Node*>				mTickingNodes;
	Camera*							mMainCamera;
	std::vector<Light*>				mLights;
	std::vector<LightInstanceData>	mLightDataInstances;
};

#endif // _SCENE_H_