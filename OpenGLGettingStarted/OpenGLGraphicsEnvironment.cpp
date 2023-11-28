#include <Windows.h>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "OpenGLGraphicsEnvironment.h"
#include "GlfwGraphicsWindow.h"
#include "GraphicsObject.h"
#include "Generate.h"
#include <iostream>
#include "RotateAnimation.h"
#include "RotateAboutPivotAnimation.h"
#include "RigidBody.h"
#include "ForceGenerator.h"
#include "ForceObject.h"

OpenGLGraphicsEnvironment::OpenGLGraphicsEnvironment(Logger& logger) : 
    m_logger(logger), m_majorVersion(4), m_minorVersion(6), whichObject(KeyboardInput::CameraMovement)
{
}

void OpenGLGraphicsEnvironment::SetVersion(int majorVersion, int minorVersion)
{
	m_majorVersion = majorVersion;
	m_minorVersion = minorVersion;
}

void OpenGLGraphicsEnvironment::SetGraphicsWindow(std::shared_ptr<AbstractGraphicsWindow> window)
{
	m_window = dynamic_pointer_cast<GlfwGraphicsWindow>(window);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    OpenGLGraphicsEnvironment* env = (OpenGLGraphicsEnvironment*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        if (env->m_allObjects.count("force") == 1)
            return;
        auto lineArrowMesh = Generate::LineArrow(1.0f, { 1, 0, 0 });
        auto vertexBuffer = std::make_shared<VertexBuffer>(6);
        vertexBuffer->SetPrimitiveType(GL_LINES);
        vertexBuffer->GenerateBufferId("VBO", BufferDataType::VertexData);
        vertexBuffer->GenerateBufferId("IBO", BufferDataType::IndexData);
        vertexBuffer->SetIsIndexed(true);
        vertexBuffer->AddVertexAttribute("Position", 0, 3);
        vertexBuffer->AddVertexAttribute("Color", 1, 3);
        vertexBuffer->StaticAllocate("VBO", lineArrowMesh->GetVertexData());
        vertexBuffer->StaticAllocate("IBO", lineArrowMesh->GetIndexData());
        lineArrowMesh->SetBuffer(vertexBuffer);
        env->m_renderer->AddVertexBuffer("force", vertexBuffer);

        auto force = std::make_shared<ForceObject>();
        force->magnitude = 1.0f;
        force->frame.SetPosition(glm::vec3(-1.0, 3.0f, 0.0f));
        force->normal = glm::vec3(1.0f, 0.0f, 0.0f);
        env->m_allObjects["force"] = force;
        env->m_allObjects["force"]->mesh = std::move(lineArrowMesh);
        env->m_currentScene->AddObject("force", env->m_allObjects["force"]);
        return;
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        env->ToggleInput();
        return;
    }
}

void OpenGLGraphicsEnvironment::Initialize()
{
    m_window->SetOpenGLVersion(m_majorVersion, m_minorVersion);

    m_window->Initialize();

    // Set up the window for antialiasing by using multisampling
    glfwWindowHint(GLFW_SAMPLES, 4);

    bool created = m_window->Create();
    if (created == false) {
        throw "Failed to create the graphics window.";
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw "Failed to initialize GLAD.";
    }

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    // Cull back faces and use counter-clockwise winding of front faces
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    m_renderer = std::make_unique<Renderer>();
    m_camera = std::make_shared<Camera>();
    m_renderer->SetCamera(m_camera);
    m_timer = std::make_unique<HighResolutionTimer>();

    LoadShaders();
    LoadObjects();

    m_window->AddData((void*)this);
    m_window->SetCallback(key_callback);

    m_camera->frame.SetPosition(0.0f, 2.0f, 6.0f);
}

void OpenGLGraphicsEnvironment::Run()
{
    m_timer->StartTiming();
    m_window->SetupFramebufferSizeCallback();
    m_timer->StartTiming();
    double elapsedSeconds;
    while (m_window->IsTimeToClose() == false) {
        elapsedSeconds = m_timer->GetElapsedTimeInSeconds();
        m_window->GetWindowSize();
        m_window->CheckInputs();
        CheckKeyState();
        m_camera->SetupLookingForward();
        m_camera->SetupProjectionAndView(m_window->GetAspectRatio());
        m_window->Clear();
        m_camera->Update(elapsedSeconds);
        m_currentScene->Update(elapsedSeconds);
        m_renderer->Render();
        m_window->NextFrame();
    }
}

