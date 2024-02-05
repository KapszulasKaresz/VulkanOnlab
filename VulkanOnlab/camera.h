#pragma once
#include <glm/glm.hpp>


struct Camera
{
	glm::vec3 wEye, wLookat, wVup;
	float fov, asp, fp, np;

	Camera() {}

	glm::mat4 getProj();
	glm::mat4 getView();
};