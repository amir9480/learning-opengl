#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "helpers.h"
#include "Texture.h"
#include "Node.h"
#include "Mesh.h"
#include "Shader.h"

struct InstanceData;
class Mesh;

class Camera : public Node
{
public:
	enum ProjectionType {
		Perspective,
		Ortho,
	};
	Camera(u32 width, u32 height, ProjectionType projectionType, f32 aspectRatio = 0.0f, bool _depthOnly = false);
	virtual ~Camera();

	template<typename T>
	void render(void(T::*callback)(Camera*), T* object);

	void applyPostProccesses();

	mathfu::mat4 getView() const;

	mathfu::mat4 getProjection() const;

	mathfu::mat4 getViewProjection() const;

	void postProccess(Shader* shader, bool blend = false, Mesh* mesh = nullptr, InstanceData* instanceData = nullptr, u32 count = 0, u32 size = 0);

	void setFov(const f32& _fov);
	f32 getFov() const;

	void setOrthoSize(const f32& _size);
	f32 getOrthoSize() const;

	void setNear(const f32& _near);
	f32 getNear() const;

	void setFar(const f32& _far);
	f32 getFar() const;

	void setCullMode(const Mesh::CullMode& _cullmode);
	Mesh::CullMode getCullMode() const;

	Texture* getGbuffer(std::string _name) const;
	Texture* getDepth() const;

	void draw() const;

	void addPostProccessShader(std::string _fragmentPath);

	// Inherited via Node
	virtual std::string getClass() const;

	bool getDepthOnly() const;
	
	std::string renderType = "final";
private:
	u32								mWidth;
	u32								mHeight;
	f32								mAspectRatio;
	u32								mFrameBuffer = 0;
	ProjectionType					mProjectionType;
	std::map<std::string, Texture*> mGBuffer;
	Texture*						mDepth = 0;
	u32								mPostProccessFrameBuffer = 0;
	Texture*					    mPostProccessTexture = nullptr;
	Texture*					    mFinalImage = nullptr;
	std::map<std::string, Shader*>	mPostProccessShaders;
	mathfu::mat4					mView;
	mathfu::mat4					mProjection;
	mathfu::mat4					mViewProjection;
	f32								mFov = 60.0;
	f32								mOrthoSize = 100.0f;
	bool							mDepthOnly = false;
	f32								mNear = 0.01f;
	f32								mFar = 1000.0f;
	Mesh::CullMode					mCullMode = Mesh::CullMode::None;

	void reCompute();
};


template<typename T>
void Camera::render(void(T::*callback)(Camera*), T* object)
{
	reCompute();
	int currentDepthStatus = 0;
	int currentFrameBuffer;
	int currentViewPort[4];
	glGetIntegerv(GL_DEPTH_TEST, &currentDepthStatus);
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFrameBuffer);
	glGetIntegerv(GL_VIEWPORT, currentViewPort);

	glViewport(0, 0, mWidth, mHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
	if (!mDepthOnly) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGBuffer["albedo"]->mTexture, 0);
	}
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	(object->*callback)(this);
	glDisable(GL_DEPTH_TEST);
	if (!mDepthOnly) {
		glBindFramebuffer(GL_FRAMEBUFFER, mPostProccessFrameBuffer);
		glBindTexture(GL_TEXTURE_2D, mPostProccessTexture->mTexture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPostProccessTexture->mTexture, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, mFinalImage->mTexture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFinalImage->mTexture, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, currentFrameBuffer);
	glViewport(currentViewPort[0], currentViewPort[1], currentViewPort[2], currentViewPort[3]);
	if (currentDepthStatus) {
		glEnable(GL_DEPTH_TEST);
	}
	
}

#endif// _CAMERA_H_