void OpenGLGraphicsEnvironment::CheckKeyState()
{
    if (m_window->GetKeyState(GLFW_KEY_1) == GLFW_PRESS) {
        m_camera->frame.SetPosition(3.0f, 3.0f, 5.0f);
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_2) == GLFW_PRESS) {
        m_camera->frame.SetPosition(3.0f, 3.0f, -5.0f);
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_3) == GLFW_PRESS) {
        m_camera->frame.SetPosition(-3.0f, 3.0f, 5.0f);
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_4) == GLFW_PRESS) {
        m_camera->frame.SetPosition(-3.0f, 3.0f, -5.0f);
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_5) == GLFW_PRESS) {
        m_camera->frame.SetPosition(3.0f, -3.0f, 5.0f);
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_6) == GLFW_PRESS) {
        m_camera->frame.SetPosition(3.0f, -3.0f, -5.0f);
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_7) == GLFW_PRESS) {
        m_camera->frame.SetPosition(-3.0f, -3.0f, 5.0f);
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_8) == GLFW_PRESS) {
        m_camera->frame.SetPosition(-3.0f, -3.0f, -5.0f);
        return;
    }

    if (m_window->GetKeyState(GLFW_KEY_D) == GLFW_PRESS) {
        if(whichObject == KeyboardInput::CameraMovement)
            m_camera->SetState(CameraState::TurningRight);
        else if(m_allObjects.count("force") == 1) {
            std::shared_ptr<ForceObject> force = std::dynamic_pointer_cast<ForceObject>(m_allObjects["force"]);
            force->SetState(ForceObjectState::TurningRight);
        }
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_A) == GLFW_PRESS) {
        if(whichObject == KeyboardInput::CameraMovement)
            m_camera->SetState(CameraState::TurningLeft);
        else if(m_allObjects.count("force") == 1) {
            std::shared_ptr<ForceObject> force = std::dynamic_pointer_cast<ForceObject>(m_allObjects["force"]);
            force->SetState(ForceObjectState::TurningLeft);
        }
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_W) == GLFW_PRESS) {
        if(whichObject == KeyboardInput::CameraMovement)
            m_camera->SetState(CameraState::MovingForward);
        else if(m_allObjects.count("force") == 1) {
            std::shared_ptr<ForceObject> force = std::dynamic_pointer_cast<ForceObject>(m_allObjects["force"]);
            force->SetState(ForceObjectState::MovingForward);
        }
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_S) == GLFW_PRESS) {
        if(whichObject == KeyboardInput::CameraMovement)
            m_camera->SetState(CameraState::MovingBackward);
        else if (m_allObjects.count("force") == 1) {
            std::shared_ptr<ForceObject> force = std::dynamic_pointer_cast<ForceObject>(m_allObjects["force"]);
            force->SetState(ForceObjectState::MovingBackward);
        }
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_Q) == GLFW_PRESS) {
        if (m_allObjects.count("force") == 1 && whichObject == KeyboardInput::ForceMovement) {
            std::shared_ptr<ForceObject> force = std::dynamic_pointer_cast<ForceObject>(m_allObjects["force"]);
            force->SetState(ForceObjectState::TurningUp);
        }
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_E) == GLFW_PRESS) {
        if (m_allObjects.count("force") == 1 && whichObject == KeyboardInput::ForceMovement) {
            std::shared_ptr<ForceObject> force = std::dynamic_pointer_cast<ForceObject>(m_allObjects["force"]);
            force->SetState(ForceObjectState::TurningDown);
        }
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_UP) == GLFW_PRESS) {
        if(whichObject == KeyboardInput::CameraMovement)
            m_camera->SetState(CameraState::MovingUp);
        else if (m_allObjects.count("force") == 1) {
            std::shared_ptr<ForceObject> force = std::dynamic_pointer_cast<ForceObject>(m_allObjects["force"]);
            force->SetState(ForceObjectState::MovingUp);
        }
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_DOWN) == GLFW_PRESS) {
        if(whichObject == KeyboardInput::CameraMovement)
            m_camera->SetState(CameraState::MovingDown);
        else if (m_allObjects.count("force") == 1) {
            std::shared_ptr<ForceObject> force = std::dynamic_pointer_cast<ForceObject>(m_allObjects["force"]);
            force->SetState(ForceObjectState::MovingDown);
        }
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_LEFT) == GLFW_PRESS) {
        if(whichObject == KeyboardInput::CameraMovement)
            m_camera->SetState(CameraState::StrafingLeft);
        else if (m_allObjects.count("force") == 1) {
            std::shared_ptr<ForceObject> force = std::dynamic_pointer_cast<ForceObject>(m_allObjects["force"]);
            force->SetState(ForceObjectState::StrafingLeft);
        }
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if(whichObject == KeyboardInput::CameraMovement)
            m_camera->SetState(CameraState::StrafingRight);
        else if (m_allObjects.count("force") == 1) {
            std::shared_ptr<ForceObject> force = std::dynamic_pointer_cast<ForceObject>(m_allObjects["force"]);
            force->SetState(ForceObjectState::StrafingRight);
        }
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_SPACE) == GLFW_PRESS) {
        for (auto& it : m_allObjects) {
            if (it.second->IsRigidBody() && m_allObjects.count("force") == 1) {
                std::shared_ptr<RigidBody> body = std::dynamic_pointer_cast<RigidBody>(it.second);
                std::shared_ptr<ForceObject> force = std::dynamic_pointer_cast<ForceObject>(m_allObjects["force"]);
                glm::vec3 forceValue = force->normal * force->magnitude;
                glm::vec3 point = glm::mat3(force->frame.orientation) * force->frame.GetPosition();
                body->AddForceAtPoint(forceValue, point);
            }
        }
        return;
    }
    if (m_window->GetKeyState(GLFW_KEY_I) == GLFW_PRESS) {
        if (m_allObjects.count("force") == 0)
            return;

        return;
    }
    m_camera->SetState(CameraState::NotMoving);
    if (m_allObjects.count("force") == 1) {
        std::shared_ptr<ForceObject> force = std::dynamic_pointer_cast<ForceObject>(m_allObjects["force"]);
        force->SetState(ForceObjectState::NotMoving);
    }
}

