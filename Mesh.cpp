#include "Mesh.h"

Mesh::Mesh():
	Node(),
	mVerticesCount(0),
	mIndicesCount(0),
	mVAO(0),
	mVBO(0),
	mEBO(0),
	mCullMode(Mesh::CullMode::Back)
{}

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

Mesh* Mesh::createCube()
{
	return new Mesh({
		// back face
		Vertex(-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f), // bottom-left
		Vertex(1.0f,   1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f), // top-right
		Vertex(1.0f,  -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f), // bottom-right   
		Vertex(-1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f), // top-left
		// front face
		Vertex(-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f), // bottom-left
		Vertex(+1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f), // bottom-right
		Vertex(+1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f), // top-right
		Vertex(-1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f), // top-left
		// left face		
		Vertex(-1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f), // top-right
		Vertex(-1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f), // top-left
		Vertex(-1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f), // bottom-left
		Vertex(-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f), // bottom-right
		// right face
		Vertex(+1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f), // top-left
		Vertex(+1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f), // bottom-right
		Vertex(+1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f), // top-right     
		Vertex(+1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f), // bottom-left     
		// bottom face
		Vertex(-1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f), // top-right
		Vertex(+1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f), // top-left
		Vertex(+1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f), // bottom-left
		Vertex(-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f), // bottom-right
		// top face
		Vertex(-1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f), // top-left
		Vertex(+1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f), // bottom-right
		Vertex(+1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f), // top-right    
		Vertex(-1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f)  // bottom-left    
		}, { 1, 0, 2, 0, 1, 3, 5, 4, 6, 6, 4, 7, 9, 8, 10, 10, 8, 11, 13, 12, 14, 12, 13, 15, 17, 16, 18, 18, 16, 19, 21, 20, 22, 20, 21, 23});
}

Mesh* Mesh::createPlane(u32 _uvRepeat)
{
	return new Mesh({
		Vertex(-1.0f, 0.0f,  1.0f, 0.0f * _uvRepeat, 1.0f * _uvRepeat, 0.0f, 1.0f, 0.0),
		Vertex(-1.0f, 0.0f, -1.0f, 0.0f * _uvRepeat, 0.0f * _uvRepeat, 0.0f, 1.0f, 0.0),
		Vertex( 1.0f, 0.0f,  1.0f, 1.0f * _uvRepeat, 1.0f * _uvRepeat, 0.0f, 1.0f, 0.0),
		Vertex( 1.0f, 0.0f, -1.0f, 1.0f * _uvRepeat, 0.0f * _uvRepeat, 0.0f, 1.0f, 0.0)
		}, {
			0, 1, 2,
			2, 1, 3
		}
	);
}

Mesh* Mesh::createSphere(u32 rows, u32 cols)
{
	std::vector<Vertex> vertices;
	std::vector<u32> indicies;

	//vertices.resize(rows * cols);
	//indicies.resize(rows * cols * 3);

	f32 rowStep = 1.0 / (rows - 1);
	f32 colStep = 1.0 / (cols - 1);
	f32 piOn2 = mathfu::kPi * 0.5f;
	f32 piM2 = mathfu::kPi * 2.0f;
	u32 currentRow = 0;
	u32 nextRow = 0;
	mathfu::vec3 tempVec;
	mathfu::vec3 tempNorm;

	for (u32 r = 0; r < rows; r++) {
		for (u32 c = 0; c < cols; c++) {

			tempVec = mathfu::vec3(
				cos(piM2 * c * colStep)* sin(mathfu::kPi * r * rowStep),
				sin(-piOn2 + mathfu::kPi * r * rowStep),
				sin(piM2 * c * colStep)* sin(mathfu::kPi * r * rowStep)
			);
			tempNorm = - tempVec.Normalized();
			vertices.push_back(Vertex(
				tempVec.x, tempVec.y, tempVec.z,
				r * rowStep,
				c * colStep,
				tempNorm.x, tempNorm.y, tempNorm.z
			));
			
			currentRow = r * cols;
			nextRow = (r+1) * cols;

			if (r < rows - 1 && c < cols - 1) {
				indicies.push_back(nextRow + c);
				indicies.push_back(currentRow + c);
				indicies.push_back(nextRow + c + 1);

				indicies.push_back(nextRow + c + 1);
				indicies.push_back(currentRow + c);
				indicies.push_back(currentRow + c + 1);
			}
		}
	}
	return new Mesh(vertices, indicies);
}

void Mesh::draw(Camera* camera)
{
	if (mMaterial && camera) {
		mMaterial->use();
		mMaterial->setMatrix("MVP", this->getTransformMatrix() * camera->getViewProjection());
		mMaterial->setMatrix("worldMatrix", this->getTransformMatrix());
		mMaterial->setMatrix("viewMatrix", camera->getView());
		mMaterial->setMatrix("projectionMatrix", camera->getProjection());
		mMaterial->setTexture("diffuse", mDiffuse, 0);
	}

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	if (mCullMode == CullMode::None) {
		glDisable(GL_CULL_FACE);
	} else {
		glEnable(GL_CULL_FACE);
		glCullFace(mCullMode == CullMode::Front ? GL_FRONT : GL_BACK);
	}
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

void Mesh::setCullMode(CullMode cullmode)
{
	mCullMode = cullmode;
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
		2, 1, 3
	});
	return out;
}

void Mesh::render(Camera* camera)
{
	this->draw(camera);
	for (auto& child : mChildren) {
		child->render(camera);
	}
}

Mesh* Mesh::setMaterial(Shader* material)
{
	mMaterial = material;
	return this;
}

Mesh* Mesh::setDiffuse(Texture* texture)
{
	mDiffuse = texture;
	return this;
}

std::string Mesh::getClass() const
{
	return "Mesh";
}
