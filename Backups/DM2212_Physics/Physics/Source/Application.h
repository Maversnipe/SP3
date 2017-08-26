
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "Scene.h"

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double *xpos, double *ypos);
	static int GetWindowWidth();
	static int GetWindowHeight();

	//Declare a window object
	static bool IsExit;
	static GLFWwindow* m_window;
	static const unsigned char Application::FPS; // FPS of this game
	static const unsigned int Application::frameTime; // time for each frame
	static StopWatch m_timer;

private:
	Application();
	~Application();
};

#endif