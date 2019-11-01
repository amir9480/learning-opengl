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
    void setFloat4(std::string name, float value1, float value2, float value3, float value4);
    void setMatrix(std::string name, mathfu::mat4 value);
	void setTexture(std::string name, const Texture* value, u32 index);

    void use();

	static Shader* simple();

	static Shader* lightShader();

	static Shader* defaultMaterial();

public:
protected:
    u32 mShaderProgram = 0;
};


#endif // _SHADER_H_
