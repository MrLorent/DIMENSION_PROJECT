#ifndef DRAW_H
#define DRAW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "color.h"
#include "geometry.h"
#include "QuadTree.h"
#include "objects.h"

struct TreeChart {
    int nbTrees;
    Point3D* trees;
};

void initTreeChart(TreeChart* chart, int NB_TREES);

void initTextureLevels(float min, float max);
void loadTextures(GLuint mapTextures[4], GLuint treeTextures[4], GLuint skyboxTextures[12]);
GLuint creaTexture(char* path);
void bindTexture(float height, GLuint textures[4]);

void initLODLevels(float max);
bool LevelOfDetailsReached(QuadTree* quad, Point3D position);
void dealWithCracks(QuadTree* quad, Point3D position, int closest, float LOD_LEVEL);

void glDrawMenu(GLuint menuTexture[1]);
void glDrawRepere(float length);
void glDrawSkybox(float x, float y, float z,  GLuint skyboxTextures[12], float zFar, bool wireframe);

void glDrawHeightMap(QuadTree* quadTree, Camera* camera, GLuint mapTextures[4], TreeChart* treesToDraw, Sun sunShine, bool wireframe);
void glDrawTriangle(Point3D a, Point3D b, Point3D c, int quadLevel, GLuint mapTextures[4], Sun sunShine, bool wireframe);

void registerTrees(QuadTree* quadTree, TreeChart* treesToDraw, Point3D cam);
void glDrawTrees(TreeChart* treePoint, float latitude, GLuint treeTextures[4], Sun sunShine);

#endif // DRAW_H