void OpenGLGraphicsEnvironment::LoadObjects()
{
    m_currentScene = std::make_shared<Scene>();
    m_renderer->SetScene(m_currentScene);
    m_renderer->SetShader(m_shaders["basic3dlighting"]);

    auto flatSurfaceMesh = Generate::FlatSurface(10, 10, { 0.0f, 0.5f, 0.0f });
    auto vertexBuffer = std::make_shared<VertexBuffer>();
    vertexBuffer->GenerateBufferId("VBO", BufferDataType::VertexData);
    vertexBuffer->GenerateBufferId("IBO", BufferDataType::IndexData);
    vertexBuffer->SetIsIndexed(true);
    vertexBuffer->AddVertexAttribute("Position", 0, 3);
    vertexBuffer->AddVertexAttribute("Color", 1, 3);
    vertexBuffer->AddVertexAttribute("Normal", 2, 3);
    vertexBuffer->StaticAllocate("IBO", flatSurfaceMesh->GetIndexData());
    vertexBuffer->StaticAllocate("VBO", flatSurfaceMesh->GetVertexData());
    flatSurfaceMesh->SetBuffer(vertexBuffer);
    m_renderer->AddVertexBuffer("flatsurfacebuffer", vertexBuffer);

    m_allObjects["flatsurface"] = std::make_shared<GraphicsObject>();
    m_allObjects["flatsurface"]->mesh = std::move(flatSurfaceMesh);

    auto cuboidMesh = Generate::Cuboid(1, 1, 1, { 1.0f, 0.0f, 0.0f });
    vertexBuffer = std::make_shared<VertexBuffer>();
    vertexBuffer->GenerateBufferId("VBO", BufferDataType::VertexData);
    vertexBuffer->AddVertexAttribute("Position", 0, 3);
    vertexBuffer->AddVertexAttribute("Color", 1, 3);
    vertexBuffer->AddVertexAttribute("Normal", 2, 3);
    vertexBuffer->StaticAllocate("VBO", cuboidMesh->GetVertexData());
    cuboidMesh->SetBuffer(vertexBuffer);
    m_renderer->AddVertexBuffer("cuboidbuffer", vertexBuffer);

    m_allObjects["red cube"] = std::make_shared<GraphicsObject>();
    m_allObjects["red cube"]->mesh = std::move(cuboidMesh);

    cuboidMesh = Generate::Cuboid(0.5f, 0.5f, 0.5f, { 1, 1, 1 });
    vertexBuffer = std::make_shared<VertexBuffer>();
    vertexBuffer->GenerateBufferId("VBO", BufferDataType::VertexData);
    vertexBuffer->AddVertexAttribute("Position", 0, 3);
    vertexBuffer->AddVertexAttribute("Color", 1, 3);
    vertexBuffer->AddVertexAttribute("Normal", 2, 3);
    vertexBuffer->StaticAllocate("VBO", cuboidMesh->GetVertexData());
    cuboidMesh->SetBuffer(vertexBuffer);
    m_renderer->AddVertexBuffer("whitecuboidbuffer", vertexBuffer);

    m_allObjects["white cube"] = std::make_shared<GraphicsObject>();
    m_allObjects["white cube"]->mesh = std::move(cuboidMesh);

    auto cylinderMesh = Generate::Cylinder(0.25f, 2.0f, 16, 16, { 1, 1, 0 }, ShadingType::Smooth_Shading);
    vertexBuffer = std::make_shared<VertexBuffer>();
    vertexBuffer->GenerateBufferId("VBO", BufferDataType::VertexData);
    vertexBuffer->AddVertexAttribute("Position", 0, 3);
    vertexBuffer->AddVertexAttribute("Color", 1, 3);
    vertexBuffer->AddVertexAttribute("Normal", 2, 3);
    vertexBuffer->StaticAllocate("VBO", cylinderMesh->GetVertexData());
    cylinderMesh->SetBuffer(vertexBuffer);
    m_renderer->AddVertexBuffer("yellowcylinderbuffer", vertexBuffer);

    m_allObjects["yellow cylinder"] = std::make_shared<GraphicsObject>();
    m_allObjects["yellow cylinder"]->mesh = std::move(cylinderMesh);

    auto lineArrowMesh = Generate::LineArrow(1.0f, {0, 0, 1});
    vertexBuffer = std::make_shared<VertexBuffer>(6);
    vertexBuffer->SetPrimitiveType(GL_LINES);
    vertexBuffer->GenerateBufferId("VBO", BufferDataType::VertexData);
    vertexBuffer->GenerateBufferId("IBO", BufferDataType::IndexData);
    vertexBuffer->SetIsIndexed(true);
    vertexBuffer->AddVertexAttribute("Position", 0, 3);
    vertexBuffer->AddVertexAttribute("Color", 1, 3);
    vertexBuffer->StaticAllocate("VBO", lineArrowMesh->GetVertexData());
    vertexBuffer->StaticAllocate("IBO", lineArrowMesh->GetIndexData());
    lineArrowMesh->SetBuffer(vertexBuffer);
    m_renderer->AddVertexBuffer("linearrowbuffer", vertexBuffer);

    m_allObjects["line arrow 1"] = std::make_shared<GraphicsObject>();
    m_allObjects["line arrow 1"]->mesh = std::move(lineArrowMesh);

    auto bodyMesh = Generate::Cylinder(0.25f, 2.0f, 16, 16, { 1, 0, 0 }, ShadingType::Smooth_Shading);
    vertexBuffer = std::make_shared<VertexBuffer>();
    vertexBuffer->GenerateBufferId("VBO", BufferDataType::VertexData);
    vertexBuffer->AddVertexAttribute("Position", 0, 3);
    vertexBuffer->AddVertexAttribute("Color", 1, 3);
    vertexBuffer->AddVertexAttribute("Normal", 2, 3);
    vertexBuffer->StaticAllocate("VBO", bodyMesh->GetVertexData());
    bodyMesh->SetBuffer(vertexBuffer);
    m_renderer->AddVertexBuffer("bodybuffer", vertexBuffer);

    auto body = std::make_shared<RigidBody>();
    body->SetMass(2.0f);
    body->SetShape(Shape::Cylinder(body->GetMass(), 0.25f, 2.0f));
    body->frame.SetPosition(0.0f, 1.0f, 0.0f);
    //body->frame.Rotate(90.0f, glm::vec3(0, 0, 1));
    //auto g = std::make_unique<Gravity>();
    //g->SetAcceleration(0.0f, -4.0f, 0.0f);
    //body->constForces["gravity"] = std::move(g);
    //body->AddForceAtBodyPoint(glm::vec3(13.5f, 0.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f));

    m_allObjects["body"] = body;
    m_allObjects["body"]->mesh = std::move(bodyMesh);

    m_currentScene->AddObject("red cube", m_allObjects["red cube"]);
    m_currentScene->AddObject("white cube", m_allObjects["white cube"]);
    m_currentScene->AddObject("yellow cylinder", m_allObjects["yellow cylinder"]);
    m_currentScene->AddObject("flatsurface", m_allObjects["flatsurface"]);
    m_currentScene->AddObject("line arrow 1", m_allObjects["line arrow 1"]);
    m_currentScene->AddObject("body", m_allObjects["body"]);

    auto rotateAnimation = std::make_unique<RotateAnimation>(90.0f, glm::vec3(0, 1, 0));
    m_allObjects["white cube"]->SetAnimation(std::move(rotateAnimation));

    m_allObjects["white cube"]->frame.SetPosition(0, 0.5f, 2.5f);
    m_allObjects["red cube"]->frame.SetPosition(0, 0.5f, 0);
    m_allObjects["yellow cylinder"]->frame.SetPosition(2.0, 1.0f, 0.0f);
    m_allObjects["line arrow 1"]->frame.SetPosition(0.0, 3.0f, 0.0f);
    //m_allObjects["yellow cylinder"]->frame.RotateAboutLocalPivot(glm::vec3(0, 1, 0), 45, glm::vec3(1, 0, 0));
    auto rotateAboutPivotAnimation = std::make_unique<RotateAboutPivotAnimation>(glm::vec3(0, 0.9f, 0), 90.0f, glm::vec3(1, 0, 0));
    m_allObjects["yellow cylinder"]->SetAnimation(std::move(rotateAboutPivotAnimation));

    rotateAboutPivotAnimation = std::make_unique<RotateAboutPivotAnimation>(glm::vec3(-0.5f, 0.5f, 0.5f), 180.0f, glm::vec3(0, 0, 1));
    m_allObjects["red cube"]->SetAnimation(std::move(rotateAboutPivotAnimation));
}

