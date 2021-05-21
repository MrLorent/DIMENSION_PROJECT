#ifndef OBJECTS_H
#define OBJECTS_H
#include "geometry.h"
#include "objects.h"

using namespace std;

struct Camera{
    Point3D position;
    float latitude; // HRIZONTALE
    float longitude; // VERTICAL
    Vector3D up;
};

struct Tree{
    float x;
    float y;
    float z;
};

Tree createTree(float x, float y, float z);


#endif 
