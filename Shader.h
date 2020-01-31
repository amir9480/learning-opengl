#ifndef _SHADER_H_
#define _SHADER_H_

#include "Texture.h"
#include "helpers.h"


class Shader
{
public:
    Shader(const char* vertexFile, const char* fragmentFile);
    virtual ~Shader();

	bool propertyExists(std::string name) const;
    void setBool(std::string name, bool value);
    void setInt(std::string name, int value);
    void setFloat(std::string name, float value);
    void setFloat2(std::string name, float value1, float value2);
    void setFloat3(std::string name, mathfu::vec3 value);
	void setFloat3(std::string name, float value1, float value2, float value3);
	void setFloat4(std::string name, mathfu::vec4 value);
    void setFloat4(std::string name, float value1, float value2, float value3, float value4);
    void setMatrix(std::string name, mathfu::mat4 value);
	void setTexture(std::string name, const Texture* value);
	void setCustom(std::string name, void* _data, u32 _size);

    void use();

	static Shader* simple();

	static Shader* lightShader();

	static Shader* defaultMaterial();

	static Shader* ambientLightShader();

	std::string mVertexFile;
	std::string mFragmentFile;
protected:
    u32 mShaderProgram = 0;
	std::map<std::string, const Texture*> mTextures;
	std::map<std::string, u32> mBufferDatas;
};


#endif // _SHADER_H_
