#include "../include/draw.h"

const Color3ub WIREFRAME_COLORS[5] = {
    createColor(255,0,255), // rgb(9, 241, 247) ROSE
    createColor(255,80,175), 
    createColor(255,125,125), 
    createColor(255,175,80), 
    createColor(255,255,0) // rgb(255, 255, 0) JAUNE
};

const Triangle TREE_LIGHT_MODEL = createTriangle(
    createPoint(0.,-0.5,1.),
    createPoint(0.,0.5,1.),
    createPoint(0.,0.5,0.)
);

float TEXTURE_LEVEL_1 = 0;
float TEXTURE_LEVEL_2 = 0;
float TEXTURE_LEVEL_3 = 0;

/*################# TEXTURE #################*/

void initTextureLevels(float min, float max){
    float zSpacing = max - min;

    TEXTURE_LEVEL_1 = zSpacing * 1/4;
    TEXTURE_LEVEL_2 = zSpacing * 1/2;
    TEXTURE_LEVEL_3 = zSpacing * 3/4;
}

void loadTextures(GLuint mapTextures[4], GLuint treeTextures[4], GLuint skyboxTextures[12])
{
    char* texturesLinks[21];
    // TEXTURES DU SOL
    texturesLinks[0] = (char*)"doc/text1.png";
    texturesLinks[1] = (char*)"doc/text2.png";
    texturesLinks[2] = (char*)"doc/text3.png";
    texturesLinks[3] = (char*)"doc/text4.png";
    // TEXTURES DES ARBRES
    texturesLinks[4] = (char*)"doc/arbre1.png";
    texturesLinks[5] = (char*)"doc/arbre2.png";
    texturesLinks[6] = (char*)"doc/arbre3.png";
    texturesLinks[7] = (char*)"doc/arbre4.png";
    // TEXTURES DE LA SKYBOX
    texturesLinks[8] = (char*)"doc/skybox-gauche.png";
    texturesLinks[9] = (char*)"doc/skybox-devant.png";
    texturesLinks[10] = (char*)"doc/skybox-droite.png";
    texturesLinks[11] = (char*)"doc/skybox-derriere.png";
    texturesLinks[12] = (char*)"doc/skybox-dessous.png";
    texturesLinks[13] = (char*)"doc/skybox-dessus.png";
    texturesLinks[14] = (char*)"doc/starbox-droite.png";
    texturesLinks[15] = (char*)"doc/starbox-devant.png";
    texturesLinks[16] = (char*)"doc/starbox-gauche.png";
    texturesLinks[17] = (char*)"doc/starbox-derriere.png";
    texturesLinks[18] = (char*)"doc/starbox-dessous.png";
    texturesLinks[19] = (char*)"doc/starbox-dessus.png";
    texturesLinks[20] = (char*)"doc/menu.png";

    // TEXTURES DU SOL
    for(int i=0; i<4;i++){
      mapTextures[i]=creaTexture(texturesLinks[i]);
    }

    // TEXTURES DES ARBRES
    for(int i=0; i<4;i++){
      treeTextures[i]=creaTexture(texturesLinks[i+4]);
    }

    // TEXTURES DE LA SKYBOX
    for(int i=0; i<12;i++){
      skyboxTextures[i]=creaTexture(texturesLinks[i+8]);
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

void bindTexture(float height, GLuint textures[4])
{
    if(height <= TEXTURE_LEVEL_1)
    {
        glBindTexture(GL_TEXTURE_2D,textures[0]);
    }
    else if(height > TEXTURE_LEVEL_1 && height <= TEXTURE_LEVEL_2)
    {
        glBindTexture(GL_TEXTURE_2D,textures[1]);
    }
    else if(height > TEXTURE_LEVEL_2 && height <= TEXTURE_LEVEL_3)
    {
        glBindTexture(GL_TEXTURE_2D,textures[2]);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D,textures[3]);
    }
}

/*############## FONCTION DRAW ##############*/

// Dessine les trois axes du repere
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

// Dessine la skybox
void glDrawSkybox(float x, float y, float z,  GLuint skyboxTextures[12], float zFar, bool wireframe)
{
  float D = sqrt(zFar);

  glDepthMask(GL_FALSE);
    glColor4f(1, 1, 1, 1);
    //Dessous
    if(wireframe){
        glBindTexture(GL_TEXTURE_2D,skyboxTextures[10]);
    }else{
        glBindTexture(GL_TEXTURE_2D,skyboxTextures[4]);
    }
    glBegin(GL_QUADS);
      glTexCoord2f(0,0); glVertex3f(-D+x,-D+y,-D+z);
      glTexCoord2f(1,0); glVertex3f(+D+x,-D+y,-D+z);
      glTexCoord2f(1,1); glVertex3f(+D+x,+D+y,-D+z);
      glTexCoord2f(0,1); glVertex3f(-D+x,+D+y,-D+z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);

    //Dessus
    if(wireframe){
        glBindTexture(GL_TEXTURE_2D,skyboxTextures[11]);
    }else{
        glBindTexture(GL_TEXTURE_2D,skyboxTextures[5]);
    }
    glBegin(GL_QUADS);
      glTexCoord2f(0,0); glVertex3f(+D+x,-D+y,+D+z);
      glTexCoord2f(1,0); glVertex3f(-D+x,-D+y,+D+z);
      glTexCoord2f(1,1); glVertex3f(-D+x,+D+y,+D+z);
      glTexCoord2f(0,1); glVertex3f(+D+x,+D+y,+D+z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);

    //Devant (x positif)
    if(wireframe){
        glBindTexture(GL_TEXTURE_2D,skyboxTextures[7]);
    }else{
        glBindTexture(GL_TEXTURE_2D,skyboxTextures[1]);
    }
    glBegin(GL_QUADS);
      glTexCoord2f(0,1); glVertex3f(+D+x,-D+y,-D+z);
      glTexCoord2f(0,0); glVertex3f(+D+x,-D+y,+D+z);
      glTexCoord2f(1,0); glVertex3f(+D+x,+D+y,+D+z);
      glTexCoord2f(1,1); glVertex3f(+D+x,+D+y,-D+z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);


    //derriere (x négatif)
    if(wireframe){
        glBindTexture(GL_TEXTURE_2D,skyboxTextures[9]);
    }else{
        glBindTexture(GL_TEXTURE_2D,skyboxTextures[3]);
    }
    glBegin(GL_QUADS);
      glTexCoord2f(1,0); glVertex3f(-D+x,-D+y,+D+z);
      glTexCoord2f(1,1); glVertex3f(-D+x,-D+y,-D+z);
      glTexCoord2f(0,1); glVertex3f(-D+x,+D+y,-D+z);
      glTexCoord2f(0,0); glVertex3f(-D+x,+D+y,+D+z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);

    //gauche (y positif)
    if(wireframe){
        glBindTexture(GL_TEXTURE_2D,skyboxTextures[8]);
    }else{
        glBindTexture(GL_TEXTURE_2D,skyboxTextures[2]);
    }
    glBegin(GL_QUADS);
      glTexCoord2f(1,1); glVertex3f(-D+x,+D+y,-D+z);
      glTexCoord2f(0,1); glVertex3f(+D+x,+D+y,-D+z);
      glTexCoord2f(0,0); glVertex3f(+D+x,+D+y,+D+z);
      glTexCoord2f(1,0); glVertex3f(-D+x,+D+y,+D+z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);

    //droite
    if(wireframe){
        glBindTexture(GL_TEXTURE_2D,skyboxTextures[6]);
    }else{
        glBindTexture(GL_TEXTURE_2D,skyboxTextures[0]);
    }
    glBegin(GL_QUADS);
      glTexCoord2f(1,1); glVertex3f(+D+x,-D+y,-D+z);
      glTexCoord2f(0,1); glVertex3f(-D+x,-D+y,-D+z);
      glTexCoord2f(0,0); glVertex3f(-D+x,-D+y,+D+z);
      glTexCoord2f(1,0); glVertex3f(+D+x,-D+y,+D+z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);
  glDepthMask(GL_TRUE);

}

void glDrawHeightMap(QuadTree* quadTree, Camera* camera, GLuint mapTextures[4], TreeChart* treesToDraw, Sun sunShine, bool wireframe){
    if(!quadTree)
    {
        return;
    }

    // FRUSTUM CULING
    if(!camera->sees(quadTree->a, quadTree->b, quadTree->c, quadTree->d))
    {
        return;
    }

    // LOD
    if(LevelOfDetailsReached(quadTree, camera->position))
    {
        /* Ajustement de la hauteur de la camera pour le mode "camera fixe" */
        if(camera->locked) camera->adjustHeight(quadTree);
         
        // ON DESSINE LES ELEMENTS DE LA MAP

        /* On dessine le triangle en haut à gauche du quad */
        glDrawTriangle(quadTree->tmpA, quadTree->tmpB, quadTree->tmpD, quadTree->height, mapTextures, sunShine, wireframe);

        /* On dessine le triangle en bas à droite du quad */
        glDrawTriangle(quadTree->tmpB, quadTree->tmpC, quadTree->tmpD, quadTree->height, mapTextures, sunShine, wireframe);

        if(quadTree->hasTree)
        {
            /* On dessine les arbres s'il y en a */
            registerTrees(quadTree, treesToDraw, camera->position);
        }
    }
    else
    {
        glDrawHeightMap(quadTree->childA, camera, mapTextures, treesToDraw, sunShine, wireframe);
        glDrawHeightMap(quadTree->childB, camera, mapTextures, treesToDraw, sunShine, wireframe);
        glDrawHeightMap(quadTree->childC, camera, mapTextures, treesToDraw, sunShine, wireframe);
        glDrawHeightMap(quadTree->childD, camera, mapTextures, treesToDraw, sunShine, wireframe);
    }
}


void glDrawTriangle(Point3D a, Point3D b, Point3D c, int quadLevel, GLuint mapTextures[4], Sun sun, bool wireframe){
    if(!wireframe)
    {
        float averageHeight = (a.z + b.z + c.z)/3;
        bindTexture(averageHeight, mapTextures);

        Color3f light = sun.getLight(a, b, c);

        glBegin(GL_TRIANGLES);
            glColor4f(light.r, light.g, light.b, 1);
            glTexCoord2f(1,1); glVertex3f(a.x, a.y, a.z); 
            glTexCoord2f(0,1); glVertex3f(b.x, b.y, b.z);
            glTexCoord2f(0,0); glVertex3f(c.x,c.y,c.z);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,0);
    }
    else
    {
        glLineWidth(1.25);
        glBegin(GL_LINE_LOOP);
            glColor3ub(
                WIREFRAME_COLORS[quadLevel-1].r,
                WIREFRAME_COLORS[quadLevel-1].g,
                WIREFRAME_COLORS[quadLevel-1].b
            );
            glVertex3f(a.x, a.y, a.z); 
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x,c.y,c.z);
        glEnd();
    }
}

void glDrawTrees(TreeChart* trees, float latitude, GLuint treeTextures[4], Sun sun) {
    for(int i = 0; i < trees->nbTrees; i++)
    {
      Color3f light = sun.getLight(TREE_LIGHT_MODEL.a, TREE_LIGHT_MODEL.b, TREE_LIGHT_MODEL.c);

      bindTexture(trees->trees[i].z, treeTextures);
      
      glPushMatrix();
          glTranslatef(trees->trees[i].x, trees->trees[i].y, trees->trees[i].z);
          glRotatef(latitude*180/M_PI,0.,0.,1.);
          glColor4f(light.r,light.g,light.b, 1);
          glScalef(0.75,0.75,0.75);
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
}

