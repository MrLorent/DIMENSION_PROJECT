#ifndef DRAW_H
#define DRAW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "QuadTree.h"
#include "objects.h"
#include "geometry.h"

void initTextureLevels(float min, float max);
void loadTextures(GLuint textures[16]);
GLuint creaTexture(char* path);

void initLODLevels(float max);
void glDrawRepere(float length);
bool LevelOfDetailsReached(QuadTree* quad, Point3D position);
void dealWithCracks(QuadTree* quad, Point3D position, int closest, float LOD_LEVEL);
void glDrawHeightMap(QuadTree* quadTree, Camera* camera, GLuint textures[16], Sun sun);
void glDrawTriangle(Point3D a, Point3D b, Point3D c, GLuint textures[16], Sun sun);
void glDrawTree(Point3D treePoint, float latitude, GLuint textures[16], Sun sun); 
void glDrawSkybox(float x,float y,float z,  GLuint textures[16]);
void glDrawSun (Sun* sun, GLuint textures[16]);
Color3f GetLight(Sun sun, Point3D a, Point3D b, Point3D c);

#endif // DRAW_H