void OpenGLGraphicsEnvironment::LoadShaders()
{
    CreateBasicShader();
    CreateBasic3DShader();
    ReadShadersFromFiles("basic3dlighting.vert.glsl", "basic3dlighting.frag.glsl");
}

void OpenGLGraphicsEnvironment::CreateBasicShader()
{
    auto shader = make_shared<Shader>(m_logger);
    std::string vertexSourceCode =
        "#version 460\n"\
        "layout(location = 0) in vec3 position;\n"\
        "layout(location = 1) in vec3 vertexColor;\n"\
        "out vec4 fragColor;\n"\
        "void main()\n"\
        "{\n"\
        "   gl_Position = vec4(position, 1.0);\n" \
        "   fragColor = vec4(vertexColor, 1.0);\n" \
        "}\n";
    std::string fragmentSourceCode =
        "#version 400\n"\
        "in vec4 fragColor;\n"\
        "out vec4 color;\n"\
        "void main()\n"\
        "{\n"\
        "   color = fragColor;\n"\
        "}\n";

   shader->Create(vertexSourceCode, fragmentSourceCode);
   m_shaders["basic"] = shader;
}

void OpenGLGraphicsEnvironment::CreateBasic3DShader()
{
    auto shader = make_shared<Shader>(m_logger);
    std::string vertexSourceCode =
        "#version 460\n"\
        "layout(location = 0) in vec3 position;\n"\
        "layout(location = 1) in vec3 vertexColor;\n"\
        "out vec4 fragColor;\n"\
        "uniform mat4 uWorld;\n"\
        "uniform mat4 uView;\n"\
        "uniform mat4 uProjection;\n"\
        "void main()\n"\
        "{\n"\
        "   gl_Position = uProjection * uView * uWorld * vec4(position, 1.0);\n" \
        "   fragColor = vec4(vertexColor, 1.0);\n" \
        "}\n";
    std::string fragmentSourceCode =
        "#version 400\n"\
        "in vec4 fragColor;\n"\
        "out vec4 color;\n"\
        "void main()\n"\
        "{\n"\
        "   color = fragColor;\n"\
        "}\n";

    shader->Create(vertexSourceCode, fragmentSourceCode);
    m_shaders["basic3d"] = shader;
}

