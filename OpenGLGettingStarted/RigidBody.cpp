#include "RigidBody.h"
#include "ForceGenerator.h"

RigidBody::RigidBody(const float mass, const glm::vec3& pos, const glm::mat3& shape) : linearVelocity(glm::vec3(0.0f)),
	angularVelocity(glm::vec3(0.0f)), linearAcceleration(glm::vec3(0.0f)), 
	angularAcceleration(glm::vec3(0.0f)),forceAcc(glm::vec3(0.0f)),
	torqueAcc(glm::vec3(0.0f)), linearDamping(0.999f), angularDamping(0.999f)
{
	if (mass <= 0.0f)
		inverseMass = 0;
	inverseMass = 1 / mass;
	inverseInertiaTensor = glm::inverse(shape);
	frame = ReferenceFrame();
	frame.SetPosition(pos);
};

RigidBody::RigidBody() : RigidBody(1.0f, glm::vec3(0, 0, 0), Shape::Cylinder(1.0f, 1.0f, 1.0f)) {};

void RigidBody::CaculateDerivedData() {
	frame.orientation[0] = glm::normalize(frame.orientation[0]);
	frame.orientation[1] = glm::normalize(frame.orientation[1]);
	frame.orientation[2] = glm::normalize(frame.orientation[2]);
};

void RigidBody::Integrate(float duration) {
	linearAcceleration = (inverseMass * forceAcc);
	glm::mat3 mat(frame.orientation);
	glm::mat3 worldInertiaTensor(1.0f);
	worldInertiaTensor *= (mat * inverseInertiaTensor * glm::transpose(mat));
	angularAcceleration = (worldInertiaTensor * torqueAcc);
	
	linearVelocity += (duration * linearAcceleration);
	angularVelocity += (duration * angularAcceleration);

	linearVelocity *= powf(linearDamping, duration);
	angularVelocity *= powf(angularDamping, duration);

	glm::vec3 worldVelocity = linearVelocity * mat;
	frame.MoveWorld(duration * worldVelocity);

	frame.Rotate(glm::degrees(duration * angularVelocity.x), glm::vec3(1.0f, 0, 0));
	frame.Rotate(glm::degrees(duration * angularVelocity.y), glm::vec3(0, 1.0f, 0));
	frame.Rotate(glm::degrees(duration * angularVelocity.z), glm::vec3(0, 0, 1.0f));

	//frame.Rotate(duration * angularVelocity.x, glm::vec3(1.0f, 0, 0));
	//frame.Rotate(duration * angularVelocity.y, glm::vec3(0, 1.0f, 0));
	//frame.Rotate(duration * angularVelocity.z, glm::vec3(0, 0, 1.0f));

	CaculateDerivedData();
	ClearAccumulators();
};

void RigidBody::AddForceAtBodyPoint(const glm::vec3& force, const glm::vec3& point) {
	glm::mat3 mat(frame.orientation);
	glm::vec3 worldPoint = mat * frame.orientation[3];
	worldPoint += point;
	AddForceAtPoint(force, worldPoint);
}

void RigidBody::Update(double duration)
{
	for (auto& it : constForces) {
		it.second->UpdateForce(this, (float)duration);
	}
	Integrate((float)duration);
}

void RigidBody::AddForceAtPoint(const glm::vec3& force, const glm::vec3& point) {
	glm::vec3 pt = point;
	glm::mat3 mat(frame.orientation);
	glm::vec3 worldPos = mat * frame.orientation[3];
	pt -= worldPos;

	forceAcc += force;
	torqueAcc += glm::cross(pt, force);
};

void RigidBody::SetShape(const glm::mat3& shape) {
	inverseInertiaTensor = glm::inverse(shape);
};