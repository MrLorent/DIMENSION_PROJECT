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

float LOD_LEVEL_1 = 0;
float LOD_LEVEL_2 = 0;
float LOD_LEVEL_3 = 0;
float LOD_LEVEL_4 = 0;

void initTreeChart(TreeChart* chart, int NB_TREES)
{
    chart->nbTrees = 0;
    chart->trees = new Point3D[NB_TREES];
}

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

/*############## LUMIERE ##############*/

Color3f GetLight(Sun sun, Point3D a, Point3D b, Point3D c){
    Vector3D v1 = createVectorFromPoints(a,b);
    Vector3D v2 = createVectorFromPoints(a,c);
    Vector3D normale = prodVect(v1,v2);
    Point3D centre = createPoint((a.x+b.x+c.x)/3, (a.y+b.y+c.y)/3, (a.z+b.z+c.z)/3);
    Vector3D vectSunShine = createVectorFromPoints(sun.position, centre);
    normale = normalize(normale);
    Color3f triangleLight = multColor(sun.color , sun.shininess * dot(normale, normalize(vectSunShine))/ (norm(vectSunShine)*norm(vectSunShine)));
    
    return triangleLight;
}

/*############## FONCTION DRAW ##############*/

// Initialise les differents seuils du LOD
// en fonction du zFar de la camera
void initLODLevels(float zFar){
  LOD_LEVEL_1 = zFar * 3/32;
  LOD_LEVEL_2 = zFar * 6/32;
  LOD_LEVEL_3 = zFar * 9/32;
  LOD_LEVEL_4 = zFar * 18/32;
}

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

void registerTrees(QuadTree* quadTree, TreeChart* treesToDraw, Point3D cam)
{
    Point3D inOrderPoints[4];
    quadTree->orderByDistance(inOrderPoints, cam);

    for(int i = 0; i < 4; i++)
    {
        if(inOrderPoints[i].tree)
        {
            treesToDraw->trees[treesToDraw->nbTrees] = inOrderPoints[i];
            treesToDraw->nbTrees++;
        }
    }
}

bool LevelOfDetailsReached(QuadTree* quad, Point3D position){

    int closestPoint = quad->getClosestCorner(position);
    float distance = quad->getDistanceFrom(position);

    // ON DETERMINE SI ON DOIT CORRIGER LES CRACKS OU PAS
    if(distance <= LOD_LEVEL_1 && quad->height == 2)
    {
        dealWithCracks(quad, position, closestPoint, LOD_LEVEL_1);
    }
    else if (distance <= LOD_LEVEL_2 && quad->height == 3)
    {
        dealWithCracks(quad, position, closestPoint, LOD_LEVEL_2);
    }
    else if (distance <= LOD_LEVEL_3 && quad->height == 4)
    {
        dealWithCracks(quad, position, closestPoint, LOD_LEVEL_3);
    }
    else if (distance <= LOD_LEVEL_4 && quad->height == 5)
    {
        dealWithCracks(quad, position, closestPoint, LOD_LEVEL_4);
    }
    
    // ON DETERMINE SI ON DOIT DESSINER OU CONTINUER A DESCENDRE
    if(quad->isLeaf())
    {
        return true;
    }
    else if (distance > LOD_LEVEL_1 && quad->height == 2)
    {
        return true;
    }
    else if (distance > LOD_LEVEL_2 && quad->height == 3)
    {
        return true;
    }
    else if (distance > LOD_LEVEL_3 && quad->height == 4)
    {
        return true;
    }
    else if (distance > LOD_LEVEL_4 && quad->height == 5)
    {
        return true;
    }
    else
    {
      return false;
    }
}

