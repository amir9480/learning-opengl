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

	static bool initialized = false;
	static std::vector<InstanceData> idata;
	static std::vector<mathfu::mat4> tempData;
	if (initialized == false) {
		for (int i = 0; i < 500; i++) {
			f32 rs = randomNumber(1, 3);
			tempData.push_back(mathfu::mat4::Transform(mathfu::vec3(randomNumber(-500, 500), randomNumber(1, 5), randomNumber(-500, 500)), mathfu::mat3::Identity(), mathfu::vec3(rs, rs, rs)).Transpose());
			idata.push_back(InstanceData(tempData[i]));
		}
		initialized = true;
	}
	static Mesh* sphere = Mesh::createSphere(120);
	sphere->setPosition(mathfu::vec3(0, 2, 10));
	sphere->setScale(mathfu::vec3(5, 5, 5));
	sphere->draw(_mainCamera, idata);
}

void Scene::postRender()
{
	Shader* lightShader = Shader::lightShader();
	for (auto node : mNodes) {
		renderLights(node);
	}

	static Mesh* lightSphere = nullptr;
	if (lightSphere == nullptr) {
		lightSphere = Mesh::createSphere(24, 24, true)->setMaterial(nullptr);
	}
	if (mLightDataInstances.size() > 2) {
		glFrontFace(GL_CW);
		mMainCamera->postProccess(lightShader, true, lightSphere, mLightDataInstances.data(), mLightDataInstances.size(), sizeof(LightInstanceData));
		glFrontFace(GL_CCW);
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
		} else {
			if (std::find(mLights.begin(), mLights.end(), light) == mLights.end()) {
				mLights.push_back(light);
				mLightDataInstances.push_back(light->toLightData());
			}
		}
	}
}
