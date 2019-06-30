#include "Mesh.h"

Mesh::Mesh():
	Node(),
	mVerticesCount(0),
	mIndicesCount(0),
	mVAO(0),
	mVBO(0),
	mEBO(0) {}

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<u32> _indicies)
	:Mesh()
{
	this->updateMesh(_vertices.data(), _vertices.size(), _indicies.data(), _indicies.size());
}

Mesh::Mesh(const Vertex* _vertices, const u32& _verticesCount, const u32* _indices, const u32& _indicesCount)
	: Mesh()
{
	this->updateMesh(_vertices, _verticesCount, _indices, _indicesCount);
}

Mesh::~Mesh()
{
	this->destroy();
}

void Mesh::draw()
{
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::destroy()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);

	mVerticesCount = 0;
	mIndicesCount = 0;
	mVAO = 0;
	mVBO = 0;
	mEBO = 0;
}

void Mesh::updateMesh(const std::vector<Vertex>& _vertices, const std::vector<u32>& _indices)
{
	updateMesh(_vertices.data(), _vertices.size(), _indices.data(), _indices.size());
}

void Mesh::updateMesh(const Vertex* _vertices, const u32& _verticesCount, const u32* _indices, const u32& _indicesCount)
{
	if (this->mVerticesCount != 0) {
		this->destroy();
	}
	this->mVerticesCount = _verticesCount;
	this->mIndicesCount = _indicesCount;
	Vertex* vertices = (Vertex*)_vertices;

	mathfu::vec3 pos[3], norm[4], axis[2];
	for (u32 i = 0; i < _indicesCount / 3; i++) {
		for (u32 j = 0; j < 3; j++) {
			pos[j].x = vertices[_indices[i * 3 + j]].px;
			pos[j].y = vertices[_indices[i * 3 + j]].py;
			pos[j].z = vertices[_indices[i * 3 + j]].pz;
			norm[j].x = vertices[_indices[i * 3 + j]].nx;
			norm[j].y = vertices[_indices[i * 3 + j]].ny;
			norm[j].z = vertices[_indices[i * 3 + j]].nz;
		}

		axis[0] = pos[1] - pos[0];
		axis[1] = pos[1] - pos[2];
		norm[3] = mathfu::vec3::CrossProduct(axis[1], axis[0]);
		for (u32 j = 0; j < 3; j++) {
			norm[j] = (norm[j] + norm[3]).Normalized();

			vertices[_indices[i * 3 + j]].nx = norm[j].x;
			vertices[_indices[i * 3 + j]].ny = norm[j].y;
			vertices[_indices[i * 3 + j]].nz = norm[j].z;
		}
	}

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, _verticesCount * sizeof(Vertex), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indicesCount * sizeof(u32), (void*)_indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh* Mesh::quad()
{
	static Mesh* out = new Mesh({
		Vertex(-1.0f,  1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
		Vertex( 1.0f,  1.0f, 0.0f, 1.0f, 1.0f),
		Vertex( 1.0f, -1.0f, 0.0f, 1.0f, 0.0f)
	}, {
		0, 1, 2,
		1, 2, 3
	});
	return out;
}

std::string Mesh::getClass() const
{
	return "Mesh";
}
