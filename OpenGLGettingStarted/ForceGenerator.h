#pragma once
#include <glm/glm.hpp>
#include "RigidBody.h"

class ForceGenerator {
public:
	virtual void updateForce(RigidBody* body, float duration) = 0;
};

class Gravity : public ForceGenerator {
	glm::vec3 gravAcc;

public:
	Gravity() : gravAcc(glm::vec3(0.0f)) {};
	Gravity(const glm::vec3& gravAcc) : gravAcc(gravAcc) {};
	Gravity(float x, float y, float z) : gravAcc({x, y, z}) {};
	virtual void updateForce(RigidBody* body, float duration);
	void SetAcceleration(glm::vec3& gravAcc) { this->gravAcc = gravAcc; };
	void SetAcceleration(float x, float y, float z) { this->gravAcc = glm::vec3(x, y, z); };
};