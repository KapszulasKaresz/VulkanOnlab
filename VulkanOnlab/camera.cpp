#include "camera.h"
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::getView()
{
   return glm::lookAt(wEye, wLookat, wVup);
}

glm::mat4 Camera::getProj()
{
    return glm::perspective(fov, asp, np, fp);
}
