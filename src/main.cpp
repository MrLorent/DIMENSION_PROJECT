#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include "../include/geometry.h"
#include "../include/preload.h"
#include "../include/QuadTree.h"
#include "../include/application.h"


#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;

//---------VARIABLES GLOBALES--------------

#define STEP_ANGLE	M_PI/90.
#define STEP_PROF	M_PI/90.

/* variables globales pour la gestion de la caméra */
Camera camera;
float profondeur = 3;
float latitude = 0.0; //HORIZONTAL
float longitude = M_PI/2.0; //VERTICAL
float obj_rot = 0.0;

float latitudeCam = 0.0;
float longitudeCam = M_PI/2.0;


//---------INITIALISATION--------------

static void init() {
	// POSITION CAMERA
	positionCamera.x = 0.0;
	positionCamera.y = 0.0;
	positionCamera.z = 0.0;
	profondeur = 4;
	latitude = 0.0;
	longitude = M_PI/2.0;
	obj_rot = 0.0;

	// POSITION REGARD CAMERA
	latitudeCam = 0.0;
	longitudeCam = M_PI/2.0;


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
	glTranslatef(3.0,0.0,0.0);
	glBegin(GL_QUADS);

    glColor3ub(255,0,0); //face rouge
    glVertex3d(1,1,1);
    glVertex3d(1,1,-1);
    glVertex3d(-1,1,-1);
    glVertex3d(-1,1,1);

    glColor3ub(0,255,0); //face verte
    glVertex3d(1,-1,1);
    glVertex3d(1,-1,-1);
    glVertex3d(1,1,-1);
    glVertex3d(1,1,1);

    glColor3ub(0,0,255); //face bleue
    glVertex3d(-1,-1,1);
    glVertex3d(-1,-1,-1);
    glVertex3d(1,-1,-1);
    glVertex3d(1,-1,1);

    glColor3ub(255,255,0); //face jaune
    glVertex3d(-1,1,1);
    glVertex3d(-1,1,-1);
    glVertex3d(-1,-1,-1);
    glVertex3d(-1,-1,1);

    glColor3ub(0,255,255); //face cyan
    glVertex3d(1,1,-1);
    glVertex3d(1,-1,-1);
    glVertex3d(-1,-1,-1);
    glVertex3d(-1,1,-1);

    glColor3ub(255,0,255); //face magenta
    glVertex3d(1,-1,1);
    glVertex3d(1,1,1);
    glVertex3d(-1,1,1);
    glVertex3d(-1,-1,1);

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
	gluLookAt(
		positionCamera.x,
		positionCamera.y,
		positionCamera.z,
		positionCamera.x + cos(latitude)*sin(longitude), 	// CAMERA POSITION X
		positionCamera.y + sin(latitude)*sin(longitude),	// CAMERA POSITION Y
		positionCamera.z + cos(longitude),					// CAMERA POSITION Z
		0.0,
		0.0,
		1.0
	);

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
			if (longitude > STEP_ANGLE) longitude -= STEP_ANGLE;
			break;
		case GLUT_KEY_DOWN :
			if(longitude < M_PI-STEP_ANGLE) longitude += STEP_ANGLE;
			break;
		case GLUT_KEY_LEFT :
			latitude += STEP_ANGLE;
			break;
		case GLUT_KEY_RIGHT :
			latitude -= STEP_ANGLE;
			break;
		default:
			if(GLUT_ACTIVE_SHIFT){
				positionCamera.z -= 1;
			}
			printf("Appui sur une touche spéciale\n");
	}

	glutPostRedisplay();
}

static void kbdFunc(unsigned char c, int x, int y) {
	/* sortie du programme si utilisation des touches ESC, */
	/* 'q' ou 'Q'*/
	switch(c) {
		case 27 :
			exit(0);
			break;
		case 'Z' : case 'z' :
			positionCamera.x += 1. * cos(latitude);
			positionCamera.y += 1. * sin(latitude);
			break;
		case 'S' : case 's' : 
			positionCamera.x -= 1. * cos(latitude);
			positionCamera.y -= 1. * sin(latitude);
			break;
		case 'Q' : case 'q' : 
			positionCamera.x += 1. * cos(latitude + M_PI/2);
			positionCamera.y += 1. * sin(latitude + M_PI/2);
			break;
		case 'D' : case 'd' : 
			positionCamera.x -= 1. * cos(latitude + M_PI/2);
			positionCamera.y -= 1. * sin(latitude + M_PI/2);
			break;
		case ' ' :
			positionCamera.z += 1;
			break;
		default:
			printf("Appui sur la touche %c\n",c);
	}
	glutPostRedisplay();
}


int main (int argc, char** argv){
	QuadTree quadTree = createQuadTree();
  	Params params = createParams();
  	PointChart heightMap;
  
  	initParams(&params);
  	loadHeightMap(params, &heightMap);
	quadTree.fillQuadTree(&heightMap);

	// printPoint3D(quadTree.a);
	// printPoint3D(quadTree.b);
	// printPoint3D(quadTree.c);
	// printPoint3D(quadTree.d);
	// cout << endl;
	// printPoint3D(quadTree.getChildA()->a);
	// printPoint3D(quadTree.getChildA()->b);
	// printPoint3D(quadTree.getChildA()->c);
	// printPoint3D(quadTree.getChildA()->d);
	// cout << endl;
	// printPoint3D(quadTree.getChildB()->a);
	// printPoint3D(quadTree.getChildB()->b);
	// printPoint3D(quadTree.getChildB()->c);
	// printPoint3D(quadTree.getChildB()->d);
	// cout << endl;
	// printPoint3D(quadTree.getChildC()->a);
	// printPoint3D(quadTree.getChildC()->b);
	// printPoint3D(quadTree.getChildC()->c);
	// printPoint3D(quadTree.getChildC()->d);
	// cout << endl;
	// printPoint3D(quadTree.getChildD()->a);
	// printPoint3D(quadTree.getChildD()->b);
	// printPoint3D(quadTree.getChildD()->c);
	// printPoint3D(quadTree.getChildD()->d);

	/*/* traitement des paramètres du programme propres à GL */
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
	glutKeyboardFunc(kbdFunc);
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
