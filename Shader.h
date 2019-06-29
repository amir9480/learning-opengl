#ifndef _SHADER_H_
#define _SHADER_H_

#include "Texture.h"
#include "helpers.h"


class Shader
{
public:
    Shader(const char* vertexFile, const char* fragmentFile);
    virtual ~Shader();

    void setBool(std::string name, bool value) const;
    void setInt(std::string name, int value) const;
    void setFloat(std::string name, float value) const;
    void setFloat2(std::string name, float value1, float value2) const;
    void setFloat3(std::string name, mathfu::vec3 value) const;
    void setFloat3(std::string name, float value1, float value2, float value3) const;
    void setFloat4(std::string name, float value1, float value2, float value3, float value4) const;
    void setMatrix(std::string name, mathfu::mat4 value);
	void setTexture(std::string name, const Texture* value, u32 index);

    void use();
protected:
    u32 mShaderProgram = 0;
};

#endif // _SHADER_H_
