#pragma once

#include <glm.hpp>
#include <vector>

struct path_t
{
    glm::mat4 proj_matrix;
    std::vector<glm::mat4> view_matrix;
};

