#include "FrustCull.h"

FrustCull::FrustCull(glm::vec3 front, glm::vec3 pos, float near, float far)
{
	front = glm::normalize(front);
	Front.Normal = front; Front.Point = pos + front * near;
	Back.Normal = -front; Back.Point = pos + front * far;

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(front, up));
	up= glm::normalize(glm::cross(right, front));

	float halfV = glm::tan(glm::radians(45.0f / 2.0f)) * far; //aspect ratio=1

	Top.Normal = glm::normalize(glm::cross(Back.Point + up * halfV, right ));
	Bottom.Normal = glm::normalize(glm::cross( right, Back.Point - up * halfV));
	Right.Normal = glm::normalize(glm::cross(up,Back.Point+right*halfV));
	Left.Normal =  glm::normalize(glm::cross(Back.Point - right * halfV, up));

	Top.Point = pos;
	Bottom.Point = pos;
	Right.Point = pos;
	Left.Point = pos;
}

bool FrustCull::CheckInside(glm::vec3 point)
{
	if (glm::dot(point - Front.Point, Front.Normal) < 0.0f) return false;
	if (glm::dot(point - Back.Point, Back.Normal) < 0.0f) return false;
	if (glm::dot(point - Top.Point, Top.Normal) < 0.0f) return false;
	if (glm::dot(point - Bottom.Point, Bottom.Normal) < 0.0f) return false;
	if (glm::dot(point - Right.Point, Right.Normal) < 0.0f) return false;
	if (glm::dot(point - Left.Point, Left.Normal) < 0.0f) return false;
	return true;
}
