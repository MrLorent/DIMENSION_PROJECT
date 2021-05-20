#ifndef PRELOAD_H
#define PRELOAD_H

#include "./geometry.h"

/*------------- PARAMS -------------*/
typedef struct Params{
    string linkHeightMap;
    int xPixels;
    int yPixels;
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

#endif