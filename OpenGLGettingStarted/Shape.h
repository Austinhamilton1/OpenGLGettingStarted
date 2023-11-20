#pragma once
#include <glm/glm.hpp>

class Shape {
public:
	static glm::mat3 Cuboid(float mass, float halfX, float halfY, float halfZ);

	static glm::mat3 Cylinder(float mass, float radius, float height);
};