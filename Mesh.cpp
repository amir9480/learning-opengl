#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "3rdparty/picosha2/picosha2.h"

#include "Mesh.h"
#include "Light.h"

Mesh::Mesh():
	Node(),
	mVerticesCount(0),
	mIndicesCount(0),
	mVAO(0),
	mVBO(0),
	mInstanceVBO(0),
	mEBO(0),
	mCullMode(Mesh::CullMode::Back)
{
	mMaterial = Shader::defaultMaterial();
	mDiffuse = Texture::defaultTexture();
}

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<u32> _indicies, bool lightMesh)
	:Mesh()
{
	this->updateMesh(_vertices.data(), _vertices.size(), _indicies.data(), _indicies.size(), lightMesh);
}

Mesh::Mesh(const Vertex* _vertices, const u32& _verticesCount, const u32* _indices, const u32& _indicesCount, bool lightMesh)
	: Mesh()
{
	this->updateMesh(_vertices, _verticesCount, _indices, _indicesCount, lightMesh);
}

Mesh::~Mesh()
{
	this->destroy();
}

Mesh* Mesh::createCube()
{
	Mesh* mesh = new Mesh({
		// back face
		Vertex(-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f), // bottom-left
		Vertex(1.0f,   1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.0f, -1.0f), // top-right
		Vertex(1.0f,  -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,  0.0f, -1.0f), // bottom-right
		Vertex(-1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f), // top-left
		// front face
		Vertex(-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f), // bottom-left
		Vertex(+1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f), // bottom-right
		Vertex(+1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f), // top-right
		Vertex(-1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f), // top-left
		// left face
		Vertex(-1.0f,  1.0f,  1.0f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f), // top-right
		Vertex(-1.0f,  1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f), // top-left
		Vertex(-1.0f, -1.0f, -1.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f), // bottom-left
		Vertex(-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f), // bottom-right
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
	mesh->mHash = "CUBE";
	return mesh;
}

Mesh* Mesh::createPlane(u32 _uvRepeat)
{
	Mesh* mesh = new Mesh({
		Vertex(-1.0f, 0.0f,  1.0f, 0.0f * _uvRepeat, 1.0f * _uvRepeat, 0.0f, 1.0f, 0.0),
		Vertex(-1.0f, 0.0f, -1.0f, 0.0f * _uvRepeat, 0.0f * _uvRepeat, 0.0f, 1.0f, 0.0),
		Vertex( 1.0f, 0.0f,  1.0f, 1.0f * _uvRepeat, 1.0f * _uvRepeat, 0.0f, 1.0f, 0.0),
		Vertex( 1.0f, 0.0f, -1.0f, 1.0f * _uvRepeat, 0.0f * _uvRepeat, 0.0f, 1.0f, 0.0)
		}, {
			0, 1, 2,
			2, 1, 3
		}
	);
	mesh->mHash = "PLANE";
	return mesh;
}

Mesh* Mesh::createSphere(u32 rows, u32 cols, bool lightMesh)
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
			tempNorm = tempVec.Normalized();
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
	Mesh* mesh = new Mesh(vertices, indicies, lightMesh);
	mesh->mHash = "SPHERE";
	return mesh;
}

Mesh* Mesh::createCone(u32 cols, bool lightMesh)
{
	std::vector<Vertex> vertices;
	std::vector<u32> indicies;

	//vertices.resize(rows * cols);
	//indicies.resize(rows * cols * 3);

	f32 colStep = 1.0 / (cols - 1);
	f32 piOn2 = mathfu::kPi * 0.5f;
	f32 piM2 = mathfu::kPi * 2.0f;
	u32 currentRow = 0;
	u32 nextRow = 0;
	mathfu::vec3 tempVec;
	mathfu::vec3 tempNorm;

	vertices.push_back(Vertex(0, 0, 1, 0, 0, 0, -1, 0));
	vertices.push_back(Vertex(0, 0, 0, 0, 0, 0, 1, 0));

	for (u32 i = 0; i < cols; i++) {
		vertices.push_back(Vertex(cos(piM2 * i * colStep), sin(piM2 * i * colStep), 1, cos(mathfu::kPi * i * colStep), sin(mathfu::kPi * i * colStep), 0, -1, 0));
		if (i < cols - 1) {
			indicies.push_back(0);
			indicies.push_back(i + 3);
			indicies.push_back(i + 2);

			indicies.push_back(1);
			indicies.push_back(i + 2);
			indicies.push_back(i + 3);
		}
	}

	Mesh* mesh = new Mesh(vertices, indicies, lightMesh);
	mesh->mHash = "CONE";
	return mesh;
}

Mesh* Mesh::createFromFile(std::string _path)
{
	aiScene* scene = (aiScene*)aiImportFile(_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == nullptr) {
		ABORT("Mesh File does not exists.");
	}
	std::vector<Vertex> vertices;
	std::vector<u32> indicies;
	if (scene->mNumMeshes > 0) {
		for (u32 i = 0; i < scene->mMeshes[0]->mNumVertices; i++) {
			vertices.push_back(Vertex(
				scene->mMeshes[0]->mVertices[i].x, scene->mMeshes[0]->mVertices[i].y, scene->mMeshes[0]->mVertices[i].z,
				scene->mMeshes[0]->mTextureCoords[0][i].x, scene->mMeshes[0]->mTextureCoords[0][i].y,
				scene->mMeshes[0]->mNormals[i].x, scene->mMeshes[0]->mNormals[i].y, scene->mMeshes[0]->mNormals[i].z
			));
		}
		for (u32 i = 0; i < scene->mMeshes[0]->mNumFaces; i++) {
			for (int j = scene->mMeshes[0]->mFaces[i].mNumIndices - 1; j >= 0 ; j--) {
				indicies.push_back(scene->mMeshes[0]->mFaces[i].mIndices[j]);
			}
		}
	}
	Mesh* mesh = new Mesh(vertices, indicies);
	mesh->mHash = picosha2::hash256_hex_string(_path);
	return mesh;
}

void Mesh::draw(Camera* camera, InstanceData* instanceData, u32 count, u32 size)
{
	if (mMaterial && camera) {
		mMaterial->use();
		mMaterial->setBool("instancing", instanceData != nullptr);
		mMaterial->setMatrix("MVP", this->getTransformMatrix() * camera->getViewProjection());
		mMaterial->setMatrix("worldMatrix", this->getTransformMatrix());
		mMaterial->setMatrix("viewMatrix", camera->getView());
		mMaterial->setMatrix("projectionMatrix", camera->getProjection());
		mMaterial->setMatrix("viewProjectionMatrix", camera->getViewProjection());
		mMaterial->setFloat3("cameraPosition", camera->getPosition());
		mMaterial->setTexture("diffuse", mDiffuse);
		mMaterial->setBool("hasNormal", mNormal != nullptr);
		mMaterial->setTexture("normalTexture", mNormal);
		mMaterial->setBool("hasDisplacement", mDisplacement != nullptr);
		mMaterial->setTexture("displacement", mDisplacement);
	}

	if (size == 0) {
		size = sizeof(InstanceData);
	}

	if (instanceData) {
		glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);
		glBufferData(GL_ARRAY_BUFFER, size * count, (void*)instanceData, GL_DYNAMIC_DRAW);
	}

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	if (mCullMode == CullMode::None) {
		glDisable(GL_CULL_FACE);
	} else {
		glEnable(GL_CULL_FACE);
		glCullFace(mCullMode == CullMode::Front ? GL_FRONT : GL_BACK);
	}
	if (instanceData) {
		glDrawElementsInstanced(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_INT, 0, count);
	} else {
		glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw(Camera* camera, std::vector<InstanceData> instanceData)
{
	draw(camera, (InstanceData*)instanceData.data(), instanceData.size());
}

void Mesh::destroy()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mInstanceVBO);
	glDeleteBuffers(1, &mEBO);

	mVerticesCount = 0;
	mIndicesCount = 0;
	mVAO = 0;
	mVBO = 0;
	mInstanceVBO = 0;
	mEBO = 0;
}

