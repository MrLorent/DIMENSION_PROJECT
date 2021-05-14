#ifndef MODEL_H
#define MODEL_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
//#include "../include/QuadTree.h"

using namespace std;

typedef struct Params{
    string linkImg;
    int xsize, ysize, zmin, zmax, znear, zfar;
    double fov;
} Params;

typedef struct Point{
    float x;
    float y;
    float z;
} Point;

typedef struct Quad {
    Point hautGauche;
    Point hautDroit;
    Point basDroit;
    Point basGauche;
}Quad;

// Construit le point 
Point createPoint(float x, float y, float z);
// Construit le quad 
Quad createQuad(Point hautGauche, Point hautDroit,Point basDroit,Point basGauche);

// Construit le scene 
Params createParams();
// Charge l'Heightmap
void loadParams(Params* params);
// Charge les pts de l'Heightmap
int loadHeightMap(Params params, vector<vector<Point>> heightMap );
// Adapte la taille de l'image en carré et puissance de 2
void adaptNbPixels (*minNbPixels);
// Converti valeur en position pour scène
int convertValue (int n, int nbPixels, int size);
// Converti valeur de z en élévation pour scène
int convertElevation (int z, Params params);


#endif
