#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <string>
#include "GraphicsObject.h"
#include "ReferenceFrame.h"
#include "ForceGenerator.h"
#include "Shape.h"

class RigidBody : public GraphicsObject {
public:
	std::unordered_map<std::string, std::unique_ptr<ForceGenerator>> constForces;

protected:
	float inverseMass;
	float linearDamping;
	float angularDamping;

	glm::vec3 linearVelocity;
	glm::vec3 angularVelocity;

	glm::vec3 linearAcceleration;
	glm::vec3 angularAcceleration;

	glm::vec3 forceAcc;
	glm::vec3 torqueAcc;

	glm::mat3 inverseInertiaTensor;

public:
	RigidBody();
	RigidBody(const float mass, const glm::vec3& position, const glm::mat3& shape);
	virtual ~RigidBody() {};

	void CaculateDerivedData();

	void Integrate(float duration);

	void AddForceAtPoint(const glm::vec3& force, const glm::vec3& point);
	void AddForceAtBodyPoint(const glm::vec3& force, const glm::vec3& point);
	
	void ClearAccumulators() {
		forceAcc = glm::vec3(0.0f);
		torqueAcc = glm::vec3(0.0f);
	};
	bool HasInfiniteMass() { return (inverseMass <= 0); };
	float GetMass() { return 1 / inverseMass; };
	void SetLinearAcceleration(const glm::vec3& acc) { linearAcceleration = acc; };
	void SetLinearAcceleration(float x, float y, float z) { SetLinearAcceleration( glm::vec3(x, y, z)); };
	void Update(double duration);
	//void SetLinearVelocity(const glm::vec3& vel) { linearVelocity = vel; };
	void SetMass(float mass) { inverseMass = (mass <= 0.0f) ? 0 : 1 / mass; };
	void SetShape(const glm::mat3& shape);
	bool IsRigidBody() { return true; };
};