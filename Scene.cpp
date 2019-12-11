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
	if (_newNode->getTicking()) {
		mTickingNodes.push_back(_newNode);
	}
	if (_newNode->getClass() == "Light" && !_newNode->getTicking()) {
		Light* light = reinterpret_cast<Light*>(_newNode);
		mLights.push_back(light);
	}
}

void Scene::setMainCamera(Camera* _camera)
{
	mMainCamera = _camera;
}

void Scene::preRender()
{
	for (auto node : mTickingNodes) {
		node->preRender(mMainCamera);
	}
}

void Scene::render()
{
	mMainCamera->render(&Scene::renderCallback, this);
}

void Scene::renderCallback(Camera* _mainCamera)
{
	for (auto node : mTickingNodes) {
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
	if (mLights.size() != mPointLightDataInstances.size() + mSpotLightDataInstances.size()) {
		for (auto node : mLights) {
			renderLights(node);
		}
	}
	if (mLights.size() > 0) {
		dynamic_cast<Light*>(mLights[0])->setShaderParameters(lightShader);
	}

	static Mesh* lightSphere = nullptr;
	if (lightSphere == nullptr) {
		lightSphere = Mesh::createSphere(24, 24, true)->setMaterial(nullptr);
	}
	if (mPointLightDataInstances.size() > 0) {
		glFrontFace(GL_CW);
		lightShader->setInt("lightType", Light::Type::Point);
		mMainCamera->postProccess(lightShader, true, lightSphere, mPointLightDataInstances.data(), mPointLightDataInstances.size(), sizeof(LightInstanceData));
		glFrontFace(GL_CCW);
	}
	if (mSpotLightDataInstances.size() > 0) {
		glFrontFace(GL_CW);
		lightShader->setInt("lightType", Light::Type::Spot);
		mMainCamera->postProccess(lightShader, true, lightSphere, mSpotLightDataInstances.data(), mSpotLightDataInstances.size(), sizeof(LightInstanceData));
		glFrontFace(GL_CCW);
	}

	for (auto node : mTickingNodes) {
		node->postRender(mMainCamera);
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
	Light* light = reinterpret_cast<Light*>(node);
	light->setShaderParameters(lightShader);

	if (!light->booted) {
		light->booted = true;
		if (light->getType() == Light::Point) {
			mPointLightDataInstances.push_back(light->toLightData());
		} else if (light->getType() == Light::Spot) {
			mSpotLightDataInstances.push_back(light->toLightData());
		}
	}
}
