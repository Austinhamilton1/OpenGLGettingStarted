#include "ForceGenerator.h"
#include "RigidBody.h"

void Gravity::updateForce(RigidBody* body, float duration) {
	if (body->HasInfiniteMass()) return;

	body->AddForceAtBodyPoint(gravAcc * body->GetMass(), glm::vec3(0.0f));
}