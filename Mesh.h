#ifndef _MESH_H_
#define _MESH_H_
#include "helpers.h"
#include "Node.h"
#include "Shader.h"
#include "Camera.h"

struct Vertex
{
	f32 px, py, pz;
	f32 nx, ny, nz;
	f32 u, v;

	Vertex(f32 _px = 0.0f, f32 _py = 0.0f, f32 _pz = 0.0f,
		f32 _u = 0.0f, f32 _v = 0.0f,
		f32 _nx = 0.0f, f32 _ny = 0.0f, f32 _nz = 0.0f) :
		px(_px), py(_py), pz(_pz),
		nx(_nx), ny(_ny), nz(_nz),
		u(_u), v(_v) {}
};

struct InstanceData
{
	mathfu::mat4 world;

	InstanceData(mathfu::mat4 _world = mathfu::mat4::Identity()) :
		world(_world) {}
};

class Camera;

class Mesh : public Node
{
public:
	enum CullMode
	{
		Back,
		Front,
		None
	};
public:
	Mesh();
	Mesh(std::vector<Vertex> _vertices, std::vector<u32> _indices);
	Mesh(const Vertex* _vertices, const u32& _verticesCount, const u32* _indices, const u32& _indicesCount);
	virtual ~Mesh();

	static Mesh* createCube();

	static Mesh* createPlane(u32 _uvRepeat = 1);

	static Mesh* createSphere(u32 rows = 12, u32 cols = 12);
	
	void draw(Camera* camera = nullptr, InstanceData* instanceData = nullptr, u32 count = 0);

	void draw(Camera* camera, std::vector<InstanceData> instanceData);

	void destroy();

	void setCullMode(CullMode cullmode = CullMode::Back);

	void updateMesh(const std::vector<Vertex>& _vertices, const std::vector<u32>& _indices);

	void updateMesh(const Vertex* _vertices, const u32& _verticesCount, const u32* _indices, const u32& _indicesCount);

	static Mesh* quad();

	static Mesh* sphere();

	static Mesh* cube();

	virtual void render(Camera* camera = nullptr);

	Mesh* setMaterial(Shader* material);

	Mesh* setDiffuse(Texture* texture);

	virtual std::string getClass() const;
private:
	u32		 mVAO;
	u32		 mVBO;
	u32		 mInstanceVBO;
	u32		 mEBO;
	u32		 mVerticesCount;
	u32		 mIndicesCount;
	CullMode mCullMode;
	Shader*  mMaterial;
	Texture* mDiffuse = nullptr;
};

#endif // _MESH_H_