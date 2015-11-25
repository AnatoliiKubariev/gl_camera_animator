#pragma once

#include "stdafx.h"
#include <glm.hpp>
#include <mat4x4.hpp>
#include <vec4.hpp>


struct camera_t
{
    void move(const glm::vec3& direction, const float rand_value = 0);
    void rotate(const float angle, const float rand_value = 0);

    glm::vec4 eye;
    glm::vec4 centre;
    glm::vec4 up;
    
    //float x_angle;
    //float y_angle;
    float z_angle;
};