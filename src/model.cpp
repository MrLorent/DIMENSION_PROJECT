#include <iostream>
#include <fstream>
#include <cstring>
#include "../include/model.h"
#include <math.h>

// Construit le scene 
Scene createScene(){
    Scene s ;
    s.linkImg=NULL;
    s.xsize=0;
    s.ysize=0;
    s.zmin=0;
    s.zmax=0;
    s.znear=0;
    s.zfar=0;
    s.fov=0;
    return s;
}

// Construit le point 
Point createPoint(float x, float y, float z){
    Point newPoint ;
    newPoint.x=x;
    newPoint.y=y;
    newPoint.z=z;
    return newPoint;
}

// Construit le quad 
Quad createQuad(Point hautGauche,Point hautDroit,Point basDroit, Point basGauche){
    Quad newQuad ;
    newQuad.hautGauche=hautGauche;
    newQuad.hautDroit=hautDroit;
    newQuad.basDroit=basDroit;
    newQuad.basGauche=basGauche;
    return newQuad;
}
