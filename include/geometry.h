#ifndef MODEL_H
#define MODEL_H
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*############# POINT #############*/

struct Point3D{
    float x;
    float y;
    float z;
    bool tree;
};

/*############# VECTOR #############*/

struct Vector3D{
    float x;
    float y;
    float z;
};

/*-------- CONSTRUCTEURS ---------*/

// Construit un point à partir de trois coordonnées
Point3D createPoint(float x, float y, float z);

Vector3D createVectorFromPoints(Point3D p1, Point3D p2);

float dot(Vector3D v1, Vector3D v2);

float norm(Vector3D v);

Vector3D normalize(Vector3D v);

void printPoint3D(Point3D p);
void printVector3D(Vector3D v);

float det(Vector3D v1, Vector3D v2);

#endif
