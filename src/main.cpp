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

/*############# VARIABLES GLOBALES #############*/

/*---------- GLOBALES POUR LA CAMERA ----------*/
const float STEP_ANGLE = M_PI/90.;
float SPEED_FACTOR;
float HEIGHT_FACTOR;
Camera camera;


/*------------ GLOBALES POUR LA MAP -----------*/
int NB_TREES;
QuadTree* quadTree;
TreeChart treesToDraw;
Sun sunShine;


/*------------- GLOBALES TEXTURE --------------*/
bool wireFrame;
GLuint textures[15];


using namespace std;

/*############## INITIALISATION ##############*/

static void init() {
	Params params;
	PointChart heightMap;

	// RECUPERATION DES PARAMETRES .TIMAC
	params = createParams();
  	readParams(&params, &camera);
  	loadHeightMap(&params, &heightMap);

	// INITIALISATION DU SOLEIL
	initSun(&sunShine, params.xSize, params.ySize);

	// INITIALISATION DES ARBRES
	NB_TREES = heightMap.width/2;
	initTreeChart(&treesToDraw, NB_TREES);
	srand (time(NULL));
	LoadTrees(&heightMap, NB_TREES);

	// INITIALISATION DES PALIERS DU LOD
	initLODLevels(camera.zFar);
	
	// INITIALISATION DU QUADTREE
	quadTree = createQuadTree(
		heightMap.points[heightMap.height - 1][0],
		heightMap.points[heightMap.height - 1][heightMap.width - 1],
		heightMap.points[0][heightMap.width - 1],
		heightMap.points[0][0],
		&heightMap,
		params
	);

	cout << "Hauteur totale du quadTree :" << endl;
	cout << quadTree->getHeight() << endl;
	
	freePointChart(&heightMap);

	// INITIALISATION DES PARAMETRES DE CAMERA
	/* Paramètre de position de la camera dans la map*/
	camera.position.x = 0;
	camera.position.y = 0;
	camera.position.z = 1;
	/* Vecteur up : vecteur normal au plan du sol (0;0;1) */
	camera.up.x = 0;
	camera.up.y = 0;
	camera.up.z = 1;
	/* Paramètre de position du regard */
	camera.latitude = 0.0;
	camera.longitude = M_PI/2.0;
	/* Coefficient de vitesse calculer selon la taille de la map */
	SPEED_FACTOR = 0.001 * (params.xSize * params.ySize)/2;

	// INITIALISATION DES TEXTURES
	wireFrame = false;
	initTextureLevels(params.zMin, params.zMax);
	loadTextures(textures);

	// INITIALISATION DES PARAMETRES GL 
	/* couleur du fond (gris sombre) */
	glClearColor(0.3,0.3,0.3,0.0);
	/* activation du ZBuffer */
	glEnable( GL_DEPTH_TEST);

	/* lissage des couleurs sur les facettes */
	glShadeModel(GL_SMOOTH);

	/* INITIALISATION DE LA SCENE */
	//createCoordinates();
}

/*############ RESIZE DE LA FENETRE #############*/

