#ifndef MODEL_H
#define MODEL_H
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

/*################ POINT ################*/

struct Point3D{
    float x;
    float y;
    float z;
    bool tree;
};

// CONSTRUCTEURS

// Construit un point à partir de trois coordonnées
Point3D createPoint(float x, float y, float z);

// METHODES

void printPoint3D(Point3D p);

/*################ TRIANGLE ################*/

struct Triangle {
    Point3D a;
    Point3D b;
    Point3D c;
};

// CONSTRUCTEURS

// Construit un point à partir de trois coordonnées
Triangle createTriangle(Point3D a, Point3D b, Point3D c);

/*################ VECTOR ################*/

struct Vector3D{
    float x;
    float y;
    float z;
};

// CONSTRUCTEURS

// Construit un vecteur à partir de deux points
Vector3D createVectorFromPoints(Point3D p1, Point3D p2);

// METHODES

void printVector3D(Vector3D v);

float dot(Vector3D v1, Vector3D v2);

float norm(Vector3D v);

Vector3D normalize(Vector3D v);

float det(Vector3D v1, Vector3D v2);

Vector3D prodVect(Vector3D v1, Vector3D v2);

bool allOnLeft(Point3D position, Vector3D limit, Point3D points[4]);

#endif