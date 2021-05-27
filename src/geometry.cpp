#include "../include/geometry.h"

Point3D createPoint(float x, float y, float z){
    Point3D newPoint ;
    newPoint.x=x;
    newPoint.y=y;
    newPoint.z=z;
    newPoint.tree = false;
    return newPoint;
}

Vector3D createVectorFromPoints(Point3D p1, Point3D p2)
{
    Vector3D v;
    v.x = p2.x - p1.x;
    v.y = p2.y - p1.y;
    v.z = p2.z - p1.z;
    return v;
}

Triangle createTriangle(Point3D a, Point3D b, Point3D c)
{
    Triangle newTriangle;

    newTriangle.a = a;
    newTriangle.b = b;
    newTriangle.c = c;

    return newTriangle;
}

float dot(Vector3D v1, Vector3D v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float norm(Vector3D v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3D normalize(Vector3D v)
{
    float n = norm(v);
    if (n == 0.)
        return v;
    v.x /= n;
    v.y /= n;
    v.z /= n;
    return v;
}

float det(Vector3D v1, Vector3D v2)
{
    return v1.x*v2.y - v1.y*v2.x;
}

Vector3D prodVect(Vector3D v1, Vector3D v2){
    Vector3D v;
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;
    return v;
}

void printPoint3D(Point3D p)
{
    cout << "(" << p.x << ";" << p.y << ";" << p.z << ")" << endl;
}

void printVector3D(Vector3D v)
{
    cout << "(" << v.x << ";" << v.y << ";" << v.z << ")" << endl;
}


