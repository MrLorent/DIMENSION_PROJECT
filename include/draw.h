#ifndef DRAW_H
#define DRAW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

GLuint creaTexture(char *chemin);
void glDrawRepere(float length);

#endif // DRAW_H