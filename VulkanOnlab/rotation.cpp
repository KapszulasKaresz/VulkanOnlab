#include "rotation.h"

glm::mat4 Rotation::getMatrix()
{
    return glm::rotate(glm::mat4(1.0f), amount, rotationAxis);
}
