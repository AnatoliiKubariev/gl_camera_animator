#pragma once

#include "stdafx.h"
#include <glm.hpp>

struct camera_t
{
    glm::vec3 eye;
    glm::vec3 centre;
    glm::vec3 up;
};