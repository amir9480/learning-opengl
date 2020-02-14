#include "Scene.h"
#include "Light.h"


Scene::Scene(std::string _name)
{
	mName = _name;
}

Scene::~Scene()
{
}

Node* Scene::addNode(Node* _newNode)
{
	this->addSubNodes(_newNode);
	return _newNode;
}

void Scene::addSubNodes(Node* _newNode)
{
	_newNode->mScene = this;
	mNodes.push_back(_newNode);
	if (_newNode->getTicking()) {
		mTickingNodes.push_back(_newNode);
	}
	if (_newNode->getClass() == "Light" && !_newNode->getTicking()) {
		Light* light = reinterpret_cast<Light*>(_newNode);
		mChangedLights.push_back(light);
	}
	for (Node* child : _newNode->getChildren()) {
		this->addSubNodes(child);
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

	/*static bool initialized = false;
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
	sphere->draw(_mainCamera, idata);*/
}

void Scene::postRender()
{
	Shader* lightShader = Shader::lightShader();
	Shader* ambientShader = Shader::ambientLightShader();
	if (mChangedLights.size() > 0) {
		for (auto node : mChangedLights) {
			renderLights(node);
		}
		mChangedLights.clear();
	}
	if (mPointLightDataInstancesIndex.size() > 0) {
		dynamic_cast<Light*>(mPointLightDataInstancesIndex.begin()->first)->setShaderParameters(lightShader);
	} else if (mSpotLightDataInstancesIndex.size() > 0) {
		dynamic_cast<Light*>(mSpotLightDataInstancesIndex.begin()->first)->setShaderParameters(lightShader);
	}

	ambientShader->setFloat4("color", mAmbientColor);
	mMainCamera->postProccess(ambientShader, true);

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


	for (auto node : mTickingNodes) {
		if (node->getClass() == "Camera") {
			reinterpret_cast<Camera*>(node)->applyPostProccesses();
		}
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

void Scene::setAmbientColor(const mathfu::vec4& _color)
{
	mAmbientColor = _color;
}

mathfu::vec4 Scene::getAmbientColor() const
{
	return mAmbientColor;
}

void Scene::renderLights(Node* node)
{
	Shader* lightShader = Shader::lightShader();
	Light* light = reinterpret_cast<Light*>(node);
	light->setShaderParameters(lightShader);

	if (light->getType() == Light::Point) {
		if (mPointLightDataInstancesIndex.find(node) != mPointLightDataInstancesIndex.end()) {
			mPointLightDataInstances[mPointLightDataInstancesIndex[node]] = light->toLightData();
		}
		else {
			mPointLightDataInstances.push_back(light->toLightData());
			mPointLightDataInstancesIndex[node] = mPointLightDataInstances.size() - 1;
		}
	}
	else if (light->getType() == Light::Spot) {
		if (mSpotLightDataInstancesIndex.find(node) != mSpotLightDataInstancesIndex.end()) {
			mSpotLightDataInstances[mSpotLightDataInstancesIndex[node]] = light->toLightData();
		}
		else {
			mSpotLightDataInstances.push_back(light->toLightData());
			mSpotLightDataInstancesIndex[node] = mSpotLightDataInstances.size() - 1;
		}
	}
}
