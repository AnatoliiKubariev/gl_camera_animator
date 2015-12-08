#include "stdafx.h"
#include "interpolation.h"
#include <glm.hpp>

glm::mat4 lerp(const glm::mat4& n, const glm::mat4& n1, const float diff)
{
    return n + diff * (n1 - n);
}


namespace
{
    void extract_exis_angle(const glm::mat4& mat, glm::vec3& axis, float& angle);
    glm::mat4 axis_angle_matrix(const glm::vec3& axis, const float angle);
    glm::mat4 extract_matrix_rotation(const glm::mat4& mat);
}

glm::mat4 slerp(const glm::mat4& n, const glm::mat4& n1, const float diff)
{
    const glm::mat4 n_rot = extract_matrix_rotation(n);
    const glm::mat4 rotation_mat = n1 * transpose(n_rot);
    glm::vec3 axis;
    float angle;
    extract_exis_angle(rotation_mat, axis, angle);
    glm::mat4 res = axis_angle_matrix(axis, angle * diff) * n_rot;
    res[3][0] = n[3][0] + diff * (n1[3][0] - n[3][0]);
    res[3][1] = n[3][1] + diff * (n1[3][1] - n[3][1]);
    res[3][2] = n[3][2] + diff * (n1[3][2] - n[3][2]);
    return res;
}

namespace
{
    glm::mat4 extract_matrix_rotation(const glm::mat4& mat)
    {
        return glm::mat4(
            mat[0][0], mat[0][1], mat[0][2], 0.0,
            mat[1][0], mat[1][1], mat[1][2], 0.0,
            mat[2][0], mat[2][1], mat[2][2], 0.0,
            0.0, 0.0, 0.0, 1.0
            );
    }

    void extract_exis_angle(const glm::mat4& mat, glm::vec3& axis, float& angle)
    {
        float s = sqrt((mat[2][1] - mat[1][2]) * (mat[2][1] - mat[1][2])
            + (mat[2][0] - mat[0][2]) * (mat[2][0] - mat[0][2])
            + (mat[1][0] - mat[0][1]) * (mat[1][0] - mat[0][1]));
        if (glm::abs(s) < 0.001f)
            s = 1.0f;
        angle = acos((mat[0][0] + mat[1][1] + mat[2][2] - 1.f) / 2.f);
        axis.x = (mat[1][2] - mat[2][1]) / s;
        axis.y = (mat[2][0] - mat[0][2]) / s;
        axis.z = (mat[0][1] - mat[1][0]) / s;
    }

    glm::mat4 axis_angle_matrix(const glm::vec3& axis, const float angle)
    {
        const glm::vec3 temp_axis = glm::normalize(axis);
        float x = temp_axis.x;
        float y = temp_axis.y;
        float z = temp_axis.z;
        float c = cos(angle);
        float s = sin(angle);
        return glm::mat4
            ( c + (1 - c)*x*x, (1 - c)*x*y + s*z, (1 - c)*x*z - s*y, 0.f
            , (1 - c)*x*y - s*z, c + (1 - c)*y*y, (1 - c)*y*z + s*x, 0.f
            , (1 - c)*x*z + s*y, (1 - c)*y*z - s*x, c + (1 - c)*z*z, 0.f
            , 0.f, 0.f, 0.f, 1.f);
    }
}