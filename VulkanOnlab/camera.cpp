#include "camera.h"
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::getView()
{   
   return glm::lookAt(wEye, wForward+ wEye, wVup);
}

void Camera::pitchandyaw(float pitchDegree, float yawDegree)
{

    pitchMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(pitchDegree), -glm::normalize(glm::cross(wVup, wForward)));

    yawMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(yawDegree), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 rot = pitchMatrix * yawMatrix;

    wForward = glm::vec4(wForward, 0.0f) * rot;
    wForward = glm::normalize(wForward);

    wVup = glm::vec4(wVup, 0.0f) * rot;
    wVup = glm::normalize(wVup);
}


glm::mat4 Camera::getProj()
{
    return glm::perspective(fov, asp, np, fp);
}
