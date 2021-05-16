#ifndef PRELOAD_H
#define PRELOAD_H

#include "./geometry.h"

/*------------- PARAMS -------------*/
typedef struct Params{
    string linkHeightMap;
    int xSize;
    int ySize;
    int zMin;
    int zMax;
    int zNear;
    int zFar;
    double fov;
} Params;

// CONSTRUCTEUR

// Constuit une structure Params, et instancie ses valeurs à 0
Params createParams();

// Charge l'Heightmap
void initParams(Params* params);

/*------------- POINTCHART -------------*/

struct PointChart{
    Point3D* points;
    int width;
    int height;
};

// CONSTRUCTEUR

void initPointChart(PointChart* chart, int width, int height);

// Charge les pts de l'Heightmap
void loadHeightMap(Params params, PointChart* heightMap);

// Converti valeur en position pour scène
int convertValue (int n, int nbPixels, int size);

// Converti valeur de z en élévation pour scène
int convertElevation (int z, Params params);

#endif