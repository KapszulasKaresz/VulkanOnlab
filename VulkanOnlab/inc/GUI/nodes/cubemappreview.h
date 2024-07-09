#pragma once
#include <string>
#include <vulkan/texture/cubemap.h>

class CubeMapPreview {
public:
	CubeMapPreview() {}

	void open(Cubemap* cubemap);
	void close();
	void draw();

private:
	Cubemap* cubemap = nullptr;
	bool display = false;
};