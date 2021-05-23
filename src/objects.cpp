#include "../include/objects.h"

bool Camera::sees(Point3D a,Point3D b,Point3D c,Point3D d){
    // CALCUL DU PERIMETRE DE CHAMP DE VISION
    
    float limitLength = sqrt(this->zFar * this->zFar + (tan(this->fov*M_PI/360.)/this->zFar) * (tan(this->fov*M_PI/360.)/this->zFar)); 

    Point3D left = createPoint(
        this->position.x + limitLength * cos(this->latitude + this->fov*M_PI/360.),
        this->position.y + limitLength * sin(this->latitude + this->fov*M_PI/360.),
        this->position.z
    );
    
    Point3D right = createPoint(
        this->position.x + limitLength * cos(this->latitude - this->fov*M_PI/360.),
        this->position.y + limitLength * sin(this->latitude - this->fov*M_PI/360.),
        this->position.z
    );

    this->rightLimit = createVectorFromPoints(this->position, right);
    this->frontLimit = createVectorFromPoints(right, left);
    this->leftLimit = createVectorFromPoints(left, this->position);

    Point3D pointsToCheck[4] = { a, b, c, d };

    if(allOnLeft(this->position, this->leftLimit, pointsToCheck))
    {
        return false;
    }
    else if (allOnLeft(this->position, this->frontLimit, pointsToCheck))
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

