#include "Light.h"
#include "Scene.h"

Light::Light(Type _type)
	:Node()
{
	if (_type != Type::Directional) {
		mTicking = false;
	}
	else {
		static const u32 mapSizes[4] = {
			20,
			50,
			200,
			600
		};
		for (int i = 0; i < 4; i++) {
			mShadowCameras.push_back(new Camera(1024, 1024, Camera::ProjectionType::Ortho, 1, true));
			mShadowCameras[i]->setName("VIRTUAL_CAMERA");
			mShadowCameras[i]->setCullMode(Mesh::CullMode::Back);
			mShadowCameras[i]->setFar(750.0f);
			//mShadowCameras[i]->getDepth()->setFilter(Texture::Linear);
			mShadowCameras[i]->setOrthoSize(mapSizes[i]);
		}
	}
	mType = _type;
}

Light::~Light()
{
	for (int i = 0; i < mShadowCameras.size(); i++) {
		delete mShadowCameras[i];
	}
	mShadowCameras.clear();
}

Light::Type Light::getType() const
{
	return mType;
}

Light* Light::setType(Light::Type _newType)
{
	mType = _newType;
	return this;
}

mathfu::vec3 Light::getColor() const
{
	return mColor;
}

Light* Light::setColor(mathfu::vec3 _newColor)
{
	mColor = _newColor;
	return this;
}

Light* Light::setPower(float power)
{
	mPower = power;
	return this;
}

float Light::getPower() const
{
	return mPower;
}

Light* Light::setRadius(float radius)
{
	mRadius = radius;
	mScale = mathfu::vec3(1.0, 1.0, 1.0) * radius;
	return this;
}

float Light::getRadius() const
{
	return mRadius;
}

Light* Light::setCone(float cone)
{
	mCone = cone;
	return this;
}

float Light::getCone() const
{
	return mCone;
}

void Light::setShaderParameters(Shader* lightShader) const
{
	lightShader->setBool("instancing", false);
	lightShader->setInt("lightType", (int)this->getType());
	lightShader->setFloat3("lightColor", this->getColor());
	lightShader->setFloat3("lightDirection", this->getForward());
	lightShader->setFloat("lightPower", this->getPower());
	lightShader->setFloat("lightRadius", this->getRadius());
	lightShader->setFloat("lightCone", this->getCone() * mathfu::kDegreesToRadians * 0.5f);
	lightShader->setFloat3("lightPosition", this->getPosition());
	if (mShadowCameras.size() > 0) {
		for (int i = 0; i < mShadowCameras.size(); i++) {
			lightShader->setMatrix("shadowVP[" + std::to_string(i) + "]", mShadowCameras[i]->getViewProjection());
			lightShader->setTexture("theShadowMap[" + std::to_string(i) + "]", mShadowCameras[i]->getDepth());
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			lightShader->setMatrix("shadowVP[" + std::to_string(i) + "]", mathfu::mat4::Identity());
			lightShader->setTexture("theShadowMap[" + std::to_string(i) + "]", nullptr);
		}
	}
}

std::string Light::getClass() const
{
	return "Light";
}

LightInstanceData Light::toLightData() const
{
	static LightInstanceData out;
	out.world = this->getTransformMatrix();
	out.position = mPosition;
	out.direction = getForward();
	out.color = mColor;
	out.power = mPower;
	out.radius = mRadius;
	out.cone = mCone * mathfu::kDegreesToRadians * 0.5f;
	return out;
}

void Light::render(Camera* camera)
{
	if (mType == Type::Directional && camera->getName() != "VIRTUAL_CAMERA") {
		for (int i = 0; i < mShadowCameras.size(); i++) {
			mathfu::vec3 shadowCameraPosition = this->getBackward() * ((camera->getFar() - camera->getNear()) / 3.0f) + camera->getPosition();
			shadowCameraPosition.x -= fmod(shadowCameraPosition.x, 1.0 / mShadowCameras[i]->getWidth());
			shadowCameraPosition.y -= fmod(shadowCameraPosition.y, 1.0 / mShadowCameras[i]->getWidth());
			shadowCameraPosition.z -= fmod(shadowCameraPosition.z, 1.0 / mShadowCameras[i]->getWidth());
			mShadowCameras[i]->setPosition(shadowCameraPosition);
			mShadowCameras[i]->lookAt(camera->getPosition());
			mShadowCameras[i]->render(&Scene::renderCallback, mScene);
		}
	}
}

void Light::postRender(Camera* camera)
{
	if (mType == Type::Directional && camera->getName() != "VIRTUAL_CAMERA") {
		Shader* lightShader = Shader::lightShader();
		this->setShaderParameters(lightShader);
		lightShader->setBool("hasShadow", true);
		lightShader->setMatrix("MVP", mathfu::mat4::Identity());
		camera->postProccess(lightShader, true);
		lightShader->setTexture("theShadowMap", 0);
		lightShader->setBool("hasShadow", false);
	}
}

void Light::onEvent(const std::string& name)
{
	// If light moved or rotated then update light cache in scene
	if (mType != Directional && (name == "move" || name == "rotate")) {
		this->updateCache();
	}
}

void Light::updateCache()
{
	if (mScene) {
		mScene->mChangedLights.push_back(this);
	}
}

LightInstanceData LightInstanceData::operator=(const LightInstanceData& _other)
{
	this->world = _other.world;
	this->position = _other.position;
	this->direction = _other.direction;
	this->color = _other.color;
	this->power = _other.power;
	this->cone = _other.cone;
	this->radius = _other.radius;

	return *this;
}