static void reshapeFunc(int width, int height) {
	GLfloat  h = (GLfloat) width / (GLfloat) height ;
	
	camera.fovH = 2.0 * atan(tan(camera.getFovVInRadian() * 0.5) * h) * 180/M_PI;
	
	/* dimension de l'écran GL */
	glViewport(0, 0, (GLint)width, (GLint)height);
	/* construction de la matrice de projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* définition de la camera */
	gluPerspective(camera.fovV, h, camera.zNear, camera.zFar);		// Angle de vue, rapport largeur/hauteur, near, far

	/* Retour a la pile de matrice Modelview
			et effacement de celle-ci */
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*################# AFFICHAGE #################*/

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

		glColor3f(1.0,0.0,0.0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawSkybox(camera.position.x,camera.position.y,camera.position.z,textures,camera.zFar);

		glDrawRepere(2.0);

		treesToDraw.nbTrees = 0;
		quadTree->initTmpPoints();
		glDrawHeightMap(quadTree, &camera, textures, &treesToDraw, sunShine, wireFrame);
		glDrawTrees(&treesToDraw, camera.latitude, textures, sunShine);

		float position[4] = {5.0,5.0,5.0,1.0};
		float black[3] = {0.0,0.0,0.0};
		float intensite[3] = {1000.0,1000.0,1000.0};
		glEnable(GL_TEXTURE_2D);
		//glEnable(GL_LIGHTING);
		//glEnable(GL_LIGHT0);
		//glLightfv(GL_LIGHT0,GL_POSITION,position);
		//glLightfv(GL_LIGHT0,GL_DIFFUSE,intensite);
		//glLightfv(GL_LIGHT0,GL_SPECULAR,black);
		//glLightf(GL_LIGHT0,GL_,black);
		//glLightf(GL_LIGHT0,GL_SPECULAR,black);
		//glDisable(GL_LIGHTING);*/

	/* Fin du dessin */
	glPopMatrix();

	/* fin de la définition de la scène */
	glFinish();

	/* changement de buffer d'affichage */
	glutSwapBuffers();
}

void idle(void) {
    if(sunShine.moving)
	{
		sunShine.longitude -= 0.2 * STEP_ANGLE;
		sunShine.position.y = sunShine.origin.y - sunShine.radius * cos(sunShine.longitude);
		sunShine.position.z = sunShine.origin.z - sunShine.radius * sin(sunShine.longitude);
	}
    glutPostRedisplay();
}

/*########### ECOUTEURS D'EVENEMENTS ############*/

static void kbdSpFunc(int c, int x, int y) {
	/* sortie du programme si utilisation des touches ESC, */
	switch(c) {
		case GLUT_KEY_UP :
			if (camera.longitude > M_PI/6) camera.longitude -= STEP_ANGLE;
			break;
		case GLUT_KEY_DOWN :
			if(camera.longitude < M_PI-M_PI/6) camera.longitude += STEP_ANGLE;
			break;
		case GLUT_KEY_LEFT :
			camera.latitude += STEP_ANGLE;
			break;
		case GLUT_KEY_RIGHT :
			camera.latitude -= STEP_ANGLE;
			break;
		default:
			if(GLUT_ACTIVE_SHIFT){
				camera.position.z -= SPEED_FACTOR;
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
			camera.position.x += SPEED_FACTOR * cos(camera.latitude);
			camera.position.y += SPEED_FACTOR * sin(camera.latitude);
			break;
		case 'S' : case 's' : 
			camera.position.x -= SPEED_FACTOR * cos(camera.latitude);
			camera.position.y -= SPEED_FACTOR * sin(camera.latitude);
			break;
		case 'Q' : case 'q' : 
			camera.position.x += SPEED_FACTOR * cos(camera.latitude + M_PI/2);
			camera.position.y += SPEED_FACTOR * sin(camera.latitude + M_PI/2);
			break;
		case 'D' : case 'd' : 
			camera.position.x -= SPEED_FACTOR * cos(camera.latitude + M_PI/2);
			camera.position.y -= SPEED_FACTOR * sin(camera.latitude + M_PI/2);
			break;
		case 'L' : case 'l' :
			if(sunShine.moving){
				sunShine.moving = false;
			}else{
				sunShine.moving = true;
			}
			break;
		case 'P' : case 'p' : 
			sunShine.longitude += 0.05;
			sunShine.position.y += SPEED_FACTOR * cos(sunShine.longitude);
			sunShine.position.z += SPEED_FACTOR * sin(sunShine.longitude);
			break;
		case 'M' : case 'm' : 
			sunShine.longitude -= 0.05;
			sunShine.position.y -= SPEED_FACTOR * cos(sunShine.longitude);
			sunShine.position.z -= SPEED_FACTOR * sin(sunShine.longitude);
			break;
		case ' ' :
			camera.position.z += SPEED_FACTOR;
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

	glutIdleFunc(idle);

	/* boucle principale de gestion des événements */
	glutMainLoop();
	/* Cette partie du code n'est jamais atteinte */


	return 0;
}

