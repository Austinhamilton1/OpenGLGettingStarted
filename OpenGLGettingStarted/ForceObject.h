#pragma once
#include "GraphicsObject.h"
#include "ForceGenerator.h"

enum class ForceObjectState {
    NotMoving, MovingForward, MovingBackward, MovingUp, MovingDown,
    TurningRight, TurningLeft,
    StrafingRight, StrafingLeft,
    TurningUp, TurningDown
};

class ForceObject : public GraphicsObject {
protected:
    ForceObjectState state;

public:
	float magnitude;
	glm::vec3 normal;
    void SetState(ForceObjectState state) { this->state = state; }
    void Update(double elapsedSeconds);
    void UpdateNormal();
};