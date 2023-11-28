#include "ForceObject.h"

void ForceObject::Update(double elapsedSeconds)
{
	switch (state) {
	case ForceObjectState::NotMoving:
		break;
	case ForceObjectState::MovingForward: {
		auto forward = -m_camera->frame.GetZAxis();
		forward *= (2.0 * elapsedSeconds);
		frame.Move(forward);
		break;
	}
	case ForceObjectState::MovingBackward: {
		auto backward = m_camera->frame.GetZAxis();
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
		frame.Rotate((float)turnAngle, m_camera->frame.GetYAxis());
		break;
	}
	case ForceObjectState::TurningLeft: {
		auto turnAngle = 90.0 * elapsedSeconds;
		frame.Rotate((float)turnAngle, m_camera->frame.GetYAxis());
		break;
	}
	case ForceObjectState::TurningDown: {
		auto turnAngle = -90.0 * elapsedSeconds;
		frame.Rotate((float)turnAngle, m_camera->frame.GetZAxis());
		break;
	}
	case ForceObjectState::TurningUp: {
		auto turnAngle = 90.0 * elapsedSeconds;
		frame.Rotate((float)turnAngle, m_camera->frame.GetZAxis());
		break;
	}
	case ForceObjectState::StrafingRight: {
		auto right = m_camera->frame.GetXAxis();
		right *= (2.0 * elapsedSeconds);
		frame.Move(right);
		break;
	}
	case ForceObjectState::StrafingLeft: {
		auto left = -m_camera->frame.GetXAxis();
		left *= (2.0 * elapsedSeconds);
		frame.Move(left);
		break;
	}
	case ForceObjectState::Growing: {
		if(magnitude <= 100)
			magnitude += (2.0 * elapsedSeconds);
		break;
	}
	case ForceObjectState::Shrinking: {
		if(magnitude > 2.0 * elapsedSeconds)
			magnitude -= (2.0 * elapsedSeconds);
		break;
	}
	}
	UpdateNormal();
}

void ForceObject::UpdateNormal() {
	normal = frame.GetXAxis();
}