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
#include "color.h"


struct TreeChart {
    int nbTrees;
    Point3D* trees;
};

void initTreeChart(TreeChart* chart, int NB_TREES);

void initTextureLevels(float min, float max);
void loadTextures(GLuint textures[15]);
GLuint creaTexture(char* path);

void initLODLevels(float max);
bool LevelOfDetailsReached(QuadTree* quad, Point3D position);
void dealWithCracks(QuadTree* quad, Point3D position, int closest, float LOD_LEVEL);

void glDrawRepere(float length);
void glDrawSkybox(float x,float y,float z,  GLuint textures[15],float zFar);

void glDrawHeightMap(QuadTree* quadTree, Camera* camera, GLuint textures[15], TreeChart* treesToDraw, Sun sunShine, bool wireframe);
void glDrawTriangle(Point3D a, Point3D b, Point3D c, int quadLevel, GLuint textures[15], Sun sunShine, bool wireframe);

void registerTrees(QuadTree* quadTree, TreeChart* treesToDraw, Point3D cam);
void glDrawTrees(TreeChart* treePoint, float latitude, GLuint textures[15], Sun sunShine); 

Color3f GetLight(Sun sunShine, Point3D a, Point3D b, Point3D c);
void glDrawSun(Sun* sun, GLuint textures[16]);

#endif // DRAW_H
