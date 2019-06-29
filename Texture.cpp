#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h";

Texture::Texture(std::string _file)
{
	stbi_set_flip_vertically_on_load(true);
	int channels = 0;
	u8* data = stbi_load(_file.c_str(), &mWidth, &mHeight, &channels, 0);
	if (!data) {
		ABORT("Texture File does not exists.");
	}
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(int _width, int _height, bool depth)
{
	mWidth = _width;
	mHeight = _height;
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, depth ? GL_DEPTH_COMPONENT : GL_RGBA, mWidth, mHeight, 0, depth ? GL_DEPTH_COMPONENT : GL_RGBA, GL_UNSIGNED_BYTE, 0);
	if (depth) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mTexture);
}

void Texture::use(u32 index) const
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

Texture* Texture::createRaw(int _color, int _width, int _height)
{
	_color = ((_color & 0xff000000) >> 24) + ((_color & 0x00ff0000) >> 8) + ((_color & 0x0000ff00) << 8);
	Texture* out = new Texture();
	out->mWidth = _width;
	out->mHeight = _height;
	u8* data = new u8[_width * _height * 3];
	for (int w = 0; w < _width; w++) {
		for (int h = 0; h < _height; h++) {
			memcpy(&data[(h * _width + w) * 3], &_color, 3);
		}
	}
	glGenTextures(1, &out->mTexture);
	glBindTexture(GL_TEXTURE_2D, out->mTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	delete[] data;
	glBindTexture(GL_TEXTURE_2D, 0);
	return out;
}

Texture* Texture::defaultTexture()
{
	static Texture* out = Texture::createRaw(0x33333300, 32, 32);
	return out;
}

Texture::Texture()
{
}
