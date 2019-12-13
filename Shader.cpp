#include "shader.h"



Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
    char tempInfo[1024];
    std::string vs = getFileContent(vertexFile);
    std::string fs = getFileContent(fragmentFile);

    u32 vertexShader;
    u32 fragmentShader;

    int success = 0;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char* tempStr = (char *)vs.c_str();
    glShaderSource(vertexShader, 1, &tempStr, 0);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (! success) {
        glGetShaderInfoLog(vertexShader, 1024, 0, tempInfo);
        std::cout << "VERTEX SHADER ERROR:" << tempInfo << std::endl;
        glfwTerminate();
        exit(-1);
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    tempStr = (char *)fs.c_str();
    glShaderSource(fragmentShader, 1, &tempStr, 0);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (! success) {
        glGetShaderInfoLog(fragmentShader, 1024, 0, tempInfo);
        std::cout << "FRAGMENT SHADER ERROR:" << tempInfo << std::endl;
        glfwTerminate();
        exit(-1);
    }

    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, vertexShader);
    glAttachShader(mShaderProgram, fragmentShader);
    glLinkProgram(mShaderProgram);

    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
    if (! success) {
        glGetProgramInfoLog(mShaderProgram, 1024, 0, tempInfo);
        std::cout << "SHADER LINK ERROR:" << tempInfo << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(mShaderProgram);
}

bool Shader::propertyExists(std::string name) const
{
	return glGetUniformLocation(mShaderProgram, name.c_str()) == -1;
}

void Shader::setBool(std::string name, bool value)
{
	use();
    glUniform1i(glGetUniformLocation(mShaderProgram, name.c_str()), value);
}

void Shader::setInt(std::string name, int value)
{
	use();
    glUniform1i(glGetUniformLocation(mShaderProgram, name.c_str()), value);
}

void Shader::setFloat(std::string name, float value)
{
	use();
    glUniform1f(glGetUniformLocation(mShaderProgram, name.c_str()), value);
}

void Shader::setFloat2(std::string name, float value1, float value2)
{
	use();
    glUniform2f(glGetUniformLocation(mShaderProgram, name.c_str()), value1, value2);
}

void Shader::setFloat3(std::string name, mathfu::vec3 value)
{
	use();
    setFloat3(name, value[0], value[1], value[2]);
}

void Shader::setFloat3(std::string name, float value1, float value2, float value3)
{
	use();
    glUniform3f(glGetUniformLocation(mShaderProgram, name.c_str()), value1, value2, value3);
}

void Shader::setFloat4(std::string name, float value1, float value2, float value3, float value4)
{
	use();
    glUniform4f(glGetUniformLocation(mShaderProgram, name.c_str()), value1, value2, value3, value4);
}

void Shader::setMatrix(std::string name, mathfu::mat4 value)
{
	use();
    glUniformMatrix4fv(glGetUniformLocation(mShaderProgram, name.c_str()), 1, false, (float*)&value);
}

void Shader::setTexture(std::string name, const Texture* value)
{
	if (value != nullptr) {
		static u32 index;
		mTextures[name] = value;
		index = std::distance(mTextures.begin(), mTextures.find(name));
		value->use(index);
		setInt(name, index);
	}
}

void Shader::setCustom(std::string name, void* _data, u32 _size)
{
	if (mBufferDatas.find(name) == mBufferDatas.end()) {
		u32 uniformBuffer;
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glGenBuffers(1, &uniformBuffer);
		mBufferDatas[name] = uniformBuffer;
	}
	glBindBuffer(GL_UNIFORM_BUFFER, mBufferDatas[name]);
	glBufferData(GL_UNIFORM_BUFFER, _size, _data, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	u32 index = std::distance(mTextures.begin(), mTextures.find(name));
	use();
	setInt(name, index);
}

void Shader::use()
{
    if (mShaderProgram != 0) {
        glUseProgram(mShaderProgram);
    }
}

Shader* Shader::simple()
{
	static Shader* simpleShader = new Shader("assets/shaders/simple_vertex.vert", "assets/shaders/simple_fragment.frag");
	return simpleShader;
}

Shader* Shader::lightShader()
{
	static Shader* lightShader = new Shader("assets/shaders/light_vertex.vert", "assets/shaders/light.frag");
	return lightShader;
}

Shader* Shader::defaultMaterial()
{
	static Shader* defaultMaterial = new Shader("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
	return defaultMaterial;
}
