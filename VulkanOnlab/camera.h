#pragma once
#include <glm/glm.hpp>


struct Camera
{
	glm::vec3 wEye, wForward, wVup;
	float fov, asp, fp, np;
	float pitchAmount = 0;
	float yawAmount = 0;
	glm::mat4 yawMatrix = glm::mat4(1.0f);
	glm::mat4 pitchMatrix= glm::mat4(1.0f);;


	Camera() {}

	glm::mat4 getProj();
	glm::mat4 getView();
	void pitchandyaw(float pitchDegree, float yawDegree);
};