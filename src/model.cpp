#include <iostream>
#include <fstream>
#include <cstring>
#include "../include/model.h"
#include <math.h>

// Construit la scene 
Params createParams(){
    Params params;
    params.linkImg=' ';
    params.xsize=0;
    params.ysize=0;
    params.zmin=0;
    params.zmax=0;
    params.znear=0;
    params.zfar=0;
    params.fov=0;

    return params;
}

// Construit le point 
Point createPoint(float x, float y, float z){
    Point newPoint ;
    newPoint.x=x;
    newPoint.y=y;
    newPoint.z=z;
    return newPoint;
}

// Construit le quad 
Quad createQuad(Point hautGauche,Point hautDroit,Point basDroit, Point basGauche){
    Quad newQuad ;
    newQuad.hautGauche = hautGauche;
    newQuad.hautDroit = hautDroit;
    newQuad.basDroit = basDroit;
    newQuad.basGauche = basGauche;
    return newQuad;
}


int convertValue (int n, int nbPixels, int size){
    return n*size/nbPixels;
}

int convertElevation (int z, Params params){
    return params.zmin + z*(params.zmax - params.zmin);
}

void adaptNbPixels (*minNbPixels){
    int nb;
    for (int i = 0 ; pow(1,i)< *minNbPixels ; i++){
        nb = 2 * 2 ;


    }
    if (*nbPixelsX % 2 != 0){
        do { *nbPixelsX--; }
        while (*nbPixelsX % 2 == 0);
    } *nbPixelsX = *nbPixelsY;
}


// Charge l'Heightmap
void loadParams(Params* params){
      ifstream fichier("./doc/timac.txt");
      if(fichier) {
            //L'ouverture s'est bien passée, on peut donc lire

            //On lit le chemin de l'image depuis le fichier
            string linkImg;
            fichier >> linkImg; 
            params->linkImg = linkImg;

            int xsize, ysize, zmin, zmax, znear, zfar ;
            double fov;
            fichier >> xsize;
            params->xsize = xsize;
            fichier >> ysize;
            params->ysize = ysize;
            fichier >> zmin;
            params->zmin = zmin;
            fichier >> zmax;
            params->zmax = zmax;
            fichier >> znear;
            params->zmax = zmax;
            fichier >> zfar;
            params->zfar = zfar;
            fichier >> fov;
            params->fov = fov;
            // cout << "\n donnée recup " << endl ;

            remove("timac.txt");
        }else
        {
            cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
        }
}

// Charge les points
int loadHeightMap(Params params, vector<vector<Point>> heightMap){
    ifstream fichier("./doc/image.ppm");
    if(fichier) {
            //L'ouverture s'est bien passée, on peut donc lire

            int minSize;
            if (params.xsize<params.ysize) minSize = params.xsize;
            else minSize = params.ysize;

            string ligne;
            getline(fichier, ligne); 
            getline(fichier, ligne);

            int nbPixelsX, nbPixelsY, Zvalue;

            fichier >> nbPixelsX;
            cout << "\n nbX : " << nbPixelsX << endl ;
            fichier >> nbPixelsY;
            cout << "\n nbY : " << nbPixelsY << endl ;

            int minNbPixels;
            if (nbPixelsX<nbPixelsY) minNbPixels = nbPixelsX;
            else minNbPixels = nbPixelsY;

            adaptNbPixels (&minNbPixels);
            cout << "\n Image réduite en format carré avec nbPixels = " << minNbPixels << endl ;

            int x,y,z;
            for (int i=0; i < minNbPixels ; i++ ){
                x=convertValue(i,minNbPixels,minSize);
                for (int j=0; j< minNbPixels ; j++){
                    fichier >> Zvalue;
                    y=convertValue(j,minNbPixels,paraminSize);
                    z=convertElevation (Zvalue, params);
                    cout << "\n ligne " << j << " : " << x << " " <<  y << " " << z << endl ;
                    heightMap.at(i).at(j)= createPoint(x,y,z);
                }
            }

            remove("image.ppm");
            return 1 ;
        }else
        {
            cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
            return 0 ;
        }
        
}