void Mesh::setCullMode(CullMode cullmode)
{
	mCullMode = cullmode;
}

void Mesh::updateMesh(const std::vector<Vertex>& _vertices, const std::vector<u32>& _indices, bool lightMesh)
{
	updateMesh(_vertices.data(), _vertices.size(), _indices.data(), _indices.size(), lightMesh);
}

void Mesh::updateMesh(const Vertex* _vertices, const u32& _verticesCount, const u32* _indices, const u32& _indicesCount, bool lightMesh)
{
	if (this->mVerticesCount != 0) {
		this->destroy();
	}
	this->mVerticesCount = _verticesCount;
	this->mIndicesCount = _indicesCount;
	Vertex* vertices = (Vertex*)_vertices;

	float r;
	mathfu::vec3 pos[3], uv[3], norm[4], tangents[4], axis[2], uvaxis[2];
	for (u32 i = 0; i < _indicesCount / 3; i++) {
		for (u32 j = 0; j < 3; j++) {
			pos[j].x = vertices[_indices[i * 3 + j]].px;
			pos[j].y = vertices[_indices[i * 3 + j]].py;
			pos[j].z = vertices[_indices[i * 3 + j]].pz;
			norm[j].x = vertices[_indices[i * 3 + j]].nx;
			norm[j].y = vertices[_indices[i * 3 + j]].ny;
			norm[j].z = vertices[_indices[i * 3 + j]].nz;
			tangents[j].x = vertices[_indices[i * 3 + j]].tx;
			tangents[j].y = vertices[_indices[i * 3 + j]].ty;
			tangents[j].z = vertices[_indices[i * 3 + j]].tz;
			uv[j].x = vertices[_indices[i * 3 + j]].u;
			uv[j].y = vertices[_indices[i * 3 + j]].v;
		}

		axis[0] = pos[0] - pos[1];
		axis[1] = pos[2] - pos[0];

		uvaxis[0] = uv[0] - uv[1];
		uvaxis[1] = uv[2] - uv[0];

		r = 1.0f / (uvaxis[0].x * uvaxis[1].y - uvaxis[0].y * uvaxis[1].x);

		norm[3] = mathfu::vec3::CrossProduct(axis[0], axis[1]).Normalized();
		tangents[3] = ((axis[0] * uvaxis[1].y - axis[1] * uvaxis[0].y) * r).Normalized();

		for (u32 j = 0; j < 3; j++) {
			norm[j] = (norm[j] + norm[3]).Normalized();
			tangents[j] = (tangents[j] + tangents[3]).Normalized();

			vertices[_indices[i * 3 + j]].nx += norm[j].x;
			vertices[_indices[i * 3 + j]].ny += norm[j].y;
			vertices[_indices[i * 3 + j]].nz += norm[j].z;

			vertices[_indices[i * 3 + j]].tx += tangents[j].x;
			vertices[_indices[i * 3 + j]].ty += tangents[j].y;
			vertices[_indices[i * 3 + j]].tz += tangents[j].z;
		}
	}
	mathfu::vec3 realNormal, realTan;
	for (u32 i = 0; i < mVerticesCount; i++) {
		realNormal = mathfu::vec3(vertices[i].nx, vertices[i].ny, vertices[i].nz).Normalized();
		realTan = mathfu::vec3(vertices[i].tx, vertices[i].ty, vertices[i].tz).Normalized();

		vertices[i].nx = realNormal.x;
		vertices[i].ny = realNormal.y;
		vertices[i].nz = realNormal.z;

		vertices[i].tx = realTan.x;
		vertices[i].ty = realTan.y;
		vertices[i].tz = realTan.z;
	}

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mInstanceVBO);
	glGenBuffers(1, &mEBO);
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, _verticesCount * sizeof(Vertex), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indicesCount * sizeof(u32), (void*)_indices, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);
	if (lightMesh) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(LightInstanceData), (void*)0, GL_DYNAMIC_DRAW);
		for (u32 i = 0; i < 4; i++) {
			glEnableVertexAttribArray(4 + i);
			glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(LightInstanceData), (void*)(4 * i * sizeof(float)));
			glVertexAttribDivisor(4 + i, 1);
		}
		for (u32 i = 0; i < 3; i++) {
			glEnableVertexAttribArray(8 + i);
			glVertexAttribPointer(8 + i, 3, GL_FLOAT, GL_FALSE, sizeof(LightInstanceData), (void*)(((int)offsetof(LightInstanceData, position)) + (i * sizeof(mathfu::vec3))));
			glVertexAttribDivisor(8 + i, 1);
		}
		for (u32 i = 0; i < 3; i++) {
			glEnableVertexAttribArray(11 + i);
			glVertexAttribPointer(11 + i, 1, GL_FLOAT, GL_FALSE, sizeof(LightInstanceData), (void*)(((int)offsetof(LightInstanceData, power)) + (i * sizeof(float))));
			glVertexAttribDivisor(11 + i, 1);
		}
	}else {
		glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData), (void*)0, GL_DYNAMIC_DRAW);
		for (u32 i = 0; i < 4; i++) {
			glEnableVertexAttribArray(4 + i);
			glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(4 * i * sizeof(float)));
			glVertexAttribDivisor(4 + i, 1);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh* Mesh::quad()
{
	static Mesh* out = (new Mesh({
		Vertex(-1.0f,  1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
		Vertex( 1.0f,  1.0f, 0.0f, 1.0f, 1.0f),
		Vertex( 1.0f, -1.0f, 0.0f, 1.0f, 0.0f)
	}, {
		0, 1, 2,
		2, 1, 3
	}))->setMaterial(nullptr);
	return out;
}

Mesh* Mesh::sphere()
{
	static Mesh* mesh = createSphere()->setMaterial(nullptr);
	return mesh;
}

Mesh* Mesh::cube()
{
	static Mesh* mesh = createCube()->setMaterial(nullptr);
	return mesh;
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

Mesh* Mesh::setNormal(Texture* texture)
{
	mNormal = texture;
	return this;
}

Mesh* Mesh::setDisplacment(Texture* texture)
{
	mDisplacement = texture;
	return this;
}

std::string Mesh::getHash() const
{
	return mHash;
}

std::string Mesh::getClass() const
{
	return "Mesh";
}

u32 Mesh::getInstanceVBO() const
{
	return mInstanceVBO;
}
