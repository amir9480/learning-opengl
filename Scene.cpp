#include "Scene.h"


Scene::Scene(std::string _name)
{
	mName = _name;
}

Scene::~Scene()
{
	for (auto node : mNodes) {
		delete node;
	}
}

void Scene::addNode(Node* _newNode)
{
	mNodes.push_back(_newNode);
}

void Scene::setMainCamera(Node* _camera)
{
}

void Scene::preRender()
{
	for (auto node : mNodes) {
		node->preRender();
	}
}

void Scene::render()
{
	for (auto node : mNodes) {
		node->render();
	}
}

void Scene::postRender()
{
	for (auto node : mNodes) {
		node->postRender();
	}
}
