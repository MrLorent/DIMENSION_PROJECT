#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include <time.h> 

#include "../include/draw.h"
#include "../include/objects.h"
#include "../include/geometry.h"
#include "../include/preload.h"
#include "../include/QuadTree.h"

//---------VARIABLES GLOBALES--------------

#define STEP_ANGLE	M_PI/90.
#define STEP_PROF	M_PI/90.

Params params;

/* variables globales pour la gestion de la caméra */
Camera camera;

QuadTree* quadTree;

bool wireFrame;

Tree trees[6];


//---------TEXTURE---------

char* texturesLinks[10];
GLuint textures[10];

using namespace std;

//---------INITIALISATION--------------

static void init() {
	PointChart heightMap;

	// RECUPERATION DES PARAMETRES .TIMAC
	params = createParams();
  	readParams(&params, &camera);
  	loadHeightMap(&params, &heightMap);
	
	// INITIALISATION DU QUADTREE
	quadTree = createQuadTree(
		heightMap.points[heightMap.height - 1][0],
		heightMap.points[heightMap.height - 1][heightMap.width - 1],
		heightMap.points[0][heightMap.width - 1],
		heightMap.points[0][0],
		&heightMap,
		params
	);
	freePointChart(&heightMap);

	// INITIALISATION DE LA POSITION DE LA CAMERA
	camera.position.x = 0;
	camera.position.y = 0;
	camera.position.z = 1;
	camera.up.x = 0;
	camera.up.y = 0;
	camera.up.z = 1;
	camera.latitude = 0.0;
	camera.longitude = M_PI/2.0;

	// POSITIONS ARBRES
	srand (time(NULL));
	createTrees(trees);

	// PARAMETRES DE TEXTURE
	wireFrame = false;
	texturesLinks[0]=(char*)"doc/roche.jpg";
	texturesLinks[1] = (char*)"doc/arbre.jpg";

	for(int i=0; i<2;i++){
		textures[i] = creaTexture(texturesLinks[i]);
	}

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

static void reshapeFunc(int width, int height) {
	GLfloat  h = (GLfloat) width / (GLfloat) height ;
	
	/* dimension de l'écran GL */
	glViewport(0, 0, (GLint)width, (GLint)height);
	/* construction de la matrice de projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* définition de la camera */
	gluPerspective( camera.fov, h, camera.zNear, camera.zFar);		// Angle de vue, rapport largeur/hauteur, near, far

	/* Retour a la pile de matrice Modelview
			et effacement de celle-ci */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
			camera.position.x,	// CAMERA POSITION X
			camera.position.y,	// CAMERA POSITION Y
			camera.position.z,	// CAMERA POSITION Z
			camera.position.x + cos(camera.latitude)*sin(camera.longitude), // LOOK DIRECTION X
			camera.position.y + sin(camera.latitude)*sin(camera.longitude),	// LOOK DIRECTION Y
			camera.position.z + cos(camera.longitude),						// LOOK DIRECTION Z
			camera.up.x,
			camera.up.y,
			camera.up.z
		);

		// glColor3f(1.0,0.0,0.0);
		// glDrawRepere(2.0, textures);

		// glPushMatrix();
		// 	glDrawTrees(trees, camera, textures);
		// glPopMatrix();
		glDrawHeightMap(quadTree);

		float position[4] = {5.0,5.0,5.0,1.0};
		float black[3] = {0.0,0.0,0.0};
		float intensite[3] = {1000.0,1000.0,1000.0};
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0,GL_POSITION,position);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,intensite);
		glLightfv(GL_LIGHT0,GL_SPECULAR,black);
		//glLightf(GL_LIGHT0,GL_,black);
		//glLightf(GL_LIGHT0,GL_SPECULAR,black);

		glPushMatrix();
		glColor3f(1.0,1.0,1.0);
		glPopMatrix();

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
			if (camera.longitude > STEP_ANGLE) camera.longitude -= STEP_ANGLE;
			break;
		case GLUT_KEY_DOWN :
			if(camera.longitude < M_PI-STEP_ANGLE) camera.longitude += STEP_ANGLE;
			break;
		case GLUT_KEY_LEFT :
			camera.latitude += STEP_ANGLE;
			break;
		case GLUT_KEY_RIGHT :
			camera.latitude -= STEP_ANGLE;
			break;
		default:
			if(GLUT_ACTIVE_SHIFT){
				camera.position.z -= 1;
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
			camera.position.x += 1. * cos(camera.latitude);
			camera.position.y += 1. * sin(camera.latitude);
			break;
		case 'S' : case 's' : 
			camera.position.x -= 1. * cos(camera.latitude);
			camera.position.y -= 1. * sin(camera.latitude);
			break;
		case 'Q' : case 'q' : 
			camera.position.x += 1. * cos(camera.latitude + M_PI/2);
			camera.position.y += 1. * sin(camera.latitude + M_PI/2);
			break;
		case 'D' : case 'd' : 
			camera.position.x -= 1. * cos(camera.latitude + M_PI/2);
			camera.position.y -= 1. * sin(camera.latitude + M_PI/2);
			break;
		case ' ' :
			camera.position.z += 1;
			break;
		case 'F' : case 'f' : 
			if(wireFrame){
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				wireFrame = false;
			}else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				wireFrame = true;
			}
			break;
		default:
			printf("Appui sur la touche %c\n",c);
	}
	glutPostRedisplay();
}


int main (int argc, char** argv){
	// printPoint3D(quadTree->a);
	// printPoint3D(quadTree->b);
	// printPoint3D(quadTree->c);
	// printPoint3D(quadTree->d);
	// cout << endl;
	// printPoint3D(quadTree->getChildA()->a);
	// printPoint3D(quadTree->getChildA()->b);
	// printPoint3D(quadTree->getChildA()->c);
	// printPoint3D(quadTree->getChildA()->d);
	// cout << endl;
	// printPoint3D(quadTree->getChildB()->a);
	// printPoint3D(quadTree->getChildB()->b);
	// printPoint3D(quadTree->getChildB()->c);
	// printPoint3D(quadTree->getChildB()->d);
	// cout << endl;
	// printPoint3D(quadTree->getChildC()->a);
	// printPoint3D(quadTree->getChildC()->b);
	// printPoint3D(quadTree->getChildC()->c);
	// printPoint3D(quadTree->getChildC()->d);
	// cout << endl;
	// printPoint3D(quadTree->getChildD()->a);
	// printPoint3D(quadTree->getChildD()->b);
	// printPoint3D(quadTree->getChildD()->c);
	// printPoint3D(quadTree->getChildD()->d);

	/*/* traitement des paramètres du programme propres à GL */
	glutInit(&argc, argv);
	/* initialisation du mode d'affichage :                */
	/* RVB + ZBuffer + Double buffer.                      */
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	/* placement et dimentions originales de la fenêtre */
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(2000,2000);
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

