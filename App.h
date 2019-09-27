#ifndef _APP_H_
#define _APP_H_

#include "helpers.h"
#include "Camera.h"

class App
{
public:
	App(std::string name);
	virtual ~App();

	virtual void run() final;

	virtual void init();

	virtual void update();

	virtual void render();

	virtual void renderScene(Camera* camera);

	virtual void destroy();

private:

	void processMouse(double xpos, double ypos);

protected:
	GLFWwindow* window;

	f32 mouseX = 0.0f;
	f32 mouseY = 0.0f;
	f32 mouseDeltaX = 0.0f;
	f32 mouseDeltaY = 0.0f;

private:
	f32 mouseLastX = 0.0f;
	f32 mouseLastY = 0.0f;
};

#endif // _APP_H_



