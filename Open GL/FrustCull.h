#pragma once
#include "glm/glm.hpp"
class FrustCull
{
private:
	struct Plane
	{
		glm::vec3 Normal;
		glm::vec3 Point;
	};
	Plane Front;
	Plane Back;
	Plane Top;
	Plane Bottom;
	Plane Left;
	Plane Right;

public:
	FrustCull(glm::vec3 front,glm::vec3 pos, float near, float far);
	bool CheckInside(glm::vec3 point);
	
};

