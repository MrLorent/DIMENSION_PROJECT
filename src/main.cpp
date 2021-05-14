#include <iostream>
#include <fstream>
#include <cstring>
#include "../include/model.h"
//#include "../include/QuadTree.h"
#include "math.h"


#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;

//---------VARIABLES GLOBALES--------------

#define STEP_ANGLE	M_PI/90.
#define STEP_PROF	M_PI/90.

/* variables globales pour la gestion de la caméra */
float profondeur = 3;
float latitude = 0.0;
float longitude = M_PI/2.;
float obj_rot = 0.0;


//---------INITIALISATION--------------

static void init() {

	profondeur = 3;
	latitude = M_PI/2.0;
	longitude = 0.0;
	obj_rot = 0.0;

	/* INITIALISATION DES PARAMETRES GL */
	/* couleur du fond (gris sombre) */
	glClearColor(0.3,0.3,0.3,0.0);
	/* activation du ZBuffer */
	glEnable( GL_DEPTH_TEST);

	/* lissage des couleurs sur les facettes */
	glShadeModel(GL_SMOOTH);

	/* INITIALISATION DE LA SCENE */
	//createCoordinates();
}




//---------FONCTION RESIZE DE LA PAGE--------------

static void reshapeFunc(int width,int height) {
	GLfloat  h = (GLfloat) width / (GLfloat) height ;
	
	/* dimension de l'écran GL */
	glViewport(0, 0, (GLint)width, (GLint)height);
	/* construction de la matrice de projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* définition de la camera */
	gluPerspective( 60.0, h, 0.01, 10.0 );			// Angle de vue, rapport largeur/hauteur, near, far

	/* Retour a la pile de matrice Modelview
			et effacement de celle-ci */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//-----------------DRAW ---------------

void glDrawRepere(float length) {
	// dessin du repère
	glBegin(GL_LINES);
		glColor3f(1.,0.,0.);
		glVertex3f(0.,0.,0.);
		glVertex3f(length,0.,0.);
		glColor3f(0.,1.,0.);
		glVertex3i(0.,0.,0.);
		glVertex3i(0.,length,0.);
		glColor3f(0.,0.,1.);
		glVertex3i(0.,0.,0.);
		glVertex3i(0.,0.,length);
	glEnd();
}

//------------------ AFFICHAGE -----------------------

static void drawFunc(void) { 
	/* reinitialisation des buffers : couleur et ZBuffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* modification de la matrice de la scène */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Debut du dessin de la scène */
	glPushMatrix();
	
	/* placement de la caméra */
	gluLookAt(profondeur*sin(longitude)*sin(latitude),profondeur*cos(latitude),profondeur*cos(longitude)*sin(latitude),
              0.0,0.0,0.0,
              0.0,1.0,0.0);

	glColor3f(1.0,0.0,0.0);
	glDrawRepere(2.0);

	float position[4] = {5.0,5.0,5.0,1.0};
	float black[3] = {0.0,0.0,0.0};
	float intensite[3] = {1000.0,1000.0,1000.0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_POSITION,position);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,intensite);
	glLightfv(GL_LIGHT0,GL_SPECULAR,black);
	//glLightf(GL_LIGHT0,GL_,black);
	//glLightf(GL_LIGHT0,GL_SPECULAR,black);

	glPushMatrix();
	glRotatef(obj_rot,0.0,1.0,0.0);
	glColor3f(1.0,1.0,1.0);
	
	glDisable(GL_LIGHTING);

	/* Fin du dessin */
	glPopMatrix();

	/* fin de la définition de la scène */
	glFinish();

	/* changement de buffer d'affichage */
	glutSwapBuffers();
}

//------------------ ECOUTEURS EVENEMENTS -----------------------


static void kbdSpFunc(int c, int x, int y) {
	/* sortie du programme si utilisation des touches ESC, */
	switch(c) {
		case GLUT_KEY_UP :
			if (latitude>STEP_ANGLE) latitude -= STEP_ANGLE;
			break;
		case GLUT_KEY_DOWN :
			if(latitude<M_PI-STEP_ANGLE) latitude += STEP_ANGLE;
			break;
		case GLUT_KEY_LEFT :
			longitude -= STEP_ANGLE;
			break;
		case GLUT_KEY_RIGHT :
			longitude += STEP_ANGLE;
			break;
		case GLUT_KEY_PAGE_UP :
			profondeur += STEP_PROF;
			break;
		case GLUT_KEY_PAGE_DOWN :
			if (profondeur>0.1+STEP_PROF) profondeur -= STEP_PROF;
			break;
		default:
			printf("Appui sur une touche spéciale\n");
	}
	glutPostRedisplay();
}


int main (int argc, char** argv)
{
  Params params = createParams();
  loadParams(&params);
  vector<vector<Point>> heightMap;
  int imageLoaded = loadHeightMap(params, heightMap);

	/* traitement des paramètres du programme propres à GL */
	glutInit(&argc, argv);
	/* initialisation du mode d'affichage :                */
	/* RVB + ZBuffer + Double buffer.                      */
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	/* placement et dimentions originales de la fenêtre */
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	/* ouverture de la fenêtre */
	if (glutCreateWindow("DIMENSION PROJECT") == GL_FALSE) {
		return 1;
	}

	init();

	/* association de la fonction callback de redimensionnement */
	glutReshapeFunc(reshapeFunc);
	/* association de la fonction callback d'affichage */
	glutDisplayFunc(drawFunc);
	/* association de la fonction callback d'événement du clavier */
	//glutKeyboardFunc(kbdFunc);
	/* association de la fonction callback d'événement du clavier (touches spéciales) */
	glutSpecialFunc(kbdSpFunc);
	/* association de la fonction callback d'événement souris */
	//glutMouseFunc(mouseFunc);
	/* association de la fonction callback de DRAG de la souris */
	//glutMotionFunc(motionFunc);

	//glutIdleFunc(idle);

	/* boucle principale de gestion des événements */
	glutMainLoop();
	/* Cette partie du code n'est jamais atteinte */
	return 0;


}
