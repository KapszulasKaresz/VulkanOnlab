#include "object.h"

glm::mat4 Object::getModelMatrix()
{
    glm::mat4 ret = glm::mat4(1.0f);

    for (Transformation* transformation : transformations) {
        ret *= transformation->getMatrix();
    }

    return ret;
}
