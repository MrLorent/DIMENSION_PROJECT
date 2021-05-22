#ifndef DRAW_H
#define DRAW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "QuadTree.h"
#include "objects.h"

GLuint creaTexture(char* path);
void glDrawRepere(float length, GLuint textures[10]);
void glDrawTrees(Tree trees[6], Camera camera, GLuint textures[10]);
void glDrawHeightMap(QuadTree* quadTree, Camera* camera);
void glDrawSkybox(float x,float y,float z,  GLuint textures[10]);

#endif // DRAW_H
