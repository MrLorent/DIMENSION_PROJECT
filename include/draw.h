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
void loadTextures(GLuint textures[15]);
GLuint creaTexture(char* path);

void initLODLevels(float max);
void glDrawRepere(float length);
bool LevelOfDetailsReached(QuadTree* quad, Point3D position);
void glDrawHeightMap(QuadTree* quadTree, Camera* camera, GLuint textures[15], Light sunShine);
void glDrawTriangle(Point3D a, Point3D b, Point3D c, GLuint textures[15], Light sunShine);
void glDrawTree(Point3D treePoint, float latitude, GLuint textures[15], Light sunShine); 
void glDrawSkybox(float x,float y,float z,  GLuint textures[15]);
Color3f GetLight(Light sunShine, Point3D a, Point3D b, Point3D c);

#endif // DRAW_H
