#include "stdafx.h"
#include "camera.h"
#include <math.h>

void camera_t::move(const glm::vec3& direction)
{
    glm::mat4 transformation =
    { 1.f, 0.f, 0.f, 0.f
    , 0.f, 1.f, 0.f, 0.f
    , 0.f, 0.f, 1.f, 0.f
    , direction.x, direction.y, direction.z, 1.f };

    eye = transformation * eye;
    centre = transformation * centre;
}

void camera_t::rotate(const float angle, const glm::vec3& axis, const rotation_type type)
{
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float c = cos(angle);
    float s = sin(angle);
    glm::mat4 transformation =
    { c + (1 - c)*x*x, (1 - c)*x*y - s*z, (1 - c)*x*z + s*y, 0.f
    , (1 - c)*x*y + s*z, c + (1 - c)*y*y, (1 - c)*y*z - s*x, 0.f
    , (1 - c)*x*z - s*y, (1 - c)*y*z + s*x, c + (1 - c)*z*z, 0.f
    , 0.f, 0.f, 0.f, 1.f };


    if (type == rotation_type::centre)
        centre = eye + transformation * (centre - eye);
    else
    if (type == rotation_type::eye)
        eye = centre + transformation * (eye - centre);
}

//void rotate(const float angle, const glm::vec4& axis, glm::vec4& point)
//{
//    float x = axis.x;
//    float y = axis.y;
//    float z = axis.z;
//    float c = cos(angle);
//    float s = sin(angle);
//    glm::mat4 transformation =
//    { c + (1 - c)*x*x, (1 - c)*x*y - s*z, (1 - c)*x*z + s*y, 0.f
//    , (1 - c)*x*y + s*z, c + (1 - c)*y*y, (1 - c)*y*z - s*x, 0.f
//    , (1 - c)*x*z - s*y, (1 - c)*y*z + s*x, c + (1 - c)*z*z, 0.f
//    , 0.f, 0.f, 0.f, 1.f };
//
//
//
//    point = axis + transformation * (point - axis);
//}
