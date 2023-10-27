#include "ForceGenerator.h"
#include "RigidBody.h"

void Gravity::UpdateForce(RigidBody* body, float duration) {
	if (body->HasInfiniteMass()) return;

	body->AddForceAtBodyPoint(duration * gravAcc * body->GetMass(), glm::vec3(0.0f));
}