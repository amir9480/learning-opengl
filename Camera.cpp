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
	static u32 attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, attachments);


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
	if (mProjectionType == ProjectionType::Perspective) {
		mProjection = mathfu::mat4::Perspective(mFov * mathfu::kDegreesToRadians, mAspectRatio, 0.01f, 1000.0f, -1.0f).Transpose();
		mView = mathfu::mat4::LookAt(mPosition + getForward(), mPosition, getUp()).Transpose();
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Shader::simple()->setTexture("screen", mGBuffer.at("albedo"), 0);
	Shader::simple()->setTexture("normal", mGBuffer.at("normal"), 1);
	Mesh::quad()->draw();
}

void Camera::addPostProccessShader(std::string _fragmentPath)
{
	mPostProccessShaders[_fragmentPath] = new Shader("assets/shaders/simple_vertex.vert", _fragmentPath.c_str());
}
