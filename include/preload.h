#ifndef PRELOAD_H
#define PRELOAD_H

#include "geometry.h"
#include "objects.h"

/*------------- PARAMS -------------*/
typedef struct Params{
    string linkHeightMap;
    float rgbMaxValue;
    int xPixels;
    int yPixels;
    float xSize;
    float ySize;
    float zMin;
    float zMax;
} Params;

// CONSTRUCTEUR

// Constuit une structure Params, et instancie ses valeurs à 0
Params createParams();

// Charge l'Heightmap
void readParams(Params* params, Camera* camera);

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