void dealWithCracks(QuadTree* quad, Point3D position, int closest, float LOD_LEVEL)
{
    float distanceA, distanceB, distanceC, distanceD;

    switch (closest)
    {
        case TOP_LEFT:
            distanceB = norm(createVectorFromPoints(position, quad->b));
            distanceD = norm(createVectorFromPoints(position, quad->d));

            if(distanceB > LOD_LEVEL)
            {
                if(quad->childC){
                    quad->getChildC()->tmpB.z = (quad->b.z + quad->c.z)/2;
                }else{
                    quad->getChildD()->tmpB.z = (quad->b.z + quad->c.z)/2;
                }
                
                if(quad->childB){
                    quad->getChildB()->tmpC.z = (quad->b.z + quad->c.z)/2;
                }else{
                    quad->getChildA()->tmpC.z = (quad->b.z + quad->c.z)/2;
                } 
            }

            if(distanceD > LOD_LEVEL)
            {
                if(quad->childC){
                    quad->getChildC()->tmpD.z = (quad->d.z + quad->c.z)/2;
                    quad->getChildD()->tmpC.z = (quad->d.z + quad->c.z)/2;
                }else{
                    quad->getChildD()->tmpC.z = (quad->d.z + quad->c.z)/2;
                }
              
            }
            break;

        case TOP_RIGHT:
            distanceA = norm(createVectorFromPoints(position, quad->a));
            distanceC = norm(createVectorFromPoints(position, quad->c));

            if(distanceA > LOD_LEVEL)
            {
              if(quad->childA){
                  quad->getChildA()->tmpD.z = (quad->a.z + quad->d.z)/2;
                  quad->getChildD()->tmpA.z = (quad->a.z + quad->d.z)/2;
              }else{
                  quad->getChildD()->tmpA.z = (quad->a.z + quad->d.z)/2;
              }
            }

            if(distanceC > LOD_LEVEL)
            {
                if(quad->childC){
                    quad->getChildC()->tmpD.z = (quad->c.z + quad->d.z)/2;
                    quad->getChildD()->tmpC.z = (quad->c.z + quad->d.z)/2;
                }else{
                    quad->getChildD()->tmpC.z = (quad->c.z + quad->d.z)/2;
                }
            }
            break;

        case BOTTOM_RIGHT:
            distanceB = norm(createVectorFromPoints(position, quad->b));
            distanceD = norm(createVectorFromPoints(position, quad->d));

            if(distanceB > LOD_LEVEL)
            {
                if(quad->childA){
                    quad->getChildA()->tmpB.z = (quad->a.z + quad->b.z)/2;
                }else{
                    quad->getChildD()->tmpB.z = (quad->a.z + quad->b.z)/2;
                }

                if(quad->childB){
                    quad->getChildB()->tmpA.z = (quad->a.z + quad->b.z)/2;
                }else if(quad->childC){
                    quad->getChildC()->tmpA.z = (quad->a.z + quad->b.z)/2;
                }
            }

            if(distanceD > LOD_LEVEL)
            {
              if(quad->childA){
                  quad->getChildA()->tmpD.z = (quad->a.z + quad->d.z)/2;
              }
              quad->getChildD()->tmpA.z = (quad->a.z + quad->d.z)/2;
            }
            break;

        case BOTTOM_LEFT:
            distanceA = norm(createVectorFromPoints(position, quad->a));
            distanceC = norm(createVectorFromPoints(position, quad->c));

            if(distanceA > LOD_LEVEL)
            {
                if(quad->childA){
                    quad->getChildA()->tmpB.z = (quad->a.z + quad->b.z)/2;
                }else{
                    quad->getChildD()->tmpB.z = (quad->a.z + quad->b.z)/2;
                }

                if(quad->childB){
                    quad->getChildB()->tmpA.z = (quad->a.z + quad->b.z)/2;
                }else if(quad->childC){
                    quad->getChildC()->tmpA.z = (quad->a.z + quad->b.z)/2;
                }
            }

            if(distanceC > LOD_LEVEL)
            {
                if(quad->childC){
                    quad->getChildC()->tmpB.z = (quad->b.z + quad->c.z)/2;
                }else{
                    quad->getChildD()->tmpB.z = (quad->b.z + quad->c.z)/2;
                }

                if(quad->childB){
                    quad->getChildB()->tmpC.z = (quad->b.z + quad->c.z)/2;
                }else if(quad->childA){
                    quad->getChildA()->tmpC.z = (quad->b.z + quad->c.z)/2;
                }
            }
            break;

        default:
          break;
    }
}

void glDrawTriangle(Point3D a, Point3D b, Point3D c, int quadLevel, GLuint mapTextures[4], Sun sunShine, bool wireframe){
    float averageHeight = (a.z + b.z + c.z)/3;
    if(!wireframe)
    {
    if(averageHeight <= TEXTURE_LEVEL_1)   glBindTexture(GL_TEXTURE_2D,mapTextures[0]);
    if(averageHeight > TEXTURE_LEVEL_1 && averageHeight <= TEXTURE_LEVEL_2)   glBindTexture(GL_TEXTURE_2D,mapTextures[1]);
    if(averageHeight > TEXTURE_LEVEL_2 && averageHeight <= TEXTURE_LEVEL_3)   glBindTexture(GL_TEXTURE_2D,mapTextures[2]);
    if(averageHeight > TEXTURE_LEVEL_3)    glBindTexture(GL_TEXTURE_2D,mapTextures[3]);


    Color3f triangleLight = GetLight(sunShine, a, b, c);

    glBegin(GL_TRIANGLES);
        glColor4f(triangleLight.r, triangleLight.g, triangleLight.b, 1);
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

void glDrawTrees(TreeChart* trees, float latitude, GLuint treeTextures[4], Sun sunShine) {
    for(int i = 0; i < trees->nbTrees; i++)
    {
      Color3f triangleLight = GetLight(sunShine, TREE_LIGHT_MODEL.a, TREE_LIGHT_MODEL.b, TREE_LIGHT_MODEL.c);

      if(trees->trees[i].z <= TEXTURE_LEVEL_1)   glBindTexture(GL_TEXTURE_2D,treeTextures[0]);
      if(trees->trees[i].z > TEXTURE_LEVEL_1 && trees->trees[i].z <= TEXTURE_LEVEL_2)   glBindTexture(GL_TEXTURE_2D,treeTextures[1]);
      if(trees->trees[i].z > TEXTURE_LEVEL_2 && trees->trees[i].z <= TEXTURE_LEVEL_3)   glBindTexture(GL_TEXTURE_2D,treeTextures[2]);
      if(trees->trees[i].z > TEXTURE_LEVEL_3)  glBindTexture(GL_TEXTURE_2D,treeTextures[3]);
      
      glPushMatrix();
          glTranslatef(trees->trees[i].x, trees->trees[i].y, trees->trees[i].z);
          glRotatef(latitude*180/M_PI,0.,0.,1.);
          glColor4f(triangleLight.r,triangleLight.g,triangleLight.b, 1);
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

