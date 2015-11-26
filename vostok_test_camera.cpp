
#include "stdafx.h"
#include "camera.h"
#include <windows.h>
#include <stdlib.h>
#include <glut.h>
#include <vector>
#include <list>
#include <math.h>

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

enum action_type{ stand, move, rotate };

struct action
{
    action() :act(stand){}
    action(float angle, const glm::vec3& axis, const rotation_type type) :act(rotate), angle(angle), axis(axis), type(type){}
    action(const glm::vec3& p) :act(move), angle(0), dest(p){}
    action_type act;
    glm::vec3 dest;
    float angle;
    glm::vec3 axis;
    rotation_type type;
};
using actions_t = std::list<action>;
actions_t actions = { action(10.f, glm::vec3(1.f, 1.f, 1.f), rotation_type::eye), action(), action(-10.f, glm::vec3(1.f, 1.f, 1.f), rotation_type::eye)
                    , action(objects[1])
                    , action(-45.f, glm::vec3(1.f, 0.f, 0.f), rotation_type::eye), action(), action(45.f, glm::vec3(1.f, 0.f, 0.f), rotation_type::eye)
                    , action(objects[2])
                    , action(70.f, glm::vec3(0.f, 1.f, 0.f), rotation_type::eye), action(), action(-70.f, glm::vec3(0.f, 1.f, 0.f), rotation_type::eye)
                    , action(-180.f, glm::vec3(0.f, 1.f, 0.f), rotation_type::centre)
                    , action(50.f, glm::vec3(0.f, 0.f, 1.f), rotation_type::eye), action(), action(-50.f, glm::vec3(0.f, 0.f, 1.f), rotation_type::eye)
                    , action(objects[4])
                    , action(-30.f, glm::vec3(1.f, 1.f, 0.f), rotation_type::eye), action(), action(30.f, glm::vec3(1.f, 1.f, 0.f), rotation_type::eye)
                    , action(objects[5])
                    , action(-60.f, glm::vec3(0.f, 1.f, 1.f), rotation_type::eye), action(), action(60.f, glm::vec3(0.f, 1.f, 1.f), rotation_type::eye)
                    , action(-180.f, glm::vec3(0.f, 1.f, 0.f), rotation_type::centre)   
                   };

actions_t::iterator next_action(actions_t& actions, actions_t::iterator ait)
{
    ++ait;
    if (ait == actions.end())
        return actions.begin();
    return ait;
}

//actions_t::iterator prev_action(actions_t& actions, actions_t::iterator ait)
//{
//    if (ait == actions.begin())
//        return ----actions.end();
//    return ----ait;
//}

actions_t::iterator cur_action = actions.begin();

size_t camera_location = 0;
camera_t camera = { glm::vec4(0.f, 8.f, 0.f, 1.f)
                  , glm::vec4(objects[0].x, objects[0].y, objects[0].z, 1.f)
                  , glm::vec4(0.f, 0.f, 0.f, 1.f) 
                  , 0.f};


bool camera_move(camera_t& camera, const glm::vec3& next_point)
{
    float speed = 0.04f;
    glm::vec3 direction(next_point.x - camera.centre.x, next_point.y - camera.centre.y, next_point.z - camera.centre.z);
    float dir_length = glm::length(direction);
    direction.x /= dir_length;
    direction.y /= dir_length;
    direction.z /= dir_length;

    if (fabs(next_point.x - camera.centre.x) < direction.x * speed)
        return true;

    camera.move(direction * speed);

    return false;
}

bool camera_rotate(camera_t& camera, float rotation_angle, const glm::vec3& axis, const rotation_type rotate)
{
    const float inc = rotation_angle > 0 ? 1.f : -1.f;
    if (camera.angle == rotation_angle)
    {
        camera.angle = 0.f;
        return true;
    }

    const float angle = inc / 180 * 3.1415;

    camera.rotate(angle, axis, rotate);
    //camera.center.x = cos(angle) * (camera.center.x - camera.eye.x) + sin(angle) * (camera.center.z - camera.eye.z) + camera.eye.x;
    //camera.center.z = cos(angle) * (camera.center.z - camera.eye.z) - sin(angle) * (camera.center.x - camera.eye.x) + camera.eye.z;
    camera.angle += inc;
    return false;
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
    switch (cur_action->act)
    {
    case stand:
        break;
    case move:
        if (camera_move(camera, cur_action->dest))
            cur_action = next_action(actions, cur_action);
        break;
    case rotate:
        if (camera_rotate(camera, cur_action->angle, cur_action->axis, cur_action->type))
            cur_action = next_action(actions, cur_action);
        break;
    }
    gluLookAt
        ( camera.eye.x, camera.eye.y, camera.eye.z
        , camera.centre.x, camera.centre.y, camera.centre.z
        , 0, 1, 0);

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
    if (cur_action->act != stand)
        return;
    switch (key)
    {
    case 'd':
        cur_action = next_action(actions, cur_action);
        break;
    //case 'a':
    //    cur_action = prev_action(actions, cur_action);
    //    break;
    }
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