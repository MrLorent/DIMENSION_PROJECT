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
    chart->width = width;
    chart->height = height;
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
        cout << "\n nbX : " << nbPixelsX << endl ;
        file >> nbPixelsY;
        cout << "\n nbY : " << nbPixelsY << endl ;

        initPointChart(heightMap, nbPixelsX, nbPixelsY);

        int x,y,z;
        for (int i=0; i < nbPixelsX ; i++ ){
            x=convertValue(i,nbPixelsX,params.xSize);
            for (int j=0; j< nbPixelsY ; j++){
                y=convertValue(j,nbPixelsY, params.ySize);
                
                file >> Zvalue;
                z=convertElevation (Zvalue, params);
                cout << "\n ligne " << j << " : " << x << " " <<  y << " " << z << endl ;
                
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
    return n/nbPixels*size;
}

int convertElevation (int z, Params params){
    return params.zMin + z*(params.zMax - params.zMin);
}