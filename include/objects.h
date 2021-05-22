#ifndef OBJECTS_H
#define OBJECTS_H
#include "geometry.h"
#include "objects.h"

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

    Vector3D a;
    Vector3D b;
    Vector3D c;
};

struct Tree{
    float x;
    float y;
    float z;
};

void createTrees(Tree trees[6]);


#endif 