void OpenGLGraphicsEnvironment::CreateBasic3DLightingShader()
{
    auto shader = make_shared<Shader>(m_logger);
    std::string vertexSourceCode =
        "#version 460\n"\
        "layout(location = 0) in vec3 position;\n"\
        "layout(location = 1) in vec3 vertexColor;\n"\
        "layout(location = 2) in vec3 vertexNormal;\n"\
        "out vec4 fragColor;\n"\
        "out vec3 fragNormal;\n"\
        "out vec3 fragPosition;\n"\
        "uniform mat4 uWorld;\n"\
        "uniform mat4 uView;\n"\
        "uniform mat4 uProjection;\n"\
        "void main()\n"\
        "{\n"\
        "   vec4 worldPosition = uWorld * vec4(position, 1.0);\n" \
        "   gl_Position = uProjection * uView * worldPosition;\n" \
        "   vec3 worldNormal = mat3(uWorld) * vertexNormal;\n" \
        "   fragPosition = vec3(worldPosition);\n" \
        "   fragNormal = normalize(worldNormal);\n" \
        "   fragColor = vec4(vertexColor, 1.0);\n" \
        "}\n";
    std::string fragmentSourceCode =
        "#version 460\n"\
        "in vec4 fragColor;\n"\
        "in vec3 fragNormal;\n"\
        "in vec3 fragPosition;\n"\
        "out vec4 color;\n"\
        "uniform float uMaterialAmbientIntensity;\n"\
        "uniform vec3 uGlobalLightPosition;\n"\
        "uniform vec3 uGlobalLightColor;\n"\
        "uniform float uGlobalLightIntensity;\n"\
        "void main()\n"\
        "{\n"\
        "   vec3 toGlobalLightDir = normalize(uGlobalLightPosition - fragPosition);\n"\
        "   float cosAngIncidence = dot(fragNormal, toGlobalLightDir);\n"\
        "   cosAngIncidence = clamp(cosAngIncidence, 0.0f, 1.0f);\n"\
        "   vec4 globalDiffuse = cosAngIncidence * uGlobalLightIntensity * vec4(uGlobalLightColor, 1.0f);\n"\
        "   vec4 ambientColor = uMaterialAmbientIntensity * vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"\
        "   color = (ambientColor + globalDiffuse)  * fragColor;\n"\
        "}\n";

    shader->Create(vertexSourceCode, fragmentSourceCode);
    m_shaders["basic3dlighting"] = shader;
}

void OpenGLGraphicsEnvironment::ReadShadersFromFiles(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
    auto shader = make_shared<Shader>(m_logger);
    shader->ReadFromFile(vertexFilePath, fragmentFilePath);
    m_shaders["basic3dlighting"] = shader;
}

void OpenGLGraphicsEnvironment::ToggleInput() {
    if (whichObject == KeyboardInput::CameraMovement)
        whichObject = KeyboardInput::ForceMovement;
    else
        whichObject = KeyboardInput::CameraMovement;
}