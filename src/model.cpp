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
    ifstream fichier(params.linkImg);
    if(fichier) {
            //L'ouverture s'est bien passée, on peut donc lire

            string ligne;
            getline(fichier, ligne); 
            cout << "\n ligne1 : " << ligne << endl ;  //On lit un mot depuis le fichier
            getline(fichier, ligne);
            cout << "\n ligne2 : " << ligne << endl ;
            //fichier.ignore();        //On change de mode

            //On lit une ligne complète

            int nbPixelsX, nbPixelsY, Zvalue;

            fichier >> nbPixelsX;
            cout << "\n nbX : " << nbPixelsX << endl ;
            fichier >> nbPixelsY;
            cout << "\n nbY : " << nbPixelsY << endl ;

            int x,y,z;
            for (int i=0; i < nbPixelsY ; i++ ){
                for (int j=0; j< nbPixelsX ; j++){
                    fichier >> Zvalue;
                    heightMap.at(i).at(j)= createPoint(i,j,Zvalue);
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
