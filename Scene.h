#ifndef _SCENE_H_
#define _SCENE_H_

#include "Node.h"
#include "helpers.h"
#include "Camera.h"
#include "Light.h"

class Scene
{
	friend class Light;
public:
	Scene(std::string _name);
	virtual ~Scene();

	Node* addNode(Node* _newNode);

	void addSubNodes(Node* _newNode);

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
	std::vector<Light*>				mChangedLights;
	std::vector<LightInstanceData>	mPointLightDataInstances;
	std::vector<LightInstanceData>	mSpotLightDataInstances;
	std::map<Node*, u32>			mPointLightDataInstancesIndex;
	std::map<Node*, u32>			mSpotLightDataInstancesIndex;
};

#endif // _SCENE_H_