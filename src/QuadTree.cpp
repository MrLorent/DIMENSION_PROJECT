#include "../include/preload.h"
#include "../include/QuadTree.h"

/*################ CONSTRUCTEURS ################*/

// Créer un nouveaux node à partir de quatres 
// points données
Node* createQuadTree(Point3D a, Point3D b, Point3D c, Point3D d, PointChart * heightMap, Params params){
    int width = b.x - a.x;
    int height = a.y - d.y;

    if(width == 1 && height == 1){
        return NULL;
    }else{
        // CREATION DU QUADTREE
        Node* newNode = new Node;

        newNode->a = a; //createMapPoint(a, params);
        newNode->b = b; //createMapPoint(b, params);
        newNode->c = c; //createMapPoint(c, params);
        newNode->d = d; //createMapPoint(d, params);

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
                    if(height != 1){
                        newNode->childA = createQuadTree(
                            a,
                            heightMap->points[(int) a.y][(int) a.x + leftWidth],
                            heightMap->points[(int) a.y - topHeight][(int) a.x + leftWidth],
                            heightMap->points[(int) a.y - topHeight][(int) a.x],
                            heightMap,
                            params
                        );
                    }else{
                        newNode->childA = NULL;
                    }
                    break;
                case TOP_RIGHT:
                    if(height != 1 && width != 1){
                        newNode->childB = createQuadTree(
                            heightMap->points[(int) b.y][(int) b.x - rightWidth],
                            b,
                            heightMap->points[(int) b.y - topHeight][(int) b.x],
                            heightMap->points[(int) b.y - topHeight][(int) b.x - rightWidth],
                            heightMap,
                            params
                        );
                    }else{
                        newNode->childB = NULL;
                    }
                    break;
                case BOTTOM_RIGHT:
                    if(width != 1){
                        newNode->childC = createQuadTree(
                            heightMap->points[(int) c.y + bottomHeight][(int) c.x - rightWidth],
                            heightMap->points[(int) c.y + bottomHeight][(int) c.x],
                            c,
                            heightMap->points[(int) c.y][(int) c.x - rightWidth],
                            heightMap,
                            params
                        );
                    }else{
                        newNode->childC = NULL;
                    }
                    break;
                case BOTTOM_LEFT:
                    newNode->childD = createQuadTree(
                        heightMap->points[(int) d.y + bottomHeight][(int) d.x],
                        heightMap->points[(int) c.y + bottomHeight][(int) d.x + leftWidth],
                        heightMap->points[(int) d.y][(int) d.y + leftWidth],
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
    if(!this->childA && !this->childB && !this->childC && !this->childD){
        return true;
    }else{
        return false;
    }
}