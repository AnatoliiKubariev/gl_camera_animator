#include "stdafx.h"
#include "path.h"
#include <glm.hpp>

glm::mat4 interpolate(glm::mat4& n, glm::mat4& n1, const float diff)
{
    glm::mat4 res;



    return res;
}


glm::mat4 look_at(const glm::vec3& eye, const glm::vec3& centre, const glm::vec3& up)
{
    const glm::vec3 z_axis = glm::normalize(eye - centre);

    const glm::vec3 x_axis = glm::normalize(glm::cross(up, z_axis));

    const glm::vec3 y_axis = glm::cross(x_axis, z_axis);

    glm::mat4 view_matrix;
    view_matrix[0].x = x_axis.x;
    view_matrix[1].x = x_axis.y;
    view_matrix[2].x = x_axis.z;
    view_matrix[3].x = -glm::dot(x_axis, eye);

    view_matrix[0].y = y_axis.x;
    view_matrix[1].y = y_axis.y;
    view_matrix[2].y = y_axis.z;
    view_matrix[3].y = -glm::dot(y_axis, eye);

    view_matrix[0].z = z_axis.x;
    view_matrix[1].z = z_axis.y;
    view_matrix[2].z = z_axis.z;
    view_matrix[3].z = -glm::dot(z_axis, eye);

    view_matrix[0].w = 0.f;
    view_matrix[1].w = 0.f;
    view_matrix[2].w = 0.f;
    view_matrix[3].w = 1.f;

    return view_matrix;
}

glm::mat4 pespective(const float fovx, const float aspect, const float z_far, const float z_near)
{
    float e = 1.0f / tanf((fovx * 3.1415 / 180) / 2.0f);
    float aspect_inv = 1.f / aspect;
    float fovy = 2.0f * atanf(aspect_inv / e);;
    float x_scale = 1.0f / tanf(0.5f * fovy);
    float y_scale = x_scale / aspect_inv;

    glm::mat4 proj_matrix;
    proj_matrix[0].x = x_scale;
    proj_matrix[0].y = 0.f;
    proj_matrix[0].z = 0.f;
    proj_matrix[0].w = 0.f;

    proj_matrix[1].x = 0.f;
    proj_matrix[1].y = y_scale;
    proj_matrix[1].z = 0.f;
    proj_matrix[1].w = 0.f;

    proj_matrix[2].x = 0.f;
    proj_matrix[2].y = 0.f;
    proj_matrix[2].z = (z_far + z_near) / (z_near - z_far);
    proj_matrix[2].w = -1.f;

    proj_matrix[3].x = 0.f;
    proj_matrix[3].y = 0.f;
    proj_matrix[3].z = (2.f * z_far * z_near) / (z_near - z_far);
    proj_matrix[3].w = 1.f;

    return proj_matrix;
}
