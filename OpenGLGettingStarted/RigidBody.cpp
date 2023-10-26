#include "RigidBody.h"

RigidBody::RigidBody(const float mass, const glm::vec3& pos) : linearVelocity(glm::vec3(0.0f)),
	angularVelocity(glm::vec3(0.0f)), linearAcceleration(glm::vec3(0.0f)), forceAcc(glm::vec3(0.0f)),
	torqueAcc(glm::vec3(0.0f)), transform(glm::mat4(0.0f)), linearDamping(0.99f), angularDamping(0.999f)
{
	if (mass <= 0.0f)
		inverseMass = 0;
	inverseMass = 1 / mass;
	frame = ReferenceFrame();
	frame.SetPosition(pos);
};

void RigidBody::CaculateDerivedData() {
	frame.orientation[0] = glm::normalize(frame.orientation[0]);
	frame.orientation[1] = glm::normalize(frame.orientation[1]);
	frame.orientation[2] = glm::normalize(frame.orientation[2]);
};

void RigidBody::Integrate(float duration) {
	glm::vec3 lastFrameAcceleration = linearAcceleration;
	lastFrameAcceleration += (inverseMass * forceAcc);

	linearVelocity += (duration * lastFrameAcceleration);

	linearVelocity *= powf(linearDamping, duration);

	glm::mat3 mat(frame.orientation);
	glm::vec3 worldVelocity = mat * linearVelocity;
	//frame.orientation[3] += worldVelocity;
	frame.MoveWorld(glm::vec3(0.001f, 0.0f, 0.0f));

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