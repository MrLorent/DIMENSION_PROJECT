#include "../include/objects.h"
#include <time.h> 


// Crée un tableau d'arbres
void createTrees(Tree trees[6]){
    for (int i=0; i<6 ; i++){
        trees[i].x = rand() % 4 + (-1);
        trees[i].y= rand() % 4 + (-1);
        trees[i].z=0;
    }
};

