#pragma once
#include "gObject.h"
#include <glm/vec2.hpp> // glm::vec3
class Plane : public gObject {
public:

	Plane();

	// Creates a sphere  
	Plane(float _x, float _y, float _z, float _dX, float _dY );
	~Plane(void);

	glm::vec2* getDimensions(){ return &dimensions; }

private:

	glm::vec2 dimensions;
	void clean();
};