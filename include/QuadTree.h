#ifndef QUADTREE_H
#define QUADTREE

#include "model.h"

const int TOP_LEFT = 0;
const int TOP_RIGHT = 1;
const int BOTTOM_RIGHT = 2;
const int BOTTOM_LEFT = 3;

typedef struct QuadTree {
    // CORNERS
    Point a; // Top left
    Point b; // Top right
    Point c; // Bottom right
    Point d; // Bottom left
    
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
    void fillQuadTree(vector<vector<Point>> chart);
    void fillSubChart(vector<vector<Point>> chart, vector<vector<Point>> subChart, int widthStart,int heightStart);

}QuadTree, Node;

/*################ CONSTRUCTEURS ################*/

QuadTree* createQuadTree();
Node* createNode();

#endif // QUADTREE_H