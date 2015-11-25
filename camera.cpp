#include "stdafx.h"
#include "camera.h"
#include <math.h>

void camera_t::move(const glm::vec3& direction, const float rand_value)
{
    glm::mat4 transformation =
    { 1.f, 0.f, 0.f, 0.f
    , 0.f, 1.f, 0.f, 0.f
    , 0.f, 0.f, 1.f, 0.f
    , direction.x, direction.y, direction.z, 1.f };

    eye = transformation * eye;
    centre = transformation * centre;

    eye.y += rand_value;
}

void camera_t::rotate(const float angle, const float rand_value)
{
    glm::mat4 transformation =
    { cos(angle), 0.f, -sin(angle), 0.f
    , 0.f, 1.f, 0.f, 0.f
    , sin(angle), 0.f, cos(angle), 0.f
    , 0.f, 0.f, 0.f, 1.f };

    glm::mat4 transformation1 =
    { 1.f, 0.f, 0.f, 0.f
    , 0.f, 1.f, 0.f, 0.f
    , 0.f, 0.f, 1.f, 0.f
    , -eye.x, 0.f, 0.f, 1.f };
    glm::mat4 transformation2 =
    { 1.f, 0.f, 0.f, 0.f
    , 0.f, 1.f, 0.f, 0.f
    , 0.f, 0.f, 1.f, 0.f
    , eye.x, 0.f, 0.f, 1.f };


    centre = eye + transformation * (centre - eye);
    eye.y += rand_value;
}