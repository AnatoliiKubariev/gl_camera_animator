#include "stdafx.h"
#include "camera.h"
#include "actions.h"
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

std::vector<camera_t> build_path(std::vector<camera_t> key_frames, const size_t path_step)
{
    std::vector<camera_t> path;
    for (size_t i = 0; i < key_frames.size() - 1; ++i)
    {
        for (size_t j = 0; j < path_step; ++j)
        {
            //eye
            glm::vec3 diff = key_frames[i + 1].eye - key_frames[i].eye;
            diff *= static_cast<float>(j) / static_cast<float>(path_step);
            glm::vec3 eye = key_frames[i].eye + diff;

            //centre
            diff = key_frames[i + 1].centre - key_frames[i].centre;
            diff *= static_cast<float>(j) / static_cast<float>(path_step);
            glm::vec3 centre = key_frames[i].centre + diff;

            //up
            diff = key_frames[i + 1].up - key_frames[i].up;
            diff *= static_cast<float>(j) / static_cast<float>(path_step);
            glm::vec3 up = key_frames[i].up + diff;

            path.push_back({ eye, centre, up });
        }
    }
    return path;
}

std::vector<camera_t> key_frames =
{ { glm::vec3(0.f, 8.f, 0.f), glm::vec3(0.f, 0.f, -4.f), glm::vec3(0.f, 1.f, 0.f) }
, { glm::vec3(4.f, 8.f, 0.f), glm::vec3(4.f, 0.f, -4.f), glm::vec3(0.f, 1.f, 0.f) }
, { glm::vec3(8.f, 8.f, 0.f), glm::vec3(8.f, 0.f, -4.f), glm::vec3(0.f, 1.f, 0.f) }

, { glm::vec3(8.f, 8.f, 0.f), glm::vec3(12.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f) }

, { glm::vec3(8.f, 8.f, 0.f), glm::vec3(8.f, 0.f, 4.f), glm::vec3(0.f, 1.f, 0.f) }
, { glm::vec3(4.f, 8.f, 0.f), glm::vec3(4.f, 0.f, 4.f), glm::vec3(0.f, 1.f, 0.f) }
, { glm::vec3(0.f, 8.f, 0.f), glm::vec3(0.f, 0.f, 4.f), glm::vec3(0.f, 1.f, 0.f) }

, { glm::vec3(0.f, 8.f, 0.f), glm::vec3(-4.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f) }
, { glm::vec3(0.f, 8.f, 0.f), glm::vec3(0.f, 0.f, -4.f), glm::vec3(0.f, 1.f, 0.f) } };

const size_t path_step = 100;
const std::vector<camera_t> path = build_path(key_frames, path_step);
size_t step = 0;


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
    ++step;

    if (step >= path.size())
        step = 0;

    for (size_t i = 0; i < objects.size(); ++i)
    {
        draw_object(objects[i], colors[i]);
    }

    glutSwapBuffers();

    glutPostRedisplay();
}

void button_event(unsigned char key, int x, int y)
{
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