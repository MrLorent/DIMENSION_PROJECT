#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include "../include/geometry.h"

// Construit le point 
Point3D createPoint(float x, float y, float z){
    Point3D newPoint ;
    newPoint.x = x;
    newPoint.y = y;
    newPoint.z = z;
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

Vector3D invertVector(Vector3D v){
    Vector3D newVector;

    newVector.x = -v.x;
    newVector.y = -v.y;
    newVector.z = -v.z;

    return newVector;
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

void printPoint3D(Point3D p)
{
    cout << "(" << p.x << ";" << p.y << ";" << p.z << ")" << endl;
}

void printVector3D(Vector3D v)
{
    cout << "(" << v.x << ";" << v.y << ";" << v.z << ")" << endl;
}

// Construit le quad 
Quad createQuad(Point3D hautGauche, Point3D hautDroit, Point3D basDroit, Point3D basGauche){
    Quad newQuad ;
    newQuad.hautGauche = hautGauche;
    newQuad.hautDroit = hautDroit;
    newQuad.basDroit = basDroit;
    newQuad.basGauche = basGauche;
    return newQuad;
}