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
    // POSITION
    Point3D position;
    Point3D closestMapPoint;
    float height;
    bool locked;

    // REGARD
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

    float getFovHInRadian();
    float getFovVInRadian();

    bool sees(Point3D a, Point3D b, Point3D c, Point3D d);
};

void initCamera(Camera* cam, Point3D position);

bool allOnLeft(Point3D position, Vector3D limit, Point3D points[4]);

#endif