#include <vector>

#include "../include/preload.h"
#include "../include/QuadTree.h"

/*################ CONSTRUCTEURS ################*/

// Créer un nouveaux node à partir de quatres 
// points données
QuadTree createQuadTree(){
    // CREATION DU QUADTREE
    QuadTree newTree;
    // CHILDREN
    newTree.childA = NULL;
    newTree.childB = NULL;
    newTree.childC = NULL;
    newTree.childD = NULL;

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

void Node::fillQuadTree(PointChart* chart){
    // INSTANCIATION DU QUAD COURANT
    this->a = chart->points[chart->height-1][0];
    this->b = chart->points[chart->height-1][chart->width-1];
    this->c = chart->points[0][chart->width-1];
    this->d = chart->points[0][0];

    // INSTANCIATION DES ENFANTS SI BESOIN
    if(chart->width <=2 && chart->height <=2){
        return;
    }else{
        int leftWidth = 1, rightWidth = 1;
        int topHeight = 1, bottomHeight = 1;

        if(chart->width != 1){
            rightWidth = chart->width/2;
            leftWidth = chart->width - rightWidth;
        }

        if(chart->height != 1){
            topHeight = chart->height/2;
            bottomHeight = chart->height - topHeight;
        }

        for(int child=TOP_LEFT; child<=BOTTOM_LEFT; child++){
            switch(child){
                case TOP_LEFT:
                    if(chart->height != 1){
                        this->childA = createNode();
                        PointChart* subChart = new PointChart;
                        initPointChart(subChart, leftWidth, topHeight);
                        fillSubChart(chart,subChart,leftWidth,topHeight,0,topHeight);
                        this->childA->fillQuadTree(subChart);
                    }
                    break;
                case TOP_RIGHT:
                    if(chart->height != 1 && chart->width != 1){
                        this->childB = createNode();
                        PointChart* subChart = new PointChart;
                        initPointChart(subChart, rightWidth, topHeight);
                        fillSubChart(chart,subChart,rightWidth,topHeight,rightWidth,topHeight);
                        this->childB->fillQuadTree(subChart);
                    }
                    break;
                case BOTTOM_RIGHT:
                    if(chart->width != 1){
                        this->childC = createNode();
                        PointChart* subChart = new PointChart;
                        initPointChart(subChart, rightWidth, bottomHeight);
                        fillSubChart(chart,subChart,rightWidth,bottomHeight,rightWidth,0);
                        this->childC->fillQuadTree(subChart);
                    }
                    break;
                case BOTTOM_LEFT:
                    this->childD = createNode();
                    PointChart* subChart = new PointChart;
                    initPointChart(subChart, leftWidth, bottomHeight);
                    fillSubChart(chart,subChart,leftWidth,bottomHeight,0,0);
                    this->childD->fillQuadTree(subChart);
                    break;
            }
        }
        freePointChart(chart);
    }
}

void Node::fillSubChart(PointChart* chart, PointChart* subChart, int width, int height, int widthStart,int heightStart){
    for(int i=heightStart; i<heightStart+height; i++){
        subChart->points[i-heightStart] = new Point3D[width];
        for(int j=widthStart; j<widthStart+width;j++){
            subChart->points[i-heightStart][j-widthStart] = chart->points[i][j];
        }
    }
}