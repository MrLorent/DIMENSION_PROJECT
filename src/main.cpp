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

const float STEP_ANGLE = M_PI/90.;

/*---------- GLOBALES POUR LA CAMERA ----------*/
float SPEED_FACTOR;
Camera camera;


/*------------ GLOBALES POUR LA MAP -----------*/
int NB_TREES;
QuadTree* quadTree;
TreeChart treesToDraw;
Sun sun;


/*------------- GLOBALES TEXTURE --------------*/
bool wireFrame;
GLuint mapTextures[4];
GLuint treeTextures[4];
GLuint skyboxTextures[12];

using namespace std;

/*############## INITIALISATION ##############*/

static void init() {
	Params params;
	PointChart heightMap;

	// RECUPERATION DES PARAMETRES .TIMAC
	params = createParams();
  	readParams(&params, &camera);
  	loadHeightMap(&params, &heightMap);

	// INITIALISATION DES ARBRES
	NB_TREES = heightMap.width;
	initTreeChart(&treesToDraw, NB_TREES);
	srand (time(NULL));
	LoadTrees(&heightMap, NB_TREES);

	// INITIALISATION DU QUADTREE
	/* Initialisation du quadtree à partir du tableau de pixels */
	quadTree = createQuadTree(
		heightMap.points[heightMap.height - 1][0],
		heightMap.points[heightMap.height - 1][heightMap.width - 1],
		heightMap.points[0][heightMap.width - 1],
		heightMap.points[0][0],
		&heightMap,
		params
	);

	/* Initialisation de la position finale des nodes */
	quadTree->initNodesHeight();
	
	/* Initialisation des niveaux du LOD en fonction du zFar */
	initLODLevels(camera.zFar);

	// INITIALISATION DES TEXTURES
	wireFrame = false;
	initTextureLevels(params.zMin, params.zMax);
	loadTextures(mapTextures, treeTextures, skyboxTextures);

	// INITIALISATION DU SOLEIL
	initSun(&sun, params.xSize, params.ySize);

	// INITIALISATION DE LA CAMERA
	/* Paramètre de position de la camera dans la map*/
	initCamera(&camera, quadTree->childA->c);
	/* Coefficient de vitesse calculer selon la taille de la map */
	SPEED_FACTOR = 0.00005 * (params.xSize * params.ySize)/2;

	/* liberation de la memoire liee au tableau de pixels */
	freePointChart(&heightMap);

	// INITIALISATION DES PARAMETRES GL 
	/* couleur du fond (gris sombre) */
	glClearColor(0.3,0.3,0.3,0.0);
	/* activation du ZBuffer */
	glEnable( GL_DEPTH_TEST);

	/* lissage des couleurs sur les facettes */
	glShadeModel(GL_SMOOTH);
}

/*############ RESIZE DE LA FENETRE #############*/

static void reshapeFunc(int width, int height) {
	GLfloat  h = (GLfloat) width / (GLfloat) height ;
	
	/* Calcul de la fov horizontal à partir de la
	fov verticale et du ratio de l'ecran */
	camera.fovH = 2.0 * atan(tan(camera.getFovVInRadian() * 0.5) * h) * 180/M_PI;
	
	/* dimension de l'écran GL */
	glViewport(0, 0, (GLint)width, (GLint)height);
	/* construction de la matrice de projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* définition de la camera */
	gluPerspective(camera.fovV, h, camera.zNear, camera.zFar);

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
	glEnable(GL_TEXTURE_2D);

	/* Gestion de la transparence des png */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Debut du dessin de la scène */
	glPushMatrix();

		/* placement de la caméra */
		gluLookAt(
			camera.position.x,	// CAMERA POSITION X
			camera.position.y,	// CAMERA POSITION Y
			camera.position.z,	// CAMERA POSITION Z
			camera.position.x + cos(camera.latitude) * sin(camera.longitude), // LOOK DIRECTION X
			camera.position.y + sin(camera.latitude) * sin(camera.longitude),	// LOOK DIRECTION Y
			camera.position.z + cos(camera.longitude),						// LOOK DIRECTION Z
			camera.up.x,
			camera.up.y,
			camera.up.z
		);

		/* DESSIN DE LA SCENE */

		/* Dessin de la skybox */
		glDrawSkybox(camera.position.x,camera.position.y,camera.position.z, skyboxTextures,camera.zFar,wireFrame);

		glDrawRepere(2.0);

		/* Remise a zero du nombre d'arbre a afficher dans la scene */
		treesToDraw.nbTrees = 0;

		/* Remise a zero des modifications liees a la gestion des cracks */
		quadTree->initTmpPoints();

		/* Dessin de la map et initialisation des arbres a dessiner */
		glDrawHeightMap(quadTree, &camera, mapTextures, &treesToDraw, sun, wireFrame);

		/* Dessin des arbres */
		if(!wireFrame) glDrawTrees(&treesToDraw, camera.latitude, treeTextures, sun);
		
	/* Fin du dessin */
	glPopMatrix();

	/* fin de la définition de la scène */
	glFinish();

	/* changement de buffer d'affichage */
	glutSwapBuffers();
}

