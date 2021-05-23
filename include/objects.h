#ifndef OBJECTS_H
#define OBJECTS_H
#include <math.h>

#include "geometry.h"

using namespace std;

struct Camera {
    // GLUT_LOOK_AT PARAMETERS
    Point3D position;
    float latitude; // HORIZONTALE
    float longitude; // VERTICAL
    Vector3D up;

    // VISUAL FIELD
    float zNear;
    float zFar;
    float fov;
    float fovForFrustum;
    Vector3D leftLimit;
    Vector3D frontLimit;
    Vector3D rightLimit;

    bool sees(Point3D a, Point3D b, Point3D c, Point3D d);
};

bool allOnLeft(Point3D position, Vector3D limit, Point3D points[4]);

#endif 