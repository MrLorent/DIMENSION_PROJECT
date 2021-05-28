#include "../include/objects.h"

void initCamera(Camera* cam, Params params, Point3D position)
{
    // PARAMETRE DE POSITION
    cam->height = 0.25;
	cam->position.x = position.x;
	cam->position.y = position.y;
	cam->position.z = position.z + cam->height;
    
    /* Parametre de position fixe */
	cam->closestMapPoint = position;
	cam->locked = true;

    // PARAMETRES GLUT
    cam->fovV = params.fovV;
	cam->zNear = params.zNear;
	cam->zFar = params.zFar;
	/* Vecteur up : vecteur normal au plan du sol (0;0;1) */
	cam->up.x = 0;
	cam->up.y = 0;
	cam->up.z = 1;
	/* Paramètre de position du regard */
	cam->latitude = 0.0;
	cam->longitude = M_PI/2.0;
}

float Camera::getFovHInRadian(){
    return this->fovH * M_PI/ 180.;
}

float Camera::getFovVInRadian(){
    return this->fovV * M_PI/180.;
}

bool Camera::sees(Point3D a,Point3D b,Point3D c,Point3D d){
    // CALCUL DU PERIMETRE DE CHAMP DE VISION
    
    float halfFovH = this->getFovHInRadian()/2;
    float limitLength = sqrt(this->zFar * this->zFar + (tan(halfFovH) * this->zFar) * (tan(halfFovH) * this->zFar));

    Point3D cam = createPoint(
        this->position.x + 1.25 * cos(this->latitude + M_PI),
        this->position.y + 1.25 * sin(this->latitude + M_PI),
        this->position.x
    );

    Point3D left = createPoint(
        this->position.x + limitLength * cos(this->latitude + halfFovH),
        this->position.y + limitLength * sin(this->latitude + halfFovH),
        this->position.z
    );
    
    Point3D right = createPoint(
        this->position.x + limitLength * cos(this->latitude - halfFovH),
        this->position.y + limitLength * sin(this->latitude - halfFovH),
        this->position.z
    );

    this->leftLimit = createVectorFromPoints(cam, left);
    this->frontLimit = createVectorFromPoints(left, right);
    this->rightLimit = createVectorFromPoints(right, cam);

    Point3D pointsToCheck[4] = { a, b, c, d };

    if(allOnLeft(cam, this->leftLimit, pointsToCheck))
    {
        return false;
    }
    else if (allOnLeft(left, this->frontLimit, pointsToCheck))
    {
        return false;
    }
    else if (allOnLeft(right, this->rightLimit, pointsToCheck))
    {
        return false;
    }
    else
    {
        return true;
    }
       
}

void Camera::adjustHeight(Node* quad){
    float distance = quad->getDistanceFrom(this->position);
        if(distance < norm(createVectorFromPoints(this->position, this->closestMapPoint))){
            switch (quad->getClosestCorner(this->position))
            {
            case TOP_LEFT:
                this->closestMapPoint = quad->a;
                this->position.z = this->closestMapPoint.z + this->height;
                break;
            case TOP_RIGHT:
                this->closestMapPoint = quad->b;
                this->position.z = this->closestMapPoint.z + this->height;
                break;
            case BOTTOM_RIGHT:
                this->closestMapPoint = quad->c;
                this->position.z = this->closestMapPoint.z + this->height;
                break;
            case BOTTOM_LEFT:
                this->closestMapPoint = quad->d;
                this->position.z = this->closestMapPoint.z + this->height;
                break;
            default:
                break;
            }
        }
}

void initSun(Sun* sun, float mapWidth, float mapHeigh) {
    sun->origin.x = mapWidth/2;
    sun->origin.y = mapHeigh/2;
    sun->origin.z = 0;
    sun->color = createColor(255,255,225);
    sun->shininess = 5.0;
    sun->longitude = -M_PI/6;
    sun->radius = mapHeigh;
    sun->position.x = sun->origin.x;
    sun->position.y = sun->origin.y - sun->radius * cos(sun->longitude);
    sun->position.z = sun->origin.z - sun->radius * sin(sun->longitude);
    sun->moving = false;
}

/*############## LUMIERE ##############*/

Color3f Sun::getLight(Point3D a, Point3D b, Point3D c){
    Vector3D v1 = createVectorFromPoints(a,b);
    Vector3D v2 = createVectorFromPoints(a,c);
    Vector3D normale = prodVect(v1,v2);
    Point3D centre = createPoint((a.x+b.x+c.x)/3, (a.y+b.y+c.y)/3, (a.z+b.z+c.z)/3);
    Vector3D vectSunShine = createVectorFromPoints(this->position, centre);
    normale = normalize(normale);
    Color3f triangleLight = multColor(this->color, this->shininess * dot(normale, normalize(vectSunShine))/ (norm(vectSunShine)*norm(vectSunShine)));
    
    return triangleLight;
}

/*############## ARBRES ##############*/

void initTreeChart(TreeChart* chart, int NB_TREES)
{
    chart->nbTrees = 0;
    chart->trees = new Point3D[NB_TREES];
}

void registerTrees(QuadTree* quadTree, TreeChart* treesToDraw, Point3D cam)
{
    Point3D inOrderPoints[4];
    quadTree->orderByDistance(inOrderPoints, cam);

    for(int i = 0; i < 4; i++)
    {
        if(inOrderPoints[i].tree)
        {
            treesToDraw->trees[treesToDraw->nbTrees] = inOrderPoints[i];
            treesToDraw->nbTrees++;
        }
    }
}