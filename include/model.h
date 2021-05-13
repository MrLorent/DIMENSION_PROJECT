#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <iostream>

typedef struct Scene {
    std::string linkImg;
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
Scene createScene(std::string linkImg, int xsize, int ysize,
 int zmin, int zmax, int znear, int zfar, double fov);
// Construit le point 
Point createPoint(float x, float y, float z);
// Construit le quad 
Quad createQuad(Point hautGauche,
    Point hautDroit,
    Point basDroit,
    Point basGauche);

#endif
