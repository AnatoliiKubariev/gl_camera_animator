#pragma once

#include "stdafx.h"
#include <glm.hpp>

enum rotation_type{ eye, centre };
struct camera_t
{
    void move(const glm::vec3& direction);
    void rotate(const float angle, const glm::vec3& axis, const rotation_type type);

    glm::vec4 eye;
    glm::vec4 centre;
    glm::vec4 up;
    
    float angle;
};