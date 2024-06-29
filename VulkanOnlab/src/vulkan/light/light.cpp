#include "vulkan/light/light.h"

bool Light::operator==(Light& other)
{
    return pos == other.pos && La == other.La && Le == other.Le;
}