void idle(void) {
    if(sun.moving)
	{
		/* Si le soleil est en mode loop, on incremente sa latitude, et on met a jour sa position */
		sun.longitude -= 0.2 * STEP_ANGLE;
		sun.position.y = sun.origin.y - sun.radius * cos(sun.longitude);
		sun.position.z = sun.origin.z - sun.radius * sin(sun.longitude);
	}
    glutPostRedisplay();
}

/*########### ECOUTEURS D'EVENEMENTS ############*/

static void kbdSpFunc(int c, int x, int y) {
	/* sortie du programme si utilisation des touches ESC, */
	switch(c) {

		// CONTROLES LIEES AU REGARD DE LA CAMERA
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

		// CONTROLES LIEES AU DEPLACEMENT DE LA CAMERA
		case 'P' : case 'p' :
			if(!camera.locked){
				camera.position.z -= SPEED_FACTOR;
			}
			break;

		default:
			cout << "Touche non gérée par l'application pressée." << endl;
	}

	glutPostRedisplay();
}

static void kbdFunc(unsigned char c, int x, int y) {
	/* sortie du programme si utilisation de la touche ESC, */
	switch(c) {
		case 27 :
			exit(0);
			break;

		// CONTROLES LIEES AU DEPLACEMENT DE LA CAMERA
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
		case ' ' :
			if(!camera.locked){
				camera.position.z += SPEED_FACTOR;
			}
			break;
		case 'C' : case 'c' : 
			if(camera.locked){
				camera.locked = false;
			}else{
				camera.locked = true;
			}
			break;

		// TOUCHES LIEES A LA GESTION DU SOLEIL
		case 'L' : case 'l' :
			if(sun.moving){
				sun.moving = false;
			}else{
				sun.moving = true;
			}
			break;
		case 'M' : case 'm' : 
			if(!sun.moving){
				sun.longitude += 0.05;
				sun.position.y = sun.origin.y - sun.radius * cos(sun.longitude);
				sun.position.z = sun.origin.z - sun.radius * sin(sun.longitude);
			}
			break;
		case 'K' : case 'k' : 
			if(!sun.moving){
				sun.longitude -= 0.05;
				sun.position.y = sun.origin.y - sun.radius * cos(sun.longitude);
				sun.position.z = sun.origin.z - sun.radius * sin(sun.longitude);
			}
			break;

		// TOUCHE LIEE AU MODE DE VISION FILAIRE
		case 'F' : case 'f' : 
			if(wireFrame){
				wireFrame = false;
			}else{
				wireFrame = true;
			}
			break;
			
		default:
			cout << "Touche non gérée par l'application pressée." << endl;
	}
	glutPostRedisplay();
}


int main (int argc, char** argv){

	/* Traitement des paramètres du programme propres à GL */
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

	/* Initialisation de tous les paramètres de la map */
	init();

	/* association de la fonction callback de redimensionnement */
	glutReshapeFunc(reshapeFunc);
	/* association de la fonction callback d'affichage */
	glutDisplayFunc(drawFunc);
	/* association de la fonction callback d'événement du clavier */
	glutKeyboardFunc(kbdFunc);
	/* association de la fonction callback d'événement du clavier (touches spéciales) */
	glutSpecialFunc(kbdSpFunc);
	/* association de la fonction callback d'affichage en continue*/
	glutIdleFunc(idle);

	/* boucle principale de gestion des événements */
	glutMainLoop();

	return 0;
}

