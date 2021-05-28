#ifndef OBJECTS_H
#define OBJECTS_H
#include <math.h>

#include "color.h"
#include "geometry.h"
#include "preload.h"
#include "QuadTree.h"

using namespace std;

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
    void adjustHeight(Node* quad);
};

void initCamera(Camera* cam, Params params, Point3D position);

/*################ LUMIERE ################*/

struct Sun {
    Point3D origin;
    Point3D position;
    Color3f color;
    float longitude;
    float radius;
    float shininess;
    bool moving;

    Color3f getLight(Point3D a, Point3D b, Point3D c);
};

void initSun(Sun* sun, float mapWidth, float mapHeigh);

#endif