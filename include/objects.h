#ifndef OBJECTS_H
#define OBJECTS_H
#include "geometry.h"

using namespace std;

struct Camera{
    Point3D position;
    float latitude; // HRIZONTALE
    float longitude; // VERTICAL
    Vector3D up;
};

#endif // OBJECTS_H