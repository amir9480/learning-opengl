#ifndef _MESH_H_
#define _MESH_H_
#include "helpers.h"
#include "Node.h"

struct Vertex
{
	f32 px, py, pz;
	f32 nx, ny, nz;
	f32 u, v;

	Vertex(f32 _px, f32 _py, f32 _pz,
		f32 _u = 0.0f, f32 _v = 0.0f,
		f32 _nx = 0.0f, f32 _ny = 0.0f, f32 _nz = 0.0f) :
		px(_px), py(_py), pz(_pz),
		nx(_nx), ny(_ny), nz(_nz),
		u(_u), v(_v) {}
};

class Mesh : public Node
{
public:
	Mesh();
	Mesh(std::vector<Vertex> _vertices, std::vector<u32> _indices);
	Mesh(const Vertex* _vertices, const u32& _verticesCount, const u32* _indices, const u32& _indicesCount);
	virtual ~Mesh();
	
	void draw();

	void destroy();

	void updateMesh(const std::vector<Vertex>& _vertices, const std::vector<u32>& _indices);

	void updateMesh(const Vertex* _vertices, const u32& _verticesCount, const u32* _indices, const u32& _indicesCount);


	static Mesh* quad();
private:
	u32 mVAO;
	u32 mVBO;
	u32 mEBO;
	u32 mVerticesCount;
	u32 mIndicesCount;
};

#endif // _MESH_H_