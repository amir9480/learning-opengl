#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "helpers.h"
#include "Texture.h"
#include "Node.h"
#include "Mesh.h"
#include "Shader.h"

class Camera : public Node
{
public:
	enum ProjectionType {
		Perspective,
		Ortho,
	};
	Camera(u32 width, u32 height, ProjectionType projectionType);
	virtual ~Camera();

	void render(void(*callback)(Camera*));

	mathfu::mat4 getView() const;

	mathfu::mat4 getProjection() const;

	mathfu::mat4 getViewProjection() const;

	void setFov(const f32& _fov);

	f32 getFov() const;

	Texture* getGbuffer(std::string _name) const;
	Texture* getDepth() const;

	void draw() const;
	
private:
	u32								mWidth;
	u32								mHeight;
	u32								mFrameBuffer = 0;
	ProjectionType					mProjectionType;
	std::map<std::string, Texture*> mGBuffer;
	Texture*						mDepth = 0;
	mathfu::mat4					mView;
	mathfu::mat4					mProjection;
	mathfu::mat4					mViewProjection;
	f32								mFov = 60.0;

	void reCompute();
};

#endif// _CAMERA_H_