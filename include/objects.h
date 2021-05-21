#include "geometry.h"

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

