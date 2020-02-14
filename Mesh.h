#ifndef _MESH_H_
#define _MESH_H_
#include "helpers.h"
#include "Node.h"
#include "Shader.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex
{
	f32 px, py, pz;
	f32 nx, ny, nz;
	f32 tx, ty, tz;
	f32 u, v;
	u8 bondId[4] = { 0 };
	f32 boneWeights[4] = { 0 };


	Vertex(f32 _px, f32 _py, f32 _pz,
		f32 _u, f32 _v,
		f32 _nx = 0.0f, f32 _ny = 0.0f, f32 _nz = 0.0f) :
		px(_px), py(_py), pz(_pz),
		nx(_nx), ny(_ny), nz(_nz),
		u(_u), v(_v),
		tx(0), ty(0), tz(0)
	{}

	Vertex(f32 _px, f32 _py, f32 _pz,
		f32 _u, f32 _v,
		f32 _nx, f32 _ny, f32 _nz,
		f32 _tx, f32 _ty, f32 _tz) : 
		px(_px), py(_py), pz(_pz),
		nx(_nx), ny(_ny), nz(_nz),
		u(_u), v(_v),
		tx(_tx), ty(_ty), tz(_tz)
	{}

	Vertex(const Vertex& _other) { *this = _other; }

	Vertex& operator = (const Vertex& _other) {
		px = _other.px; py = _other.py; pz = _other.pz;
		nx = _other.nx; ny = _other.ny; nz = _other.nz;
		tx = _other.tx; ty = _other.ty; tz = _other.tz;
		u = _other.u; v = _other.v;
		bondId[0] = _other.bondId[0]; bondId[1] = _other.bondId[1]; bondId[2] = _other.bondId[2]; bondId[3] = _other.bondId[3];
		boneWeights[0] = _other.boneWeights[0]; boneWeights[1] = _other.boneWeights[1]; boneWeights[2] = _other.boneWeights[2]; boneWeights[3] = _other.boneWeights[3];
		return *this;
	}
};

struct InstanceData
{
	mathfu::mat4 world;

	InstanceData(mathfu::mat4 _world = mathfu::mat4::Identity()) :
		world(_world) {}
};

struct Bone
{
	Bone() {}
	Bone(const Bone& _other) { *this = _other; }
	Bone(Bone&& _other) noexcept { *this = _other; }

	Bone& operator = (const Bone& _other) {
		name = _other.name;
		trans = _other.trans;
		return *this;
	}

	Bone& operator = (Bone&& _other) noexcept {
		name = _other.name;
		trans = _other.trans;
		return *this;
	}

	mathfu::mat4 trans;
	std::string name;
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
	Mesh(std::vector<Vertex> _vertices, std::vector<u32> _indices, bool lightMesh = false);
	Mesh(const Vertex* _vertices, const u32& _verticesCount, const u32* _indices, const u32& _indicesCount, bool lightMesh = false);
	virtual ~Mesh();

	static Mesh* createCube();

	static Mesh* createPlane(u32 _uvRepeat = 1);

	static Mesh* createSphere(u32 rows = 12, u32 cols = 12, bool lightMesh = false);

	static Mesh* createCone(u32 cols = 12, bool lightMesh = false);

	static Mesh* createFromFile(std::string _path);

	void readNodeHierarchy(const f32& _animationTime, const aiNode* _node, const aiMatrix4x4& parentTransform);

	const aiNodeAnim* findNodeAnimation(const aiAnimation* _animation, const std::string& _name) const;

	void draw(Camera* camera = nullptr, InstanceData* instanceData = nullptr, u32 count = 0, u32 size = 0);

	void draw(Camera* camera, std::vector<InstanceData> instanceData);

	void destroy();

	void setCullMode(CullMode cullmode = CullMode::Back);

	void updateMesh(const std::vector<Vertex>& _vertices, const std::vector<u32>& _indices, bool lightMesh = false);

	void updateMesh(const Vertex* _vertices, const u32& _verticesCount, const u32* _indices, const u32& _indicesCount, bool lightMesh = false);

	static Mesh* quad();

	static Mesh* sphere();

	static Mesh* cube();

	virtual void render(Camera* camera = nullptr);

	Mesh* setMaterial(Shader* material);

	Mesh* setDiffuse(Texture* texture);

	Mesh* setNormal(Texture* texture);

	Mesh* setDisplacment(Texture* texture);

	std::string getHash() const;

	virtual std::string getClass() const;

	u32 getInstanceVBO() const;
private:
	u32								mVAO;
	u32								mVBO;
	u32								mInstanceVBO;
	u32								mEBO;
	u32								mVerticesCount;
	u32								mIndicesCount;
	CullMode						mCullMode;
	Shader*							mMaterial;
	Texture*						mDiffuse = nullptr;
	Texture*						mNormal = nullptr;
	Texture*						mDisplacement = nullptr;
	std::string						mHash;
	std::vector<Bone>				mBones;
	std::vector<mathfu::mat4>		mBonesData;
	aiScene*						mScene;
	aiAnimation*					mCurrentAnimation;
};

#endif // _MESH_H_