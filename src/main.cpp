#include <iostream>
#include <fstream>
#include <cstring>
#include "../include/model.h"
#include <math.h>

using namespace std;

void chargerHeightmap(Scene *s){
      ifstream fichier("../doc/timac.txt");
      if(fichier) {
            //L'ouverture s'est bien passée, on peut donc lire

            //On lit le chemin de l'image depuis le fichier
            string linkImg;
            fichier >> linkImg; 
            s->linkImg = tlinkImg;
            

            // cout << "lien recup : " << s->lien << endl ;

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
            // cout << "\n donnée recup : " << leJeu->mode << endl ;

            
            remove("sauvegarde.txt");
        }else
        {
            cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
        }
}
