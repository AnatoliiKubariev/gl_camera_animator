#include "stdafx.h"
#include "camera.h"
#include "cubic.h"
#include <windows.h>
#include <stdlib.h>
#include <glut.h>
#include <vector>
#include <list>
#include <memory>

std::vector<std::vector<float>> colors
{
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f },
    { 1.0f, 1.0f, 0.0f },
    { 1.0f, 0.0f, 1.0f },
    { 0.0f, 1.0f, 1.0f }
};

std::vector<glm::vec3> objects
{
    { 0.0f, 0.0f, -4.0f },
    { 4.0f, 0.0f, -4.0f },
    { 8.0f, 0.0f, -4.0f },
    { 8.0f, 0.0f, 4.0f },
    { 4.0f, 0.0f, 4.0f },
    { 0.0f, 0.0f, 4.0f }
};

std::vector<cubic_t> calculate_cubic_pline(size_t n, const std::vector<glm::vec3>& v)
{
    std::vector<glm::vec3> gamma(n + 1);
    std::vector<glm::vec3> delta(n + 1);
    std::vector<glm::vec3> D(n + 1);
    size_t i;

    gamma[0] = glm::vec3(0.f, 0.f, 0.f);
    gamma[0].x = 1.f / 2.f;
    gamma[0].y = 1.f / 2.f;
    gamma[0].z = 1.f / 2.0;
    for (i = 1; i < n; ++i)
    {
        gamma[i] = (glm::vec3(glm::vec3(1.f, 1.f, 1.f) / (4.f * glm::vec3(1.f, 1.f, 1.f) - gamma[i - 1])));
    }
    gamma[n] = glm::vec3(1.f, 1.f, 1.f) / (2.f * glm::vec3(1.f, 1.f, 1.f) - gamma[i - 1]);

    delta[0] = 3.f * (v[1] - v[0]) * gamma[0];
    for (i = 1; i < n; ++i)
    {
        delta[i] = (3.f * (v[i + 1] - v[i - 1]) - delta[i - 1]) * gamma[n];
    }
    delta[n] = (3.f * (v[n] - v[n - 1]) - delta[n - 1]) * gamma[n];

    D[n] = delta[n];
    for (i = n - 1; i > 0; --i)
    {
        D[i] = delta[i] - gamma[i] * D[i + 1];
    }

    // a + b*s + c*s^2 +d*s^3
    std::vector<cubic_t> c(n);
    for (i = 0; i < n; ++i)
    {
        c[i] = cubic_t(
        { v[i]
        , D[i]
        , 3.f * (v[i + 1] - v[i]) - 2.f * D[i] - D[i + 1]
        , 2.f * (v[i] - v[i + 1]) + D[i] + D[i + 1] });
    }
    return c;
}

std::vector<camera_t> build_path(std::vector<camera_t> key_frames, const size_t path_step)
{
    std::vector<glm::vec3> eyes(key_frames.size());
    std::vector<glm::vec3> centres(key_frames.size());
    std::vector<glm::vec3> ups(key_frames.size());

    for (size_t i = 0; i < key_frames.size(); ++i)
    {
        eyes[i] = (key_frames[i].eye);
        centres[i] = (key_frames[i].centre);
        ups[i] = (key_frames[i].up);

    }

    std::vector<cubic_t> eye_cubic = calculate_cubic_pline(key_frames.size() - 1, eyes);
    std::vector<cubic_t> centre_cubic = calculate_cubic_pline(key_frames.size() - 1, centres);
    std::vector<cubic_t> up_cubic = calculate_cubic_pline(key_frames.size() - 1, ups);

    std::vector<camera_t> path;
    for (size_t i = 0; i < key_frames.size() - 1; ++i)
    {
        for (size_t j = 0; j < path_step; ++j)
        {
            float k = static_cast<float>(j) / static_cast<float>(path_step - 1);

            glm::vec3 eye = eye_cubic[i].get_point_on_spline(k);
            glm::vec3 centre = centre_cubic[i].get_point_on_spline(k);
            glm::vec3 up = up_cubic[i].get_point_on_spline(k);

            path.push_back({ eye, centre, up });
        }
    }
    return path;
}

std::vector<camera_t> key_frames =
{ { glm::vec3(0.f, 5.f, 0.f), glm::vec3(0.f, 0.f, -4.f), glm::vec3(0.f, 1.f, 0.f) }
, { glm::vec3(3.f, 9.f, 0.f), glm::vec3(4.f, 0.f, -4.f), glm::vec3(0.f, 1.f, 0.f) }
, { glm::vec3(10.f, 1.f, 0.f), glm::vec3(8.f, 0.f, -4.f), glm::vec3(0.f, 1.f, 0.f) }

, { glm::vec3(8.f, 8.f, 0.f), glm::vec3(3.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f) }

, { glm::vec3(8.f, 8.f, 0.f), glm::vec3(8.f, 0.f, 4.f), glm::vec3(0.f, 1.f, 0.f) }
, { glm::vec3(6.f, -3.f, 0.f), glm::vec3(4.f, 0.f, 4.f), glm::vec3(0.f, 1.f, 0.f) }
, { glm::vec3(-3.f, 8.f, 0.f), glm::vec3(0.f, 0.f, 4.f), glm::vec3(0.f, 1.f, 0.f) }

, { glm::vec3(0.f, 8.f, 0.f), glm::vec3(3.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f) }
, { glm::vec3(0.f, 5.f, 0.f), glm::vec3(0.f, 0.f, -4.f), glm::vec3(0.f, 1.f, 0.f) } };

const size_t path_step = 200;
const std::vector<camera_t> path = build_path(key_frames, path_step);
size_t step = 0;

size_t key_step = 0;

void draw_object(const glm::vec3& eye, const std::vector<float>& colors)
{
    glPushMatrix();
    glColor3d(colors[0], colors[1], colors[2]);
    glTranslated(eye.x, eye.y, eye.z);
    glutSolidTeapot(1);
    glPopMatrix();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt
        (path[step].eye.x, path[step].eye.y, path[step].eye.z
        , path[step].centre.x, path[step].centre.y, path[step].centre.z
        , path[step].up.x, path[step].up.y, path[step].up.z);
    if (step < key_step)
        ++step;
    if (step >= path.size())
    {
        step = 0;
        key_step = 0;
    }

    for (size_t i = 0; i < objects.size(); ++i)
    {
        draw_object(objects[i], colors[i]);
    }

    glutSwapBuffers();

    glutPostRedisplay();
}

void button_event(unsigned char key, int x, int y)
{
    key_step += path_step;
}

void resize(int width, int height)
{
    const float ar = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char *argv[])
{
    const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

    const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[] = { 100.0f };

    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Teapots invasion: press any key to move camera");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(button_event);

    glClearColor(0, 0, 0, 0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}