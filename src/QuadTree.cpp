#include "../include/QuadTree.h"

/*################ CONSTRUCTEURS ################*/

// Créer un nouveaux node à partir de quatres 
// points données
Node* createNode(Point a, Point b, Point c, Point d){
    // CREATION DU QUAD
    Node* newNode = new Node;
    // QUAD CORNERS
    newNode->a = a;
    newNode->b = b;
    newNode->c = c;
    newNode->d = d;
    // CHILDREN
    newNode->childA = NULL;
    newNode->childB = NULL;
    newNode->childC = NULL;
    newNode->childD = NULL;

    return newNode;
}

/*################ ACCESSEURS ################*/
// EN LECTURE

// Retourne un pointeur vers le fils haut gauche
// du node courant
Node* Node::getChildA() {
    return this->childA;
};

// Retourne un pointeur vers le fils haut droit
// du node courant
Node* Node::getChildB() {
    return this->childB;
};

// Retourne un pointeur vers le fils bas droit
// du node courant
Node* Node::getChildC() {
    return this->childC;
};

// Retourne un pointeur vers le fils bas droit
// du node courant
Node* Node::getChildD() {
    return this->childD;
};