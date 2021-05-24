#ifndef DRAW_H
#define DRAW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "QuadTree.h"
#include "objects.h"

void initTextureLevels(float min, float max);
void loadTextures(GLuint textures[15]);
GLuint creaTexture(char* path);

void initLODLevels(float max);
void glDrawRepere(float length);
void glDrawHeightMap(QuadTree* quadTree, Camera* camera, GLuint textures[15]);
bool LevelOfDetailsReached(QuadTree* quad, Point3D position);
void dealWithCracks(QuadTree* quad, Point3D position, int closest, float LOD_LEVEL);
void glDrawTriangle(Point3D a, Point3D b, Point3D c, GLuint textures[15]);
void glDrawTree(Point3D treePoint, float latitude, GLuint textures[15]);
void glDrawSkybox(float x,float y,float z,  GLuint textures[15]);

#endif // DRAW_H
