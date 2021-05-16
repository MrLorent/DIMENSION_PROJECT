#ifndef QUADTREE_H
#define QUADTREE

#include "geometry.h"

const int TOP_LEFT = 0;
const int TOP_RIGHT = 1;
const int BOTTOM_RIGHT = 2;
const int BOTTOM_LEFT = 3;

typedef struct Node {
    // CORNERS
    Point3D a; // Top left
    Point3D b; // Top right
    Point3D c; // Bottom right
    Point3D d; // Bottom left
    
    // CHILDREN
    Node* childA; // Top left child
    Node* childB; // Top right child
    Node* childC; // Bottom right child
    Node* childD; // Bottom left child

    /*############### ACCESSEURS ###############*/
    // EN LECTURE

    Node* getChildA();
    Node* getChildB();
    Node* getChildC();
    Node* getChildD();
    
    // EN ECRITURE

    /*############### METHODES ###############*/
    bool isLeaf();
    void fillQuadTree(vector<vector<Point3D>> chart);
    void fillSubChart(vector<vector<Point3D>> chart, vector<vector<Point3D>> subChart, int widthStart,int heightStart);

}QuadTree, Node;

/*################ CONSTRUCTEURS ################*/

QuadTree* createQuadTree();
Node* createNode();

#endif // QUADTREE_H