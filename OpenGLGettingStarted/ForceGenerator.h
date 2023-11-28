#pragma once
#include <glm/glm.hpp>

class RigidBody;

class ForceGenerator {
public:
	ForceGenerator() {};
	virtual ~ForceGenerator() {};
	virtual void UpdateForce(RigidBody* body, float duration) = 0;
};

class Force : public ForceGenerator {
public:
	glm::vec3 force;
	glm::vec3 pos;

	Force() : force(glm::vec3(0.0f)), pos(glm::vec3(0.0f)) {};
	Force(const glm::vec3& force, const glm::vec3& pos) : force(force), pos(pos) {};
	virtual void UpdateForce(RigidBody* body, float duration);
	void SetAcceleration(glm::vec3& force) { this->force = force; };
	void SetAcceleration(float x, float y, float z) { this->force = glm::vec3(x, y, z); };
	void SetPosition(glm::vec3& pos) { this->pos = pos; };
	void SetPosition(float x, float y, float z) { this->pos = glm::vec3(x, y, z); };
};

class Gravity : public ForceGenerator {
	glm::vec3 gravAcc;

public:
	Gravity() : gravAcc(glm::vec3(0.0f)) {};
	Gravity(const glm::vec3& gravAcc) : gravAcc(gravAcc) {};
	Gravity(float x, float y, float z) : gravAcc({x, y, z}) {};
	virtual void UpdateForce(RigidBody* body, float duration);
	void SetAcceleration(glm::vec3& gravAcc) { this->gravAcc = gravAcc; };
	void SetAcceleration(float x, float y, float z) { this->gravAcc = glm::vec3(x, y, z); };
};