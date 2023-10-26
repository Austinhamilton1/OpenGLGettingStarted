#pragma once
#include "AbstractAnimation.h"
#include <glm/glm.hpp>
#include "ForceGenerator.h"

class GravityAnimation : public AbstractAnimation {
protected:
	Gravity gravForce;

public:
	GravityAnimation(const Gravity& g);
	virtual void Update(float duration);
};