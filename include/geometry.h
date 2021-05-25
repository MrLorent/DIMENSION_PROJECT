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

/*################ COULEUR ################*/

struct Color3f {
    float r;
    float g;
    float b;
};

Color3f createColor(float r, float g, float b);
Color3f multColor(Color3f c, float a);
Color3f multColors(Color3f c1, Color3f c2);

/*################ LUMIERE ################*/

struct Sun {
    Point3D position;
    Color3f color;
    bool risingSun;
    float distance;
    float longitude;
};

void initSun(Sun* sun, float mapWidth, float mapHeigh);

#endif