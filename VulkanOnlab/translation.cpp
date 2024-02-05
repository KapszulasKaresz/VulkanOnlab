#include "translation.h"

glm::mat4 Translation::getMatrix()
{
    return glm::translate(glm::mat4(1.0f), translation);
}
