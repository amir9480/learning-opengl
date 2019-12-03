#include "Camera.h"

Camera::Camera(u32 width, u32 height, ProjectionType projectionType, f32 aspectRatio)
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

	std::vector<std::string> gBuffers = { "albedo", "normal" };
	for (u32 index = 0; index < gBuffers.size(); index++) {
		mGBuffer[gBuffers[index]] = new Texture(mWidth, mHeight);
		glBindTexture(GL_TEXTURE_2D, mGBuffer[gBuffers[index]]->mTexture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, mGBuffer[gBuffers[index]]->mTexture, 0);
	}
	static u32 attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
	glDrawBuffers(4, attachments);


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

	glBindFramebuffer(GL_FRAMEBUFFER, mPostProccessFrameBuffer);
	if (blend) {
		glBindTexture(GL_TEXTURE_2D, mFinalImage->mTexture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFinalImage->mTexture, 0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	} else {
		// Render to temp texture
		glBindTexture(GL_TEXTURE_2D, mPostProccessTexture->mTexture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPostProccessTexture->mTexture, 0);
	}
	shader->setTexture("final", mFinalImage);
	shader->setTexture("albedo", mGBuffer["albedo"]);
	shader->setTexture("normal", mGBuffer["normal"]);
	shader->setTexture("depth", mDepth);
	if (mesh) {
		mesh->setMaterial(shader);
		mesh->draw(this, instanceData, count, size);
	} else {
		Mesh::quad()->draw();
	}

	if (blend) {
		glDisable(GL_BLEND);
	} else {
		// Render to main image
		glBindTexture(GL_TEXTURE_2D, mFinalImage->mTexture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFinalImage->mTexture, 0);
		glFrontFace(GL_CCW);
		Shader::simple()->use();
		Shader::simple()->setTexture("screen", mPostProccessTexture);
		Mesh::quad()->draw();
	}
}

void Camera::setFov(const f32& _fov)
{
	mFov = _fov;
}

f32 Camera::getFov() const
{
	return mFov;
}

void Camera::reCompute()
{
	mView = mathfu::mat4::LookAt(getGlobalPosition() + getForward(), getGlobalPosition(), getUp(), -1.0).Transpose();
	if (mProjectionType == ProjectionType::Perspective) {
		mProjection = mathfu::mat4::Perspective(mFov * mathfu::kDegreesToRadians, mAspectRatio, 0.1f, 1000.0f, -1.0f).Transpose();
		mViewProjection = (mView * mProjection);
	}
	else {
		mProjection = mathfu::mat4::Ortho(-1, 1, -1, 1, 0.01f, 1000.0f, -1.0f);
		mViewProjection = (mView * mProjection);
	}
}

std::string Camera::getClass() const
{
	return "Camera";
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
	// Shader::simple()->setTexture("screen", getGbuffer("albedo"), 0);
	Shader::simple()->setTexture("screen", mFinalImage);
	Mesh::quad()->draw();
}

void Camera::addPostProccessShader(std::string _fragmentPath)
{
	mPostProccessShaders[_fragmentPath] = new Shader("assets/shaders/simple_vertex.vert", _fragmentPath.c_str());
}
