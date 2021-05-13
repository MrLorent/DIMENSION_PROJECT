#ifndef QUADTREE_H
#define QUADTREE

#include "model.h"

typedef struct Node {
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

    // ACCESSEURS
    // En lecture

    Node* getChildA();
    Node* getChildB();
    Node* getChildC();
    Node* getChildD();
    
    // En Ecriture

    // METHODES
    bool isLeaf();

    
}Node, QuadTree;

// CONSTRUCTEUR
Node* createNode(Point a, Point b, Point c, Point d);

#endif // QUADTREE_H