#ifndef DRAW_H
#define DRAW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "../include/QuadTree.h"

GLuint creaTexture(char *chemin);
void glDrawRepere(float length, GLuint texture[15]);
void glDrawHeightMap(QuadTree* quadTree);

#endif // DRAW_H