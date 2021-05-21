#ifndef GLOBALES_H
#define GLOBALES_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>

#include "../include/draw.h"
#include "../include/objects.h"
#include "../include/geometry.h"
#include "../include/preload.h"
#include "../include/QuadTree.h"

//---------VARIABLES GLOBALES--------------

#define STEP_ANGLE	M_PI/90.
#define STEP_PROF	M_PI/90.

extern Params params;

/* variables globales pour la gestion de la caméra */
extern Camera camera;

extern QuadTree* quadTree;

extern bool wireFrame;

//---------TEXTURE---------

char* textures[15];
GLuint texture[50];

#endif // GLOBALES_H