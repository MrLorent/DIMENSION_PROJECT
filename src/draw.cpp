#include "../include/draw.h"
#include "../include/objects.h"

float MIN_LEVEL = 0;
float FIRST_LEVEL = 0;
float SECOND_LEVEL = 0;
float THIRD_LEVEL = 0;
float MAX_LEVEL = 0;

/*################# TEXTURE #################*/

void initTextureLevels(float min, float max){
    float zSpacing = max - min;

    MIN_LEVEL = min;
    FIRST_LEVEL = zSpacing * 1/4;
    SECOND_LEVEL = zSpacing * 1/2;
    THIRD_LEVEL = zSpacing * 3/4;
    MAX_LEVEL = max;
}

void loadTextures(GLuint textures[15])
{
    char* texturesLinks[15];
    texturesLinks[0] = (char*)"doc/roche.jpg";
    texturesLinks[1] = (char*)"doc/arbre1.png";
    texturesLinks[2] = (char*)"doc/skybox-droite.png";
    texturesLinks[3] = (char*)"doc/skybox-devant.png";
    texturesLinks[4] = (char*)"doc/skybox-gauche.png";
    texturesLinks[5] = (char*)"doc/skybox-derriere.png";
    texturesLinks[6] = (char*)"doc/skybox-dessous.png";
    texturesLinks[7] = (char*)"doc/skybox-dessus.png";
    texturesLinks[8] = (char*)"doc/herbe.jpg";
    texturesLinks[9] = (char*)"doc/roche.jpg";
    texturesLinks[10] = (char*)"doc/roche2.jpg";
    texturesLinks[11] = (char*)"doc/neige.jpeg";
    texturesLinks[12] = (char*)"doc/arbre2.png";
    texturesLinks[13] = (char*)"doc/arbre3.png";
    texturesLinks[14] = (char*)"doc/arbre4.png";

    for(int i=0; i<15;i++){
      textures[i]=creaTexture(texturesLinks[i]);
    }
}

// Fonction permettant de créer une texture GLuint
// à partir d'un chemin vers une image
GLuint creaTexture(char* path){
    SDL_Surface* image = IMG_Load(path);
    if(NULL == image) {
        fprintf(stderr, "Echec du chargement de l'image %s\n", path);
        exit(EXIT_FAILURE);
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GLenum format;
    switch(image->format->BytesPerPixel) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            fprintf(stderr, "Format des pixels de l'image %s non supporte.\n", path);
            return EXIT_FAILURE;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);  
    
    SDL_FreeSurface(image);

    return texture_id;

}

/*############## FONCTION DRAW ##############*/

// Fonction permettant de générer un repère
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

// Fonction qui génère la skybox
void glDrawSkybox(float x,float y,float z,  GLuint textures[15])
{
  float D=50;

  glDepthMask(GL_FALSE);
  glColor4f(1, 1, 1, 1);
  
  //Dessous
  glBindTexture(GL_TEXTURE_2D,textures[6]);
  glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex3f(-D+x,-D+y,-D+z);
    glTexCoord2f(1,0); glVertex3f(+D+x,-D+y,-D+z);
    glTexCoord2f(1,1); glVertex3f(+D+x,+D+y,-D+z);
    glTexCoord2f(0,1); glVertex3f(-D+x,+D+y,-D+z);
  glEnd();
  glBindTexture(GL_TEXTURE_2D,0);

  //Dessus
  glBindTexture(GL_TEXTURE_2D,textures[7]);
  glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex3f(+D+x,-D+y,+D+z);
    glTexCoord2f(1,0); glVertex3f(-D+x,-D+y,+D+z);
    glTexCoord2f(1,1); glVertex3f(-D+x,+D+y,+D+z);
    glTexCoord2f(0,1); glVertex3f(+D+x,+D+y,+D+z);
  glEnd();
  glBindTexture(GL_TEXTURE_2D,0);

  //Devant
  glBindTexture(GL_TEXTURE_2D,textures[4]);
  glBegin(GL_QUADS);
    glTexCoord2f(0,1); glVertex3f(+D+x,-D+y,-D+z);
    glTexCoord2f(0,0); glVertex3f(+D+x,-D+y,+D+z);
    glTexCoord2f(1,0); glVertex3f(+D+x,+D+y,+D+z);
    glTexCoord2f(1,1); glVertex3f(+D+x,+D+y,-D+z);
  glEnd();
  glBindTexture(GL_TEXTURE_2D,0);


  //derriere
  glBindTexture(GL_TEXTURE_2D,textures[2]);
  glBegin(GL_QUADS);
    glTexCoord2f(1,0); glVertex3f(-D+x,-D+y,+D+z);
    glTexCoord2f(1,1); glVertex3f(-D+x,-D+y,-D+z);
    glTexCoord2f(0,1); glVertex3f(-D+x,+D+y,-D+z);
    glTexCoord2f(0,0); glVertex3f(-D+x,+D+y,+D+z);
  glEnd();
  glBindTexture(GL_TEXTURE_2D,0);

  //gauche
  glBindTexture(GL_TEXTURE_2D,textures[3]);
  glBegin(GL_QUADS);
    glTexCoord2f(1,1); glVertex3f(-D+x,+D+y,-D+z);
    glTexCoord2f(0,1); glVertex3f(+D+x,+D+y,-D+z);
    glTexCoord2f(0,0); glVertex3f(+D+x,+D+y,+D+z);
    glTexCoord2f(1,0); glVertex3f(-D+x,+D+y,+D+z);
  glEnd();
  glBindTexture(GL_TEXTURE_2D,0);

  //droite
  glBindTexture(GL_TEXTURE_2D,textures[5]);
  glBegin(GL_QUADS);
    glTexCoord2f(1,1); glVertex3f(+D+x,-D+y,-D+z);
    glTexCoord2f(0,1); glVertex3f(-D+x,-D+y,-D+z);
    glTexCoord2f(0,0); glVertex3f(-D+x,-D+y,+D+z);
    glTexCoord2f(1,0); glVertex3f(+D+x,-D+y,+D+z);
  glEnd();
  glBindTexture(GL_TEXTURE_2D,0);

  glDepthMask(GL_TRUE);
}


