#ifndef QUADTREE_H
#define QUADTREE_H

#include "geometry.h"
#include "preload.h"

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
    bool hasTree;
    
    // CHILDREN
    Node* childA = NULL; // Top left child
    Node* childB = NULL; // Top right child
    Node* childC = NULL; // Bottom right child
    Node* childD = NULL; // Bottom left child

    /*############### ACCESSEURS ###############*/
    // EN LECTURE

    Node* getChildA();
    Node* getChildB();
    Node* getChildC();
    Node* getChildD();
    
    // EN ECRITURE

    /*############### METHODES ###############*/
    bool isLeaf();
    
    int getHeight();
    void initNodesHeight();
    void initTmpPoints();

    int getClosestCorner(Point3D cam);
    float getDistanceFrom(Point3D position);
    void orderByDistance(Point3D chart[4], Point3D cam);

}QuadTree, Node;

/*################ CONSTRUCTEURS ################*/

Node* createQuadTree(Point3D a, Point3D b, Point3D c, Point3D d, PointChart * heightMap, Params params);

#endif // QUADTREE_H