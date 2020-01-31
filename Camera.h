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
	Camera(u32 width, u32 height, ProjectionType projectionType, f32 aspectRatio = 0.0f);
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

	Texture* getGbuffer(std::string _name) const;
	Texture* getDepth() const;

	void draw() const;

	void addPostProccessShader(std::string _fragmentPath);

	// Inherited via Node
	virtual std::string getClass() const;
	
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

	void reCompute();
};


template<typename T>
void Camera::render(void(T::*callback)(Camera*), T* object)
{
	reCompute();
	glViewport(0, 0, mWidth, mHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGBuffer["albedo"]->mTexture, 0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	(object->*callback)(this);
	glDisable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, mPostProccessFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, mPostProccessTexture->mTexture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPostProccessTexture->mTexture, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, mFinalImage->mTexture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFinalImage->mTexture, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	Shader::simple()->use();
	//Shader::simple()->setTexture("screen", mGBuffer["albedo"]);
	//Mesh::quad()->draw();

	/*for (auto postProccessShader : mPostProccessShaders) {
		this->postProccess(postProccessShader.second);
	}*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#endif// _CAMERA_H_