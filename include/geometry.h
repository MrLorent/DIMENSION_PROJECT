#ifndef MODEL_H
#define MODEL_H
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*############# POINT #############*/

typedef struct Vec3f{
    float x;
    float y;
    float z;
} Point3D, Vector3D;

/*-------- CONSTRUCTEURS ---------*/

// Construit un point à partir de trois coordonnées
Point3D createPoint(float x, float y, float z);

Vector3D createVectorFromPoints(Point3D p1, Point3D p2);

Vector3D invertVector(Vector3D v);

float dot(Vector3D v1, Vector3D v2);

float norm(Vector3D v);

Vector3D normalize(Vector3D v);

void printPoint3D(Point3D p);
void printVector3D(Vector3D v);

float det(Vector3D v1, Vector3D v2);


/*############# QUADS #############*/

typedef struct Quad {
    Point3D hautGauche;
    Point3D hautDroit;
    Point3D basDroit;
    Point3D basGauche;
}Quad;

/*-------- CONSTRUCTEURS ---------*/

// Construit un quad à partir de quatre points
Quad createQuad(Point3D hautGauche, Point3D hautDroit, Point3D basDroit, Point3D basGauche);


#endif
