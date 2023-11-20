#pragma once
#include "AbstractGraphicsWindow.h"
class GlfwGraphicsWindow :  public AbstractGraphicsWindow
{
protected:
	int m_openglMajorVersion, m_openglMinorVersion;
	GLFWwindow* m_window;

public:
	GlfwGraphicsWindow(string title, int width = 800, int height = 600);
	~GlfwGraphicsWindow();

	void SetOpenGLVersion(int majorVersion, int minorVersion);
	void SetupFramebufferSizeCallback();
	void Initialize();
	bool Create();
	bool IsTimeToClose();
	void CheckInputs();
	unsigned int GetKeyState(unsigned int whichKey);
	void Clear();
	void NextFrame();
	void GetWindowSize();
	void GetCursorPos(double* x, double* y);
	unsigned int GetMouseState(unsigned int whichKey);

private:
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

