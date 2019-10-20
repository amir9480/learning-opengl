#include "Scene.h"


Scene::Scene(std::string _name)
{
	mName = _name;
}

Scene::~Scene()
{
}

void Scene::addNode(Node* _newNode)
{
	mNodes.push_back(_newNode);
}

void Scene::setMainCamera(Camera* _camera)
{
	mMainCamera = _camera;
}

void Scene::preRender()
{
	for (auto node : mNodes) {
		node->preRender();
	}
}

void Scene::render()
{
	mMainCamera->render(&Scene::renderCallback, this);
}

void Scene::renderCallback(Camera* _mainCamera)
{
	for (auto node : mNodes) {
		node->render(_mainCamera);
	}
}

void Scene::postRender()
{
	for (auto node : mNodes) {
		node->postRender();
	}
}
