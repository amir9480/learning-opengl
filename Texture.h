#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "helpers.h"

class Camera;

class Texture
{
	friend Camera;
public:
	Texture(std::string _file);
	Texture(int _width, int _height, bool depth = false);
	virtual ~Texture();

	void use(u32 index) const;

	static Texture* createRaw(int _color, int _width = 1, int _height = 1);

	static Texture* defaultTexture();
private:
	u32 mTexture = 0;
	int mWidth = 0;
	int mHeight = 0;

	Texture();
};

#endif // _TEXTURE_H_