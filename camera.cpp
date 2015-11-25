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

void camera_t::rotate(const float angle)
{
    glm::mat4 transformation =
    { cos(angle), 0.f, -sin(angle), 0.f
    , 0.f, 1.f, 0.f, 0.f
    , sin(angle), 0.f, cos(angle), 0.f
    , 0.f, 0.f, 0.f, 1.f };

    centre = eye + transformation * (centre - eye);
}