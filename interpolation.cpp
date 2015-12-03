#include "stdafx.h"
#include "interpolation.h"
#include <glm.hpp>

glm::mat4 lerp(const glm::mat4& n, const glm::mat4& n1, const float diff)
{
    return n + diff * (n1 - n);
}


namespace
{
    void extract_exis_angle(glm::mat4 const & mat, glm::vec3 & axis, float & angle);
    glm::mat4 axis_angle_matrix(glm::vec3 const & axis, float const angle);
    glm::mat4 extract_matrix_rotation(glm::mat4 const & mat);
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
        float c = cos(angle);
        float s = sin(angle);
        float t = 1.f - c;
        glm::vec3 n = normalize(axis);

        return glm::mat4(
            t * n.x * n.x + c, t * n.x * n.y + n.z * s, t * n.x * n.z - n.y * s, 0.f,
            t * n.x * n.y - n.z * s, t * n.y * n.y + c, t * n.y * n.z + n.x * s, 0.f,
            t * n.x * n.z + n.y * s, t * n.y * n.z - n.x * s, t * n.z * n.z + c, 0.f,
            0.f, 0.f, 0.f, 1.f
            );
    }
}