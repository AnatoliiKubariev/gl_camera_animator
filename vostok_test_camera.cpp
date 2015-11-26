
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

size_t camera_location = 0;
camera_t camera = { glm::vec4(0.f, 8.f, 0.f, 1.f)
, glm::vec4(objects[0].x, objects[0].y, objects[0].z, 1.f)
, glm::vec4(0.f, 1.f, 0.f, 0.f)
, 0.f };

const glm::vec3 ox(1.f, 0.f, 0.f);
const glm::vec3 oy(0.f, 1.f, 0.f);
const glm::vec3 oz(0.f, 0.f, 1.f);
const glm::vec3 xy(1.f, 1.f, 0.f);
const glm::vec3 xz(1.f, 0.f, 1.f);
const glm::vec3 yz(0.f, 1.f, 1.f);
const glm::vec3 xyz(1.f, 1.f, 1.f);

using actions_t = std::vector<std::unique_ptr<camera_action_t>>;
actions_t generate_action()
{
    actions_t actions;

    actions.push_back(std::make_unique<rotate_t>(camera, 10.f, xyz, rotation_type::eye));
    actions.push_back(std::make_unique<stand_t>());
    actions.push_back(std::make_unique<rotate_t>(camera, -10.f, xyz, rotation_type::eye));
    actions.push_back(std::make_unique<move_t>(camera, objects[1]));
    actions.push_back(std::make_unique<rotate_t>(camera, -45.f, ox, rotation_type::eye));
    actions.push_back(std::make_unique<stand_t>());
    actions.push_back(std::make_unique<rotate_t>(camera, 45.f, ox, rotation_type::eye));
    actions.push_back(std::make_unique<move_t>(camera, objects[2]));
    actions.push_back(std::make_unique<rotate_t>(camera, 70.f, oy, rotation_type::eye));
    actions.push_back(std::make_unique<stand_t>());
    actions.push_back(std::make_unique<rotate_t>(camera, -70.f, oy, rotation_type::eye));
    actions.push_back(std::make_unique<rotate_t>(camera, -180.f, oy, rotation_type::centre));
    actions.push_back(std::make_unique<rotate_t>(camera, 50.f, oz, rotation_type::eye));
    actions.push_back(std::make_unique<stand_t>());
    actions.push_back(std::make_unique<rotate_t>(camera, -50.f, oz, rotation_type::eye));
    actions.push_back(std::make_unique<move_t>(camera, objects[4]));
    actions.push_back(std::make_unique<rotate_t>(camera, -30.f, xy, rotation_type::eye));
    actions.push_back(std::make_unique<stand_t>());
    actions.push_back(std::make_unique<rotate_t>(camera, 30.f, xy, rotation_type::eye));
    actions.push_back(std::make_unique<move_t>(camera, objects[5]));
    actions.push_back(std::make_unique<rotate_t>(camera, -60.f, yz, rotation_type::eye));
    actions.push_back(std::make_unique<stand_t>());
    actions.push_back(std::make_unique<rotate_t>(camera, 60.f, yz, rotation_type::eye));
    actions.push_back(std::make_unique<rotate_t>(camera, -180.f, oy, rotation_type::centre));

    return actions;
}


actions_t actions = generate_action();
actions_t::iterator cur_action = actions.begin();
actions_t::iterator next_action(actions_t& actions, actions_t::iterator ait)
{
    ++ait;
    if (ait == actions.end())
        return actions.begin();
    return ait;
}


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

    if (cur_action->get()->event())
        cur_action = next_action(actions, cur_action);

    gluLookAt
        (camera.eye.x, camera.eye.y, camera.eye.z
        , camera.centre.x, camera.centre.y, camera.centre.z
        , camera.up.x, camera.up.y, camera.up.z);

    //draw objects
    for (size_t i = 0; i < objects.size(); ++i)
    {
        draw_object(objects[i], colors[i]);
    }

    glutSwapBuffers();

    glutPostRedisplay();
}

void button_event(unsigned char key, int x, int y)
{
    cur_action->get()->button_event(key);
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

const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

// Program entry point /

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Name");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(button_event);
    //glutMouseFunc(my_func);

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
    //while (true)
    //{
    //    glutMainLoop();
    //}

    return EXIT_SUCCESS;
}