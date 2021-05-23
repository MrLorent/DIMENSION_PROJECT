#include <iostream>
#include <fstream>
#include <time.h> 
#include "../include/preload.h"

// Constuit une structure Params, et instancie ses valeurs à 0
Params createParams(){
    Params params;
    params.linkHeightMap="";
    params.xPixels = 0;
    params.yPixels = 0;
    params.xSize = 0;
    params.ySize = 0;
    params.zMin = 0;
    params.zMax = 0;

    return params;
}

void readParams(Params* params, Camera* camera){
    ifstream file("./doc/params.timac");
    if(file) {
        //L'ouverture s'est bien passée, on peut donc lire

        // PARAMETRE DE LA MAP
        file >> params->linkHeightMap;
        file >> params->xSize;
        file >> params->ySize;
        file >> params->zMin;
        file >> params->zMax;

        // PARAMETRE CAMERA
        file >> camera->zNear;
        file >> camera->zFar;
        file >> camera->fov;

        remove("params.timac");
    }else{
        cout << "ERREUR: Impossible d'ouvrir le fichier params.timac." << endl;
    }
}

void initPointChart(PointChart* chart, const int width, const int height){
    chart->points = new Point3D*[height];
    chart->width = width;
    chart->height = height;
}

void freePointChart(PointChart* chart){
    for(int i=0; i<chart->height-1;i++){
        free(chart->points[i]);
    }
    free(chart->points);
}

// Charge les points
void loadHeightMap(Params* params, PointChart* heightMap){
    ifstream file(params->linkHeightMap);

    if(file) {
        //L'ouverture s'est bien passée, on peut donc lire
        int Zvalue;

        string line;
        getline(file, line); 
        getline(file, line);

        file >> params->xPixels;
        file >> params->yPixels;

        file >> params->rgbMaxValue;

        initPointChart(heightMap, params->xPixels, params->yPixels);

        int x,y,z;
        for (int i=0; i < params->yPixels ; i++ ){
            heightMap->points[i] = new Point3D[params->xPixels];
            y = i;
            for (int j=0; j< params->xPixels ; j++){
                x = j;
                
                file >> z;
                
                heightMap->points[i][j] = createPoint(x,y,z);
            }
        }

        remove("image.ppm");
    }else
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
    }
        
}

Point3D createMapPoint(Point3D p, Params params){
    Point3D newMapPoint;

    newMapPoint.x = ((float) p.x/params.xPixels)*params.xSize;
    newMapPoint.y = ((float) p.y/params.yPixels)*params.ySize;
    newMapPoint.z = params.zMin + (p.z/params.rgbMaxValue)*(params.zMax - params.zMin);
    newMapPoint.tree = p.tree;

    return newMapPoint;
}

// Charge arbre aléatoirement
void LoadTrees(PointChart* heightMap){
    int randomX=0;
    int randomY=0;
    for (int j=0; j < heightMap->width/2 ; j++){
        randomX= rand() % heightMap->width;
        randomY= rand() % heightMap->height;
        heightMap->points[randomY][randomX].tree = true;
    };
};