void glDrawHeightMap(QuadTree* quadTree, int currentLevel, Camera* camera, GLuint textures[15]){
    if(!quadTree)
    {
        return;
    }

    if(!camera->sees(quadTree->a, quadTree->b, quadTree->c, quadTree->d))
    {
        return;
    }

    if(quadTree->isLeaf() /*|| LevelOfDetailsReached(quadTree, currentLevel)*/)
    {
        // ON DESSINE LES ELEMENTS DE LA MAP

        /* On dessine le triangle en haut à gauche du quad */
        glDrawTriangle(quadTree->a, quadTree->b, quadTree->d, textures);

        /* On dessine le triangle en bas à droite du quad */
        glDrawTriangle(quadTree->b, quadTree->c, quadTree->d, textures);

        if(quadTree->hasTree)
        {
            /* On dessine les arbres s'il y en a */
            glDrawTree(quadTree->a, camera->latitude, textures);
            glDrawTree(quadTree->b, camera->latitude, textures);
            glDrawTree(quadTree->c, camera->latitude, textures);
            glDrawTree(quadTree->d, camera->latitude, textures);
        }
    }
    else
    {
        currentLevel++;
        glDrawHeightMap(quadTree->childA, currentLevel, camera, textures);
        glDrawHeightMap(quadTree->childB, currentLevel, camera, textures);
        glDrawHeightMap(quadTree->childC, currentLevel, camera, textures);
        glDrawHeightMap(quadTree->childD, currentLevel, camera, textures);
    }
}

bool LevelOfDetailsReached(QuadTree* quad, int level){

}

void glDrawTriangle(Point3D a, Point3D b, Point3D c, GLuint textures[15]){
    float averageHeight = (a.z + b.z + c.z)/3;
    if(averageHeight >= MIN_LEVEL && averageHeight <= FIRST_LEVEL)   glBindTexture(GL_TEXTURE_2D,textures[8]);
    if(averageHeight > FIRST_LEVEL && averageHeight <= SECOND_LEVEL)   glBindTexture(GL_TEXTURE_2D,textures[9]);
    if(averageHeight > SECOND_LEVEL && averageHeight <= THIRD_LEVEL)   glBindTexture(GL_TEXTURE_2D,textures[10]);
    if(averageHeight > THIRD_LEVEL && averageHeight <= MAX_LEVEL)    glBindTexture(GL_TEXTURE_2D,textures[11]);

    glBegin(GL_TRIANGLES);
        glColor4f(1, 1, 1, 1);
        glTexCoord2f(1,1); glVertex3f(a.x, a.y, a.z); 
        glTexCoord2f(0,1); glVertex3f(b.x, b.y, b.z);
        glTexCoord2f(0,0); glVertex3f(c.x,c.y,c.z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);
}

void glDrawTree(Point3D treePoint, float latitude, GLuint textures[15] ) {
    if(!treePoint.tree)
    {
      return;
    }

    if(treePoint.z >= MIN_LEVEL && treePoint.z <= FIRST_LEVEL)   glBindTexture(GL_TEXTURE_2D,textures[1]);
    if(treePoint.z > FIRST_LEVEL && treePoint.z <= SECOND_LEVEL)   glBindTexture(GL_TEXTURE_2D,textures[12]);
    if(treePoint.z > SECOND_LEVEL && treePoint.z <= THIRD_LEVEL)   glBindTexture(GL_TEXTURE_2D,textures[13]);
    if(treePoint.z > THIRD_LEVEL && treePoint.z <= MAX_LEVEL)  glBindTexture(GL_TEXTURE_2D,textures[14]);
    
    glPushMatrix();
        glTranslatef(treePoint.x, treePoint.y,treePoint.z);
        glRotatef(latitude*180/M_PI,0.,0.,1.);
        glColor4f(1, 1, 1, 1);
        glScalef(0.25,0.25,0.25);
        glPushMatrix();
            glBegin(GL_POLYGON);
            glTexCoord2f( 0,  0); glVertex3f(0,-0.5,1);
            glTexCoord2f( 1,  0); glVertex3f(0,0.5,1);
            glTexCoord2f( 1,  1); glVertex3f(0,0.5,0);
            glTexCoord2f( 0,  1); glVertex3f(0,-0.5,0);
            glEnd();
        glPopMatrix();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D,0);
}