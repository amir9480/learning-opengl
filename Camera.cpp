#include "Camera.h"

Camera::Camera(u32 width, u32 height, ProjectionType projectionType, f32 aspectRatio, bool _depthOnly)
	:Node()
{
	mWidth = width;
	mHeight = height;
	if (aspectRatio == 0.0f)
	{
		mAspectRatio = (float)width / height;
	}
	else
	{
		mAspectRatio = aspectRatio;
	}
	mProjectionType = projectionType;

	glGenFramebuffers(1, &mFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

	mDepthOnly = _depthOnly;
	if (!_depthOnly) {
		std::vector<std::string> gBuffers = { "albedo", "normal", "tangent" };
		for (u32 index = 0; index < gBuffers.size(); index++) {
			mGBuffer[gBuffers[index]] = new Texture(mWidth, mHeight);
			glBindTexture(GL_TEXTURE_2D, mGBuffer[gBuffers[index]]->mTexture);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, mGBuffer[gBuffers[index]]->mTexture, 0);
		}
		static u32 attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);
	}


	mDepth = new Texture(mWidth, mHeight, true);
	glBindTexture(GL_TEXTURE_2D, mDepth->mTexture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepth->mTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		ABORT("FRAME BUFFER NOT WORKING!");
	}

	// Post proccess framebuffer
	glGenFramebuffers(1, &mPostProccessFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mPostProccessFrameBuffer);
	mPostProccessTexture = new Texture(mWidth, mHeight);
	mFinalImage = new Texture(mWidth, mHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Camera::~Camera()
{
	for (auto gb : mGBuffer) {
		delete gb.second;
	}
	for (auto pps : mPostProccessShaders) {
		delete pps.second;
	}
	delete mDepth;
	delete mPostProccessTexture;
	delete mFinalImage;
	glDeleteFramebuffers(1, &mFrameBuffer);
	glDeleteFramebuffers(1, &mPostProccessFrameBuffer);
}

void Camera::applyPostProccesses()
{
	if (!mDepthOnly) {
		for (auto postProccessShader : mPostProccessShaders) {
			this->postProccess(postProccessShader.second);
		}
	}
}

mathfu::mat4 Camera::getView() const
{
	return mView;
}

mathfu::mat4 Camera::getProjection() const
{
	return mProjection;
}

mathfu::mat4 Camera::getViewProjection() const
{
	return mViewProjection;
}

void Camera::postProccess(Shader* shader, bool blend, Mesh* mesh, InstanceData* instanceData, u32 count, u32 size)
{
	shader->use();
	shader->setBool("instancing", instanceData != nullptr);
	shader->setFloat3("camPos", this->getPosition());
	shader->setMatrix("viewProjection", this->getViewProjection());
	shader->setMatrix("view", this->getView());
	shader->setMatrix("projection", this->getProjection());
	shader->setMatrix("viewProjectionInv", this->getViewProjection().Inverse());
	shader->setMatrix("viewInv", this->getView().Inverse());
	shader->setMatrix("projectionInv", this->getProjection().Inverse());
	shader->setFloat2("resolution", mWidth, mHeight);

	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_NEVER);
	glBindFramebuffer(GL_FRAMEBUFFER, mPostProccessFrameBuffer);
	glBindTexture(GL_TEXTURE_2D + 15, mFinalImage->mTexture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D + 15, mFinalImage->mTexture, 0);
	if (blend) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	} else {
		// Render to temp texture
		glBindTexture(GL_TEXTURE_2D + 15, mPostProccessTexture->mTexture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D + 15, mPostProccessTexture->mTexture, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	shader->setTexture("final", mFinalImage);
	shader->setTexture("albedo", mGBuffer["albedo"]);
	shader->setTexture("normal", mGBuffer["normal"]);
	shader->setTexture("tangent", mGBuffer["tangent"]);
	shader->setTexture("depth", mDepth);
	if (mesh) {
		mesh->setMaterial(shader);
		mesh->draw(this, instanceData, count, size);
	}
	else {
		Mesh::quad()->setMaterial(shader);
		Mesh::quad()->draw();
	}

	if (blend) {
		glDisable(GL_BLEND);
	} else {
		// Render to main image
		glBindTexture(GL_TEXTURE_2D + 15, mFinalImage->mTexture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D + 15, mFinalImage->mTexture, 0);
		glFrontFace(GL_CCW);
		Shader::simple()->use();
		Shader::simple()->setTexture("screen", mPostProccessTexture);
		Mesh::quad()->draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Camera::setFov(const f32& _fov)
{
	mFov = _fov;
}

f32 Camera::getFov() const
{
	return mFov;
}

void Camera::setOrthoSize(const f32& _size)
{
	mOrthoSize = _size;
}

f32 Camera::getOrthoSize() const
{
	return mOrthoSize;
}

void Camera::setNear(const f32& _near)
{
	mNear = _near;
}

f32 Camera::getNear() const
{
	return mNear;
}

void Camera::setFar(const f32& _far)
{
	mFar = _far;
}

f32 Camera::getFar() const
{
	return mFar;
}

void Camera::setCullMode(const Mesh::CullMode& _cullmode)
{
	mCullMode = _cullmode;
}

Mesh::CullMode Camera::getCullMode() const
{
	return mCullMode;
}

void Camera::reCompute()
{
	mView = mathfu::mat4::LookAt(getGlobalPosition() + getForward(), getGlobalPosition(), getUp(), -1.0).Transpose();
	if (mProjectionType == ProjectionType::Perspective) {
		mProjection = mathfu::mat4::Perspective(mFov * mathfu::kDegreesToRadians, mAspectRatio, mNear, mFar, -1.0f).Transpose();
		mViewProjection = (mView * mProjection);
	}
	else {
		mProjection = mathfu::mat4::Ortho(-mOrthoSize/2, mOrthoSize / 2, -mOrthoSize / 2, mOrthoSize / 2, mNear, mFar, -1.0f).Transpose();
		mViewProjection = (mView * mProjection);
	}
}

std::string Camera::getClass() const
{
	return "Camera";
}

bool Camera::getDepthOnly() const
{
	return mDepthOnly;
}

Texture* Camera::getGbuffer(std::string _name) const
{
	if (mGBuffer.find(_name) == mGBuffer.end()) {
		ABORT(std::string("GBuffer \'") + _name + "\' Not found");
	}
	return mGBuffer.at(_name);
}

Texture* Camera::getDepth() const
{
	return mDepth;
}

void Camera::draw() const
{
	Shader::simple()->use();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	 if (renderType == "depth" || mDepthOnly) {
		 Shader::simple()->setInt("depth", 1);
		 Shader::simple()->setTexture("screen", mDepth);
	 } else if (mGBuffer.find(renderType) == mGBuffer.end()) {
		Shader::simple()->setTexture("screen", mFinalImage);
     } else {
		Shader::simple()->setTexture("screen", getGbuffer(renderType));
	 }
	Mesh::quad()->draw();
	Shader::simple()->setInt("depth", 0);
	glEnable(GL_DEPTH_TEST);
}

void Camera::addPostProccessShader(std::string _fragmentPath)
{
	mPostProccessShaders[_fragmentPath] = new Shader("assets/shaders/simple_vertex.vert", _fragmentPath.c_str());
}
