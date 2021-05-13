#ifndef MODEL_H
#define MODEL_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>

#include <string>

using namespace std;

typedef struct Scene {
    string linkImg;
    int xsize, ysize, zmin, zmax, znear, zfar ;
    double fov;
} Scene ;

typedef struct Point{
    float x;
    float y;
    float z;
} Point;

typedef struct Quad{
    Point hautGauche;
    Point hautDroit;
    Point basDroit;
    Point basGauche;
} Quad;

// Construit le scene 
Scene createScene();
// Construit le point 
Point createPoint(float x, float y, float z);
// Construit le quad 
Quad createQuad(Point hautGauche,
    Point hautDroit,
    Point basDroit,
    Point basGauche);
// Charge l'Heightmap
void loadHeightmap(Scene *s);


#endif
