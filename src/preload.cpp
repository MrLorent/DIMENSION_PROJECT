#include <iostream>
#include <fstream>
#include "../include/preload.h"

// Constuit une structure Params, et instancie ses valeurs à 0
Params createParams(){
    Params params;
    params.linkHeightMap="";
    params.xSize=0;
    params.ySize=0;
    params.zMin=0;
    params.zMax=0;
    params.zNear=0;
    params.zFar=0;
    params.fov=0;

    return params;
}

void initParams(Params* params){
    ifstream file("./doc/params.timac");
    if(file) {
        //L'ouverture s'est bien passée, on peut donc lire

        //On lit le chemin de l'image depuis le fichier
        file >> params->linkHeightMap;
        cout << params->linkHeightMap;
        file >> params->xSize;
        file >> params->ySize;
        file >> params->zMin;
        file >> params->zMax;
        file >> params->zNear;
        file >> params->zFar;
        file >> params->fov;

        remove("timac.txt");
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
void loadHeightMap(Params params, PointChart* heightMap){
    ifstream file(params.linkHeightMap);

    if(file) {
        //L'ouverture s'est bien passée, on peut donc lire
        int nbPixelsX, nbPixelsY, Zvalue;

        string line;
        getline(file, line); 
        getline(file, line);

        file >> nbPixelsX;
        file >> nbPixelsY;

        initPointChart(heightMap, nbPixelsX, nbPixelsY);

        int x,y,z;
        for (int i=0; i < nbPixelsY ; i++ ){
            heightMap->points[i] = new Point3D[nbPixelsX];
            y = i; //convertValue(i,nbPixelsY,params.ySize);
            for (int j=0; j< nbPixelsX ; j++){
                x = j; //convertValue(j,nbPixelsX, params.xSize);
                
                file >> Zvalue;
                z = convertElevation (Zvalue, params);
                
                heightMap->points[i][j] = createPoint(x,y,z);
            }
        }

        remove("image.ppm");
    }else
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
    }
        
}

int convertValue (int n, int nbPixels, int size){
    return ((float) n/nbPixels)*size;
}

int convertElevation (int z, Params params){
    return params.zMin + (float)(z/255)*(params.zMax - params.zMin);
}