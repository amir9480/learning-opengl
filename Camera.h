#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "helpers.h"
#include "Texture.h"
#include "Node.h"
#include "Mesh.h"
#include "Shader.h"

class Mesh;

class Camera : public Node
{
public:
	enum ProjectionType {
		Perspective,
		Ortho,
	};
	Camera(u32 width, u32 height, ProjectionType projectionType, f32 aspectRatio = 0.0f);
	virtual ~Camera();

	template<typename T>
	void render(void(T::*callback)(Camera*), T* object);

	mathfu::mat4 getView() const;

	mathfu::mat4 getProjection() const;

	mathfu::mat4 getViewProjection() const;

	void postProccess(Shader* shader, bool blend = false, Mesh* mesh = nullptr);

	void setFov(const f32& _fov);

	f32 getFov() const;

	Texture* getGbuffer(std::string _name) const;
	Texture* getDepth() const;

	void draw() const;

	void addPostProccessShader(std::string _fragmentPath);

	
	
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

	void reCompute();

	// Inherited via Node
	virtual std::string getClass() const;
};


template<typename T>
void Camera::render(void(T::*callback)(Camera*), T* object)
{
	reCompute();
	glViewport(0, 0, mWidth, mHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGBuffer["albedo"]->mTexture, 0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	(object->*callback)(this);
	glDisable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, mPostProccessFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, mFinalImage->mTexture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFinalImage->mTexture, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	//Shader::simple()->use();
	//Mesh::quad()->draw();

	for (auto postProccessShader : mPostProccessShaders) {
		this->postProccess(postProccessShader.second);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}

#endif// _CAMERA_H_