#pragma once
#include "AbstractGraphicsWindow.h"
#include <unordered_map>
class GlfwGraphicsWindow :  public AbstractGraphicsWindow
{
protected:
	int m_openglMajorVersion, m_openglMinorVersion;
	GLFWwindow* m_window;
	std::unordered_map<unsigned int, void (*)()> callbackFunctions;

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
	void SetCallback(void (*callback) (GLFWwindow*, int, int, int, int));
	void AddData(void* data);

private:
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

