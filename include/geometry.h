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
} Point3D, vector3D;

/*-------- CONSTRUCTEURS ---------*/

// Construit un point à partir de trois coordonnées
Point3D createPoint(float x, float y, float z);


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
