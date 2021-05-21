#include "../include/QuadTree.h"

/*################ CONSTRUCTEURS ################*/

// Créer un nouveaux node à partir de quatres 
// points données
Node* createQuadTree(Point3D a, Point3D b, Point3D c, Point3D d, PointChart * heightMap, Params params){
    int width = b.x - a.x;
    int height = a.y - d.y;

    // CREATION DU QUADTREE
    Node* newNode = new Node;

    newNode->a = createMapPoint(a, params);
    newNode->b = createMapPoint(b, params);
    newNode->c = createMapPoint(c, params);
    newNode->d = createMapPoint(d, params);

    if(width == 1 && height == 1)
    {
        newNode->childA = NULL;
        newNode->childB = NULL;
        newNode->childC = NULL;
        newNode->childD = NULL;
        
        return newNode;
    }
    else
    {
        int leftWidth = 1, rightWidth = 1;
        int topHeight = 1, bottomHeight = 1;

        if(height != 1){
            topHeight = height/2;
            bottomHeight = height - topHeight;
        }

        if(width != 1){
            rightWidth = width/2;
            leftWidth = width - rightWidth;
        }

        for(int child=TOP_LEFT; child <= BOTTOM_LEFT; child++){
            switch(child){
                case TOP_LEFT:
                    newNode->childA = createQuadTree(
                        a,
                        heightMap->points[(int) a.y][(int) a.x + leftWidth],
                        heightMap->points[(int) a.y - topHeight][(int) a.x + leftWidth],
                        heightMap->points[(int) a.y - topHeight][(int) a.x],
                        heightMap,
                        params
                    );
                    break;
                case TOP_RIGHT:
                    newNode->childB = createQuadTree(
                        heightMap->points[(int) b.y][(int) b.x - rightWidth],
                        b,
                        heightMap->points[(int) b.y - topHeight][(int) b.x],
                        heightMap->points[(int) b.y - topHeight][(int) b.x - rightWidth],
                        heightMap,
                        params
                    );
                    break;
                case BOTTOM_RIGHT:
                    newNode->childC = createQuadTree(
                        heightMap->points[(int) c.y + bottomHeight][(int) c.x - rightWidth],
                        heightMap->points[(int) c.y + bottomHeight][(int) c.x],
                        c,
                        heightMap->points[(int) c.y][(int) c.x - rightWidth],
                        heightMap,
                        params
                    );
                    break;
                case BOTTOM_LEFT:
                    newNode->childD = createQuadTree(
                        heightMap->points[(int) d.y + bottomHeight][(int) d.x],
                        heightMap->points[(int) d.y + bottomHeight][(int) d.x + leftWidth],
                        heightMap->points[(int) d.y][(int) d.x + leftWidth],
                        d,
                        heightMap,
                        params
                    );
                    break;
            }
        }

        return newNode;
    }
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
    return this->childA == NULL && this->childB == NULL && this->childC == NULL && this->childD == NULL;
}