#include "Shape.h"

glm::mat3 Shape::Cuboid(float mass, float halfX, float halfY, float halfZ) {
	float xVal = (1 / 12) * mass * (halfY * halfY + halfZ * halfZ);
	float yVal = (1 / 12) * mass * (halfX * halfX + halfZ * halfZ);
	float zVal = (1 / 12) * mass * (halfX * halfX + halfY * halfY);
	glm::vec3 x(xVal, 0, 0);
	glm::vec3 y(0, yVal, 0);
	glm::vec3 z(0, 0, zVal);
	glm::mat3 shape(x, y, z);
	return shape;
};

glm::mat3 Shape::Cylinder(float mass, float radius, float height) {
	float xVal = (1.0f / 12) * mass * height * height + (1.0f / 4) * mass * radius * radius;
	float yVal = (1.0f / 2) * mass * radius * radius;
	float zVal = (1.0f / 12) * mass * height * height + (1.0f / 4) * mass * radius * radius;
	glm::vec3 x(xVal, 0, 0);
	glm::vec3 y(0, yVal, 0);
	glm::vec3 z(0, 0, zVal);
	glm::mat3 shape(x, y, z);
	return shape;
};