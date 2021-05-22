#include "../include/objects.h"

bool Camera::sees(Point3D a,Point3D b,Point3D c,Point3D d){
    // CALCUL DU PERIMETRE DE CHAMP DE VISION
    float limitLength = sqrt(this->zFar * this->zFar + (tan(this->fov*M_PI/360.)/this->zFar) * (tan(this->fov*M_PI/360.)/this->zFar)); 

    this->leftLimit.x = -(this->position.x + limitLength * cos(this->latitude + this->fovForFrustum));
    this->leftLimit.y = -(this->position.y + limitLength * sin(this->latitude + this->fovForFrustum));
    this->leftLimit.z = -(this->position.z);

    this->rightLimit.x = this->position.x + limitLength * cos(this->latitude - this->fovForFrustum);
    this->rightLimit.y = this->position.y + limitLength * sin(this->latitude - this->fovForFrustum);
    this->rightLimit.z = this->position.z;

    Point3D pointsToCheck[4] = { a, b, c, d };

    if(allOnLeft(this->position, this->leftLimit, pointsToCheck))
    {
        return false;
    }
    else if (allOnLeft(this->position, this->rightLimit, pointsToCheck))
    {
        return false;
    }
    else
    {
        return true;
    }
       
}

bool allOnLeft(Point3D position, Vector3D limit, Point3D points[4])
{
    int nbChecked = 0;

    while (nbChecked < 4)
    {
        if(det(limit,createVectorFromPoints(position,points[nbChecked])) > 0.0)
        {
            return false;
        }
        nbChecked++;
    }

    return true;
}

// Crée un tableau d'arbres
void createTrees(Tree trees[6]){
    for (int i=0; i<6 ; i++){
        trees[i].x = rand() % 4 + (-1);
        trees[i].y= rand() % 4 + (-1);
        trees[i].z=0;
    }
};

