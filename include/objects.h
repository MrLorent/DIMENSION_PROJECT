#ifndef OBJECTS_H
#define OBJECTS_H
#include <math.h>

#include "geometry.h"
#include "color.h"

using namespace std;

/*################ LUMIERE ################*/

struct Sun {
    Point3D origin;
    Point3D position;
    Color3f color;
    float longitude;
    float radius;
    float shininess;
    bool moving;
};

void initSun(Sun* sun, float mapWidth, float mapHeigh);

/*################ CAMERA ################*/

struct Camera {
    // GLUT_LOOK_AT PARAMETERS
    Point3D position;
    float latitude; // HORIZONTAL
    float longitude; // VERTICAL
    Vector3D up;

    // VISUAL FIELD
    float zNear;
    float zFar;
    float fovH;
    float fovV;
    Vector3D leftLimit;
    Vector3D frontLimit;
    Vector3D rightLimit;

    bool sees(Point3D a, Point3D b, Point3D c, Point3D d);

    float getFovHInRadian();
    float getFovVInRadian();
};

bool allOnLeft(Point3D position, Vector3D limit, Point3D points[4]);

#endif