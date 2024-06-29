#include "vulkan/transform/scale.h"

glm::mat4 Scale::getMatrix()
{
    return glm::scale(glm::mat4(1.0f), scale);
}
