#include "../include/QuadTree.h"

/*################ CONSTRUCTEURS ################*/

// Créer un nouveaux node à partir de quatres 
// points données
Node* createQuadTree(Point3D a, Point3D b, Point3D c, Point3D d, PointChart * heightMap, Params params){
    int width = b.x - a.x;
    int height = a.y - d.y;

    // CREATION DU QUADTREE
    Node* newNode = new Node;

    /* On instancie la height du node à 0 pour l'instant */
    newNode->height = 0;

    /* On instancie les coins du quad qui serviront de références */
    newNode->a = createMapPoint(a, params);
    newNode->b = createMapPoint(b, params);
    newNode->c = createMapPoint(c, params);
    newNode->d = createMapPoint(d, params);

    /* On instancie les coins du quad qui serviront au dessin de la map */
    newNode->tmpA = newNode->a;
    newNode->tmpB = newNode->b;
    newNode->tmpC = newNode->c;
    newNode->tmpD = newNode->d;

    /* On indique si un des quatres points du quad contient un tree */
    newNode->hasTree = a.tree || b.tree || c.tree || d.tree;

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

int Node::getHeight(){
    if(this->isLeaf()){
        this->height = 1;
        return 1;
    }else{
        int heightA = 0, heightB = 0, heightC = 0, heightD = 0;

        // RECUPERATION DES HAUTEURS DES ENFANTS
        if(this->childA) heightA = this->childA->getHeight();
        if(this->childB) heightB = this->childB->getHeight();
        if(this->childC) heightC = this->childC->getHeight();
        if(this->childD) heightD = this->childD->getHeight();

        // PREMIERE COMPARAISON
        if(heightA < heightB) heightA = heightB;
        if(heightC < heightD) heightC = heightD;

        // SECONDE ET DERNIERE COMPARAISON
        if(heightA > heightC)
        {
            this->height = heightA +1;
            return heightA + 1;
        }
        else
        {
            this->height = heightC +1;
            return heightC + 1;
        }
    }
}

float Node::getDistanceFrom(Point3D position, int* closest){
    int closestA = TOP_LEFT;
    int closestC = BOTTOM_RIGHT;

    // RECUPERATION DE LA DISTANCE A CHACUN DES ANGLES DU QUAD
    float distanceA = norm(createVectorFromPoints(position, this->a));
    float distanceB = norm(createVectorFromPoints(position, this->b));
    float distanceC = norm(createVectorFromPoints(position, this->c));
    float distanceD = norm(createVectorFromPoints(position, this->d));

    // PREMIERE COMPARAISON
    if(distanceA > distanceB)
    {
        closestA = TOP_RIGHT;
        distanceA = distanceB;
    }

    if(distanceC > distanceD)
    {
        closestC = BOTTOM_LEFT;
        distanceC = distanceD;
    }

    // SECONDE ET DERNIERE COMPARAISON
    if(distanceA < distanceC){
        *closest = closestA;
        return distanceA;
    }
    else
    {
        *closest = closestC;
        return distanceC;
    }
}

void Node::initTmpPoints(){
    this->tmpA.z = this->a.z;
    this->tmpB.z = this->b.z;
    this->tmpC.z = this->c.z;
    this->tmpD.z = this->d.z;

    if(this->childA) this->childA->initTmpPoints();
    if(this->childB) this->childB->initTmpPoints();
    if(this->childC) this->childC->initTmpPoints();
    if(this->childD) this->childD->initTmpPoints();
}