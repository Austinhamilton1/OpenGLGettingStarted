#include "ForceObject.h"

void ForceObject::Update(double elapsedSeconds)
{
	switch (state) {
	case ForceObjectState::NotMoving:
		break;
	case ForceObjectState::MovingForward: {
		auto forward = -frame.GetZAxis();
		forward *= (2.0 * elapsedSeconds);
		frame.Move(forward);
		break;
	}
	case ForceObjectState::MovingBackward: {
		auto backward = frame.GetZAxis();
		backward *= (2.0 * elapsedSeconds);
		frame.Move(backward);
		break;
	}
	case ForceObjectState::MovingUp: {
		auto up = glm::vec3{ 0, 1, 0 };
		up *= (2.0 * elapsedSeconds);
		frame.Move(up);
		break;
	}
	case ForceObjectState::MovingDown: {
		auto down = glm::vec3{ 0, -1, 0 };
		down *= (2.0 * elapsedSeconds);
		frame.Move(down);
		break;
	}
	case ForceObjectState::TurningRight: {
		auto turnAngle = -90.0 * elapsedSeconds;
		frame.Rotate((float)turnAngle, frame.GetYAxis());
		break;
	}
	case ForceObjectState::TurningLeft: {
		auto turnAngle = 90.0 * elapsedSeconds;
		frame.Rotate((float)turnAngle, frame.GetYAxis());
		break;
	}
	case ForceObjectState::TurningDown: {
		auto turnAngle = -90.0 * elapsedSeconds;
		frame.Rotate((float)turnAngle, frame.GetZAxis());
		break;
	}
	case ForceObjectState::TurningUp: {
		auto turnAngle = 90.0 * elapsedSeconds;
		frame.Rotate((float)turnAngle, frame.GetZAxis());
		break;
	}
	case ForceObjectState::StrafingRight: {
		auto right = frame.GetXAxis();
		right *= (2.0 * elapsedSeconds);
		frame.Move(right);
		break;
	}
	case ForceObjectState::StrafingLeft: {
		auto left = -frame.GetXAxis();
		left *= (2.0 * elapsedSeconds);
		frame.Move(left);
		break;
	}
	}
	UpdateNormal();
}

void ForceObject::UpdateNormal() {
	normal = frame.GetXAxis();
}