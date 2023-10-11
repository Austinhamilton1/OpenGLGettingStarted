#pragma once
#include <memory>
#include <map>
#include "GlfwGraphicsWindow.h"
#include "GraphicsObject.h"
#include "Shader.h"
#include "Scene.h"
#include "Camera.h"
#include "Renderer.h"

class OpenGLGraphicsEnvironment
{
protected:
	Logger& m_logger;
	int m_majorVersion, m_minorVersion;
	std::shared_ptr<GlfwGraphicsWindow> m_window;
	std::map<std::string,std::shared_ptr<Shader>> m_shaders;
	std::unique_ptr<Scene> m_currentScene;
	std::map<std::string, std::shared_ptr<GraphicsObject>> m_allObjects;
	std::shared_ptr<Camera> m_camera;
	std::unique_ptr<Renderer> m_renderer;

public:
	OpenGLGraphicsEnvironment(Logger& logger);
	void SetVersion(int majorVersion, int minorVersion);
	void SetGraphicsWindow(std::shared_ptr<AbstractGraphicsWindow> window);
	void Initialize();
	void Run();

private:
	void LoadObjects();
	void LoadShaders();
	void CreateBasicShader();
	void CreateBasic3DShader();
	static void GLAPIENTRY
		MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}
};

