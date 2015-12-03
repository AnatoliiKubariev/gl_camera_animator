#pragma once

#include <glm.hpp>

glm::mat4 lerp(const glm::mat4& n, const glm::mat4& n1, const float diff);
glm::mat4 slerp(const glm::mat4& n, const glm::mat4& n1, const float diff);