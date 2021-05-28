#ifndef PRELOAD_H
#define PRELOAD_H
#include <iostream>
#include <fstream>
#include <time.h> 

#include "geometry.h"

/*------------- PARAMS -------------*/
typedef struct Params{
    // PARAMETRES DE MAP
    string linkHeightMap;
    float rgbMaxValue;
    int xPixels;
    int yPixels;
    float xSize;
    float ySize;
    float zMin;
    float zMax;

    // PARAMETRES DE CAMERA
    float zNear;
    float zFar;
    float fovV;
} Params;

// CONSTRUCTEUR

// Constuit une structure Params, et instancie ses valeurs à 0
Params createParams();

// Charge l'Heightmap
void readParams(Params* params);

/*------------- POINTCHART -------------*/

struct PointChart{
    Point3D** points; // [y][x]
    int width; // x
    int height; // y
};

// CONSTRUCTEUR

void initPointChart(PointChart* chart, int width, int height);
void freePointChart(PointChart* chart);

// Charge les pts de l'Heightmap
void loadHeightMap(Params* params, PointChart* heightMap);

// Converti valeur en position pour scène
Point3D createMapPoint(Point3D p, Params params);

// Charge arbre aléatoirement
void LoadTrees(PointChart* heightMap, int NB_TREES);

#endif