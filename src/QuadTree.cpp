#include "../include/QuadTree.h"

float LOD_LEVEL_1 = 0;
float LOD_LEVEL_2 = 0;
float LOD_LEVEL_3 = 0;
float LOD_LEVEL_4 = 0;

/*################ CONSTRUCTEURS ################*/

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

    /* On instancie les enfants à NULL de base */
    newNode->childA = NULL;
    newNode->childB = NULL;
    newNode->childC = NULL;
    newNode->childD = NULL;

    /* On indique si un des quatres points du quad contient un tree */
    newNode->hasTree = a.tree || b.tree || c.tree || d.tree;

    if(width == 1 && height == 1)
    {   
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
                    if(height != 1){
                        newNode->childA = createQuadTree(
                            a,
                            heightMap->points[(int) a.y][(int) a.x + leftWidth],
                            heightMap->points[(int) a.y - topHeight][(int) a.x + leftWidth],
                            heightMap->points[(int) a.y - topHeight][(int) a.x],
                            heightMap,
                            params
                        );
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
                    }
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

Node* Node::getChildA() {
    return this->childA;
};

Node* Node::getChildB() {
    return this->childB;
};

Node* Node::getChildC() {
    return this->childC;
};

Node* Node::getChildD() {
    return this->childD;
};

/*################ METHODES ################*/

bool Node::isLeaf(){
    return this->childA == NULL && this->childB == NULL && this->childC == NULL && this->childD == NULL;
}

void Node::initNodesHeight(){
    if(this->isLeaf()){
        this->height = 1;
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
        }
        else
        {
            this->height = heightC +1;
        }

        if(this->childA) this->childA->initNodesHeight();
        if(this->childB) this->childB->initNodesHeight();
        if(this->childC) this->childC->initNodesHeight();
        if(this->childD) this->childD->initNodesHeight();
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

int Node::getClosestCornerFrom(Point3D cam)
{
    int closest1 = TOP_LEFT;
    int closest2 = BOTTOM_RIGHT;

    // RECUPERATION DE LA DISTANCE A CHACUN DES ANGLES DU QUAD
    float distanceA = norm(createVectorFromPoints(cam, this->a));
    float distanceB = norm(createVectorFromPoints(cam, this->b));
    float distanceC = norm(createVectorFromPoints(cam, this->c));
    float distanceD = norm(createVectorFromPoints(cam, this->d));

    // PREMIERE COMPARAISON
    if(distanceA > distanceB)
    {
        closest1 = TOP_RIGHT;
        distanceA = distanceB;
    }

    if(distanceC > distanceD)
    {
        closest2 = BOTTOM_LEFT;
        distanceC = distanceD;
    }

    // SECONDE ET DERNIERE COMPARAISON
    if(distanceA < distanceC)
    {
        return closest1;
    }
    else
    {
        return closest2;
    }
}

float Node::getDistanceFrom(Point3D position){

    // RECUPERATION DE LA DISTANCE A CHACUN DES ANGLES DU QUAD
    float distanceA = norm(createVectorFromPoints(position, this->a));
    float distanceB = norm(createVectorFromPoints(position, this->b));
    float distanceC = norm(createVectorFromPoints(position, this->c));
    float distanceD = norm(createVectorFromPoints(position, this->d));

    // PREMIERE COMPARAISON
    if(distanceA > distanceB)
    {
        distanceA = distanceB;
    }

    if(distanceC > distanceD)
    {
        distanceC = distanceD;
    }

    // SECONDE ET DERNIERE COMPARAISON
    if(distanceA < distanceC){
        return distanceA;
    }
    else
    {
        return distanceC;
    }
}

void Node::orderByDistance(Point3D chart[4], Point3D cam)
{
    float distanceA, distanceB, distanceC, distanceD;
    int closest = this->getClosestCornerFrom(cam);

    switch (closest)
    {
    case TOP_LEFT:
        distanceB = norm(createVectorFromPoints(cam, this->b));
        distanceD = norm(createVectorFromPoints(cam, this->d));

        chart[0] = this->tmpA;
        if(distanceB < distanceD){
            chart[1] = this->tmpB;
            chart[2] = this->tmpD;
        }else{
            chart[1] = this->tmpD;
            chart[2] = this->tmpB;
        }
        chart[3] = this->tmpC;
        break;

    case TOP_RIGHT:
        distanceA = norm(createVectorFromPoints(cam, this->a));
        distanceC = norm(createVectorFromPoints(cam, this->c));

        chart[0] = this->tmpB;
        if(distanceA < distanceC){
            chart[1] = this->tmpA;
            chart[2] = this->tmpC;
        }else{
            chart[1] = this->tmpC;
            chart[2] = this->tmpA;
        }
        chart[3] = this->tmpD;
        break;

    case BOTTOM_RIGHT:
        distanceB = norm(createVectorFromPoints(cam, this->b));
        distanceD = norm(createVectorFromPoints(cam, this->d));

        chart[0] = this->tmpC;
        if(distanceB < distanceD){
            chart[1] = this->tmpB;
            chart[2] = this->tmpD;
        }else{
            chart[1] = this->tmpD;
            chart[2] = this->tmpB;
        }
        chart[3] = this->tmpA;
        break;

    case BOTTOM_LEFT:
        distanceA = norm(createVectorFromPoints(cam, this->a));
        distanceC = norm(createVectorFromPoints(cam, this->c));

        chart[0] = this->tmpD;
        if(distanceA < distanceC){
            chart[1] = this->tmpA;
            chart[2] = this->tmpC;
        }else{
            chart[1] = this->tmpC;
            chart[2] = this->tmpA;
        }
        chart[3] = this->tmpB;
        break;
    
    default:
        break;
    }
}

void Node::fixTopCrack()
{
    if(this->childA && this->childB)
    {
        this->getChildA()->tmpB.z = (this->a.z + this->b.z)/2;
        this->getChildB()->tmpA.z = (this->a.z + this->b.z)/2;
    }
    else if(this->childC)
    {
        this->getChildC()->tmpA.z = (this->a.z + this->b.z)/2;
        this->getChildD()->tmpB.z = (this->a.z + this->b.z)/2;
    }
}

void Node::fixRightCrack()
{
    if(this->childC && this->childB)
    {
        this->getChildB()->tmpC.z = (this->b.z + this->c.z)/2;
        this->getChildC()->tmpB.z = (this->b.z + this->c.z)/2;
    }
    else if(this->childA)
    {
        this->getChildA()->tmpC.z = (this->b.z + this->c.z)/2;
        this->getChildD()->tmpB.z = (this->b.z + this->c.z)/2;
    } 
}

void Node::fixBottomCrack()
{
    if(this->childC)
    {
        this->getChildC()->tmpD.z = (this->d.z + this->c.z)/2;
        this->getChildD()->tmpC.z = (this->d.z + this->c.z)/2;
    }
}

void Node::fixLeftCrack()
{
    if(this->childA){
        this->getChildA()->tmpD.z = (this->a.z + this->d.z)/2;
        this->getChildD()->tmpA.z = (this->a.z + this->d.z)/2;
    }
}


/*################ LOD ################*/

void initLODLevels(float zFar){
  LOD_LEVEL_1 = zFar * 3/32;
  LOD_LEVEL_2 = zFar * 6/32;
  LOD_LEVEL_3 = zFar * 9/32;
  LOD_LEVEL_4 = zFar * 18/32;
}

bool LevelOfDetailsReached(QuadTree* quad, Point3D position){

    int closestPoint = quad->getClosestCornerFrom(position);
    float distance = quad->getDistanceFrom(position);

    // ON DETERMINE SI ON DOIT CORRIGER LES CRACKS OU PAS
    if(distance <= LOD_LEVEL_1 && quad->height == 2)
    {
        dealWithCracks(quad, position, closestPoint, LOD_LEVEL_1);
    }
    else if (distance <= LOD_LEVEL_2 && quad->height == 3)
    {
        dealWithCracks(quad, position, closestPoint, LOD_LEVEL_2);
    }
    else if (distance <= LOD_LEVEL_3 && quad->height == 4)
    {
        dealWithCracks(quad, position, closestPoint, LOD_LEVEL_3);
    }
    else if (distance <= LOD_LEVEL_4 && quad->height == 5)
    {
        dealWithCracks(quad, position, closestPoint, LOD_LEVEL_4);
    }
    
    // ON DETERMINE SI ON DOIT DESSINER OU CONTINUER A DESCENDRE
    if(quad->isLeaf())
    {
        return true;
    }
    else if (distance > LOD_LEVEL_1 && quad->height == 2)
    {
        return true;
    }
    else if (distance > LOD_LEVEL_2 && quad->height == 3)
    {
        return true;
    }
    else if (distance > LOD_LEVEL_3 && quad->height == 4)
    {
        return true;
    }
    else if (distance > LOD_LEVEL_4 && quad->height == 5)
    {
        return true;
    }
    else
    {
      return false;
    }
}

void dealWithCracks(QuadTree* quad, Point3D position, int closest, float LOD_LEVEL)
{
    float distanceA, distanceB, distanceC, distanceD;

    switch (closest)
    {
        case TOP_LEFT:
            distanceB = norm(createVectorFromPoints(position, quad->b));
            distanceD = norm(createVectorFromPoints(position, quad->d));

            if(distanceB > LOD_LEVEL)
            {
                quad->fixRightCrack();
            }

            if(distanceD > LOD_LEVEL)
            {
                quad->fixBottomCrack();              
            }
            break;

        case TOP_RIGHT:
            distanceA = norm(createVectorFromPoints(position, quad->a));
            distanceC = norm(createVectorFromPoints(position, quad->c));

            if(distanceA > LOD_LEVEL)
            {
                quad->fixLeftCrack();
            }

            if(distanceC > LOD_LEVEL)
            {
                quad->fixBottomCrack();
            }
            break;

        case BOTTOM_RIGHT:
            distanceB = norm(createVectorFromPoints(position, quad->b));
            distanceD = norm(createVectorFromPoints(position, quad->d));

            if(distanceB > LOD_LEVEL)
            {
                quad->fixTopCrack();
            }

            if(distanceD > LOD_LEVEL)
            {
                quad->fixLeftCrack();
            }
            break;

        case BOTTOM_LEFT:
            distanceA = norm(createVectorFromPoints(position, quad->a));
            distanceC = norm(createVectorFromPoints(position, quad->c));

            if(distanceA > LOD_LEVEL)
            {
                quad->fixTopCrack();
            }

            if(distanceC > LOD_LEVEL)
            {
                quad->fixRightCrack();
            }
            break;

        default:
          break;
    }
}