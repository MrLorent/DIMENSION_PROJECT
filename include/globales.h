#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>

#include "../include/objects.h"
#include "../include/geometry.h"
#include "../include/preload.h"
#include "../include/QuadTree.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

//---------VARIABLES GLOBALES--------------

#define STEP_ANGLE	M_PI/90.
#define STEP_PROF	M_PI/90.

Params params;

/* variables globales pour la gestion de la caméra */
Camera camera;

QuadTree* quadTree;

bool wireFrame;

//---------TEXTURE---------

char *textures[15];
GLuint texture[50];