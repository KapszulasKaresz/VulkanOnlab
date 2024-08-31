#pragma once
#ifndef GLM_FORCE_RADIANS
	#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

struct Transformation {
	int id;
	static int rollingId;

	Transformation() { id = rollingId++; }
	virtual glm::mat4 getMatrix() = 0;
	virtual std::string getName() = 0;

	bool operator==(Transformation& other) { return id == other.id; }
};

