#include "../include/objects.h"

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

float Camera::getFovHInRadian(){
    return this->fovH * M_PI/ 180.;
}

float Camera::getFovVInRadian(){
    return this->fovV * M_PI/180.;
}

bool allOnLeft(Point3D position, Vector3D limit, Point3D points[4])
{
    int nbChecked = 0;

    while (nbChecked < 4)
    {
        if(det(limit,createVectorFromPoints(position,points[nbChecked])) < 0.0)
        {
            return false;
        }
        nbChecked++;
    }

    return true;
}

void initSun(Sun* sun, float mapWidth, float mapHeigh) {
    sun->position.x = mapWidth/2;
    sun->position.y = -1;
    sun->position.z = 0;
    sun->color = createColor(255,255,255);
    sun->moving = false;
    sun->radius = mapHeigh/2+1 ;
}

