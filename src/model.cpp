#include <iostream>
#include <fstream>
#include <cstring>
#include "../include/model.h"
#include <math.h>

// Construit le scene 
Scene createScene(){
    Scene s ;
    s.linkImg=' ';
    s.xsize=0;
    s.ysize=0;
    s.zmin=0;
    s.zmax=0;
    s.znear=0;
    s.zfar=0;
    s.fov=0;
    return s;
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
    newQuad.hautGauche=hautGauche;
    newQuad.hautDroit=hautDroit;
    newQuad.basDroit=basDroit;
    newQuad.basGauche=basGauche;
    return newQuad;
}

// Charge l'Heightmap
void loadHeightmap(Scene *s){
      ifstream fichier("./doc/timac.txt");
      if(fichier) {
            //L'ouverture s'est bien passée, on peut donc lire

            //On lit le chemin de l'image depuis le fichier
            string linkImg;
            fichier >> linkImg; 
            s->linkImg = linkImg;

            int xsize, ysize, zmin, zmax, znear, zfar ;
            double fov;
            fichier >> xsize;
            s->xsize = xsize;
            fichier >> ysize;
            s->ysize = ysize;
            fichier >> zmin;
            s->zmin = zmin;
            fichier >> zmax;
            s->zmax = zmax;
            fichier >> znear;
            s->zmax = zmax;
            fichier >> zfar;
            s->zfar = zfar;
            fichier >> fov;
            s->fov = fov;
            // cout << "\n donnée recup " << endl ;

            remove("timac.txt");
        }else
        {
            cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
        }
}

// Charge les points
int loadPointsHeightmap(){
    ifstream fichier("./doc/image.ppm");
    if(fichier) {
            //L'ouverture s'est bien passée, on peut donc lire

            string ligne;
            getline(fichier, ligne); 
            cout << "\n ligne1 : " << ligne << endl ;  //On lit un mot depuis le fichier
            getline(fichier, ligne);
            cout << "\n ligne2 : " << ligne << endl ;
            fichier.ignore();        //On change de mode

            //On lit une ligne complète

            int nbPixelsX, nbPixelsY, Zvalue;

            fichier >> nbPixelsX;
            cout << "\n nbX : " << nbPixelsX << endl ;
            fichier >> nbPixelsY;
            cout << "\n nbY : " << nbPixelsY << endl ;

            Point heightMap[nbPixelsX+100][nbPixelsY];

            for (int i=0; i < nbPixelsY ; i++ ){
                cout << "\n ligne " << i << endl ;
                for (int j=0; j< nbPixelsX+100 ; j++){
                    fichier >> Zvalue;
                    cout << " Z" << j << ": " << Zvalue << endl ;
                    heightMap[i][j]=createPoint(i,j,Zvalue);
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
