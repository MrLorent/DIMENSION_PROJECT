#ifndef QUADTREE_H
#define QUADTREE_H

#include "geometry.h"
#include "preload.h"

// CODE ASSOCIER A CHAQUN DES COINS D'UN QUAD
const int TOP_LEFT = 0;
const int TOP_RIGHT = 1;
const int BOTTOM_RIGHT = 2;
const int BOTTOM_LEFT = 3;

typedef struct Node {
    // POSITION IN TREE
    int height;

    // CORNERS
    Point3D a; // Top left
    Point3D b; // Top right
    Point3D c; // Bottom right
    Point3D d; // Bottom left

    // CORNERS TO DRAW
    Point3D tmpA;
    Point3D tmpB;
    Point3D tmpC;
    Point3D tmpD;

    // TREES
    bool hasTree; // Informe sur la présence d'arbre sur le quad
    
    // CHILDREN
    Node* childA = NULL; // Top left child
    Node* childB = NULL; // Top right child
    Node* childC = NULL; // Bottom right child
    Node* childD = NULL; // Bottom left child

    /*############### ACCESSEURS ###############*/
    // EN LECTURE

    // Retourne un pointeur vers le fils haut
    // gauche du node courant
    Node* getChildA();
    
    // Retourne un pointeur vers le fils haut droit
    // du node courant
    Node* getChildB();

    // Retourne un pointeur vers le fils bas droit
    // du node courant
    Node* getChildC();
    
    // Retourne un pointeur vers le fils bas droit
    // du node courant
    Node* getChildD();
    
    // EN ECRITURE

    /*############### METHODES ###############*/

    // Revoie true sur le nodes est un enfant
    bool isLeaf();
    
    // Revoie la hauteur d'un node dans le quadtree
    int getHeight();

    // Instancie l'attribut "height" de chaque node
    // avec sa hauteur dans l'arbre
    void initNodesHeight();

    // Initialise la position temporaire de chacun
    // des quatres points a celle des points de
    // reference
    void initTmpPoints();

    // Renvoie le code du coin du quad le plus
    // proche de la position passée en paramètre
    int getClosestCornerFrom(Point3D cam);

    // Renvoie la distance a la quelle se trouve
    // le node par rapport a la position passee
    // en parametre (la distance du quad est
    // calculee par rapport au coin le plus proche)
    float getDistanceFrom(Point3D position);

    // Renvoie un tableau contenant les quatre
    // coins du quad rangees du plus proche au
    // plus loin de la position passee en parametre
    void orderByDistance(Point3D chart[4], Point3D cam);

    void fixTopCrack();

    void fixRightCrack();

    void fixBottomCrack();

    void fixLeftCrack();

}QuadTree, Node;

/*################ CONSTRUCTEURS ################*/

// Créer un nouveaux node à partir de quatres 
// points données
Node* createQuadTree(Point3D a, Point3D b, Point3D c, Point3D d, PointChart * heightMap, Params params);

/*################### LOD ###################*/

// Initialise les differents seuils du LOD
// en fonction du zFar de la camera
void initLODLevels(float max);

// Indique si le quad passe en parametre doit etre dessiner
// ou non par rapport à sa distance a la camera, et a sa
// hauteur dans le quadtree et detecte les cracks
bool LevelOfDetailsReached(QuadTree* quad, Point3D position);

// Modifie la hauteur de certains points des enfants du quad
// passe en parametre pour reparer les cracks genere pas le
// LOD
void dealWithCracks(QuadTree* quad, Point3D position, int closest, float LOD_LEVEL);

#endif // QUADTREE_H