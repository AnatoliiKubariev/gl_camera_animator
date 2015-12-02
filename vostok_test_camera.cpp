#include "stdafx.h"
#include "path.h"
#include "key_frame.h"
#include <glut.h>
#include <gtx\matrix_interpolation.hpp>
#include <vector>

glm::mat4 interpolate(const glm::mat4 const& n, const glm::mat4 const& n1, const float diff);
glm::mat4 look_at(const glm::vec3& eye, const glm::vec3& centre, const glm::vec3& up);
glm::mat4 perspective(const float fovx, const float aspect, const float z_far, const float z_near);

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

path_t build_linear_path(const std::vector<key_frame_t>& key_frames, const size_t path_step)
{
    path_t path;
    for (size_t i = 0; i < key_frames.size() - 1; ++i)
    {
        glm::mat4 n = look_at(key_frames[i].eye, key_frames[i].centre, key_frames[i].up);
        glm::mat4 n1 = look_at(key_frames[i + 1].eye, key_frames[i + 1].centre, key_frames[i + 1].up);

        for (float j = 0; j < 1; j += 1.f / path_step)
        {
            path.view_matrix.push_back(interpolate(n, n1, j));
        }
    }
    return path;
}

enum intertolation_type { linear, cubic };

path_t build_path(intertolation_type type, const std::vector<key_frame_t>& key_frames, const size_t path_step)
{
    switch (type)
    {
    case linear:
        return build_linear_path(key_frames, path_step);
    //case cubic:
    //    return build_cubic_path(key_frames, path_step);
    }
}

const std::vector<key_frame_t> key_frames =
{ { glm::vec3(0.f, 5.f, 0.f), glm::vec3(0.f, 0.f, -4.f), glm::vec3(0.f, -1.f, 0.f) }
, { glm::vec3(3.f, 9.f, 0.f), glm::vec3(4.f, 0.f, -4.f), glm::vec3(0.f, -1.f, 0.f) }
, { glm::vec3(10.f, 1.f, 0.f), glm::vec3(8.f, 0.f, -4.f), glm::vec3(0.f, -1.f, 0.f) }

, { glm::vec3(8.f, 8.f, 0.f), glm::vec3(3.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f) }

, { glm::vec3(8.f, 8.f, 0.f), glm::vec3(8.f, 0.f, 4.f), glm::vec3(0.f, -1.f, 0.f) }
, { glm::vec3(6.f, -3.f, 0.f), glm::vec3(4.f, 0.f, 4.f), glm::vec3(0.f, -1.f, 0.f) }
, { glm::vec3(-3.f, 8.f, 0.f), glm::vec3(0.f, 0.f, 4.f), glm::vec3(0.f, -1.f, 0.f) }

, { glm::vec3(0.f, 8.f, 0.f), glm::vec3(3.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f) }
, { glm::vec3(0.f, 5.f, 0.f), glm::vec3(0.f, 0.f, -4.f), glm::vec3(0.f, -1.f, 0.f) } };

const intertolation_type type = linear;
const size_t path_step = 200;
path_t path = build_path(type, key_frames, path_step);
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

    for (size_t i = 0; i < objects.size(); ++i)
    {
        draw_object(objects[i], colors[i]);
    }

    //glMatrixMode(GL_PROJECTION);
    //glLoadMatrixf(path.proj_matrix[0].data);
    //glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(path.view_matrix[step][0].data);

    if (step < key_step)
        ++step;
    if (step >= path.view_matrix.size())
    {
        step = 0;
        key_step = 0;
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

    //path.proj_matrix = perspective(90.f, ar, 1.f, 20.f);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    //glLoadMatrixf(path.proj_matrix[0].data);
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