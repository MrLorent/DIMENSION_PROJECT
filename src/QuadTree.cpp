#include <vector>

#include "../include/QuadTree.h"

/*################ CONSTRUCTEURS ################*/

// Créer un nouveaux node à partir de quatres 
// points données
QuadTree* createQuadTree(){
    // CREATION DU QUADTREE
    QuadTree* newTree = new QuadTree;
    // CHILDREN
    newTree->childA = NULL;
    newTree->childB = NULL;
    newTree->childC = NULL;
    newTree->childD = NULL;

    return newTree;
}

Node* createNode(){
    // CREATION DU NODE
    Node* newNode = new Node;
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

/*################ METHODES ################*/

bool Node::isLeaf(){
    if(!this->childA && !this->childB && !this->childC && !this->childD){
        return true;
    }else{
        return false;
    }
}

void Node::fillQuadTree(vector<vector<Point3D>> chart){
    // INSTANCIATION DU QUAD COURANT
    this->a = chart.at(0).at(0);
    this->b = chart.at(0).at(chart.size()-1);
    this->c = chart.at(chart.size()-1).at(0);
    this->d = chart.at(chart.size()-1).at(1);

    // INSTANCIATION DES ENFANTS SI BESOIN
    if(chart.size() == 2){
        return;
    }else{
        
    }
}

void Node::fillSubChart(vector<vector<Point3D>> chart, vector<vector<Point3D>> subChart, int widthStart,int heightStart){
    for(int i=widthStart; i<chart.size()-1;i++){
        for(int j=heightStart; i<chart.at(0).size()-1;i++){
            
        }
    }
}