#include "Scene.h"
#include "Light.h"


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
		renderLights(node);
	}
	for (auto node : mNodes) {
		node->postRender();
	}
}

Node* Scene::find(std::string name)
{
	for (auto node : mNodes) {
		if (node->getName() == name) {
			return node;
		}
	}
	Node* out;
	for (auto node : mNodes) {
		if (out = node->findChild(name)) {
			return out;
		}
	}
	return nullptr;
}

std::list<Node*>& Scene::getNodes()
{
	return mNodes;
}

void Scene::renderLights(Node* node)
{
	Shader* lightShader = Shader::lightShader();
	if (node->getClass() == "Light") {
		Light* light = reinterpret_cast<Light*>(node);
		light->setShaderParameters(lightShader);

		if (light->getType() == Light::Directional) {
			lightShader->setMatrix("MVP", mathfu::mat4::Identity());
			mMainCamera->postProccess(lightShader, true);
		}
		else {
			glFrontFace(GL_CW);
			lightShader->setMatrix("MVP", light->getTransformMatrix() * mMainCamera->getViewProjection());
			mMainCamera->postProccess(lightShader, true, Mesh::sphere());
			glFrontFace(GL_CCW);
		}
	}
}
