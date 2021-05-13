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

Node* createNode(){
    return new Node;
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

void Node::fillQuadTree(vector<vector<Point>> chart){
    // INSTANCIATION DU QUAD COURANT
    this->a = chart.at(0).at(0);
    this->b = chart.at(0).at(chart.size()-1);
    this->c = chart.at(chart.size()-1).at(0);
    this->d = chart.at(chart.size()-1).at(1);

    // INSTANCIATION DES ENFANTS SI BESOIN
    if(chart.size() == 2){
        return;
    }else{
        int subSize = (chart.size()/2) -1;
        vector<vector<Point>> subChart;
        
        // Pour chaque enfant
        for(int i=TOP_LEFT; i <= BOTTOM_LEFT; i++){
            switch(i){
                case TOP_LEFT:
                    fillSubChart(chart, subChart, 0,0);
                    this->childA = createNode();
                    this->childA->fillQuadTree(subChart);
                    break;
                case TOP_RIGHT:
                    fillSubChart(chart, subChart, 0,chart.size()-1);
                    this->childB = createNode();
                    this->childB->fillQuadTree(subChart);
                    break;
                case BOTTOM_RIGHT:
                    fillSubChart(chart, subChart, chart.size()-1,0);
                    this->childC = createNode();
                    this->childC->fillQuadTree(subChart);
                    break;
                case BOTTOM_LEFT:
                    fillSubChart(chart, subChart, chart.size()-1,chart.size()-1);
                    this->childD = createNode();
                    this->childD->fillQuadTree(subChart);
                    break;
            }
        }
    }
}

void Node::fillSubChart(vector<vector<Point>> chart, vector<vector<Point>> subChart, int widthStart,int heightStart){
    for(int i=widthStart; i<chart.size()-1;i++){
        for(int j=heightStart; i<chart[0].size()-1;i++){
            subChart.at(i-widthStart).at(j-heightStart) = chart.at(i).at(j);
        }
    }
}