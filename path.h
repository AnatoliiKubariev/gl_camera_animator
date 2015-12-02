#pragma once

#include <glm.hpp>
#include <vector>

glm::mat4 look_at(const glm::vec3& eye, const glm::vec3& centre, const glm::vec3& up);
glm::mat4 pespective(const float fovx, const float aspect, const float z_far, const float z_near);

struct path_t
{
    glm::mat4 proj_matrix;
    std::vector<glm::mat4> view_matrix;
};

