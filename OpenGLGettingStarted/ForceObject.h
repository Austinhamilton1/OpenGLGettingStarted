#pragma once
#include "GraphicsObject.h"
#include "ForceGenerator.h"
#include "Camera.h"

enum class ForceObjectState {
    NotMoving, MovingForward, MovingBackward, MovingUp, MovingDown,
    TurningRight, TurningLeft,
    StrafingRight, StrafingLeft,
    TurningUp, TurningDown,
    Growing, Shrinking
};

class ForceObject : public GraphicsObject {
protected:
    ForceObjectState state;
    std::shared_ptr<Camera> m_camera;

public:
	float magnitude;
	glm::vec3 normal;
    ForceObject() : magnitude(1), normal(glm::vec3(1.0f, 0.0f, 0.0f)),
        state(ForceObjectState::NotMoving) {};
    void SetState(ForceObjectState state) { this->state = state; }
    void Update(double elapsedSeconds);
    void UpdateNormal();
    void SetPosition(const glm::vec3& pos) { frame.SetPosition(pos); };
    void SetPosition(float x, float y, float z) { SetPosition(glm::vec3(x, y, z)); };
    void SetCamera(std::shared_ptr<Camera> m_camera) { this->m_camera = m_camera; };
};