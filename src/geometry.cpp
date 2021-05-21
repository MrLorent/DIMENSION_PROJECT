#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include "../include/geometry.h"

// Construit le point 
Point3D createPoint(float x, float y, float z){
    Point3D newPoint ;
    newPoint.x=x;
    newPoint.y=y;
    newPoint.z=z;
    return newPoint;
}

void printPoint3D(Point3D p)
{
    cout << "(" << p.x << ";" << p.y << ";" << p.z << ")" << endl;
}

void printVector3D(Vector3D v)
{
    cout << "(" << v.x << ";" << v.y << ";" << v.z << ")" << endl;
}

// Construit le quad 
Quad createQuad(Point3D hautGauche, Point3D hautDroit, Point3D basDroit, Point3D basGauche){
    Quad newQuad ;
    newQuad.hautGauche = hautGauche;
    newQuad.hautDroit = hautDroit;
    newQuad.basDroit = basDroit;
    newQuad.basGauche = basGauche;
    return newQuad;
}
