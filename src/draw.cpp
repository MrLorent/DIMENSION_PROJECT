#include "../include/draw.h"
#include "../include/objects.h"
#include "../include/geometry.h"

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

/*############## LUMIERE ##############*/

Color3f GetLight(Sun sunShine, Point3D a, Point3D b, Point3D c){
    Vector3D v1 = createVectorFromPoints(a,b);
    Vector3D v2 = createVectorFromPoints(a,c);
    Vector3D normale = prodVect(v1,v2);
    Point3D centre = createPoint((a.x+b.x+c.x)/3, (a.y+b.y+c.y)/3, (a.z+b.z+c.z)/3);
    Vector3D vectSunShine = createVectorFromPoints(sunShine.position, centre);
    normale = normalize(normale);
    Color3f triangleLight = multColor(sunShine.color , dot(normale, normalize(vectSunShine))/ (norm(vectSunShine)*norm(vectSunShine)));
    
    return triangleLight;
}

/*############## FONCTION DRAW ##############*/

void initLODLevels(float zFar){
  LOD_LEVEL_1 = zFar * 2/16;
  LOD_LEVEL_2 = zFar * 3/16;
  LOD_LEVEL_3 = zFar * 4/16;
  LOD_LEVEL_4 = zFar * 5/16;
}

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

void glDrawHeightMap(QuadTree* quadTree, Camera* camera, GLuint textures[15], TreeChart* treesToDraw, Sun sunShine){
    if(!quadTree)
    {
        return;
    }

    if(!camera->sees(quadTree->a, quadTree->b, quadTree->c, quadTree->d))
    {
        return;
    }

    if(LevelOfDetailsReached(quadTree, camera->position))
    {
        // ON DESSINE LES ELEMENTS DE LA MAP

        /* On dessine le triangle en haut à gauche du quad */
        glDrawTriangle(quadTree->tmpA, quadTree->tmpB, quadTree->tmpD, textures, sunShine);

        /* On dessine le triangle en bas à droite du quad */
        glDrawTriangle(quadTree->tmpB, quadTree->tmpC, quadTree->tmpD, textures, sunShine);

        if(quadTree->hasTree)
        {
            /* On dessine les arbres s'il y en a */
            registerTrees(quadTree, treesToDraw, camera->position);
        }
    }
    else
    {
        glDrawHeightMap(quadTree->childA, camera, textures, treesToDraw, sunShine);
        glDrawHeightMap(quadTree->childB, camera, textures, treesToDraw, sunShine);
        glDrawHeightMap(quadTree->childC, camera, textures, treesToDraw, sunShine);
        glDrawHeightMap(quadTree->childD, camera, textures, treesToDraw, sunShine);
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
              if(quad->childC) quad->getChildC()->tmpB.z = (quad->b.z + quad->c.z)/2;
              if(quad->childB) quad->getChildB()->tmpC.z = (quad->b.z + quad->c.z)/2; 
            }

            if(distanceD > LOD_LEVEL)
            {
              if(quad->childC) quad->getChildC()->tmpD.z = (quad->d.z + quad->c.z)/2;
              if(quad->childD) quad->getChildD()->tmpC.z = (quad->d.z + quad->c.z)/2;
            }
            break;

        case TOP_RIGHT:
            distanceA = norm(createVectorFromPoints(position, quad->a));
            distanceC = norm(createVectorFromPoints(position, quad->c));

            if(distanceA > LOD_LEVEL)
            {
              if(quad->childA) quad->getChildA()->tmpD.z = (quad->a.z + quad->d.z)/2;
              if(quad->childD) quad->getChildD()->tmpA.z = (quad->a.z + quad->d.z)/2; 
            }

            if(distanceC > LOD_LEVEL)
            {
              if(quad->childC) quad->getChildC()->tmpD.z = (quad->c.z + quad->d.z)/2;
              if(quad->childD) quad->getChildD()->tmpC.z = (quad->c.z + quad->d.z)/2;
            }
            break;

        case BOTTOM_RIGHT:
            distanceB = norm(createVectorFromPoints(position, quad->b));
            distanceD = norm(createVectorFromPoints(position, quad->d));

            if(distanceB > LOD_LEVEL)
            {
              if(quad->childA) quad->getChildA()->tmpB.z = (quad->a.z + quad->b.z)/2;
              if(quad->childB) quad->getChildB()->tmpA.z = (quad->a.z + quad->b.z)/2; 
            }

            if(distanceD > LOD_LEVEL)
            {
              if(quad->childA) quad->getChildA()->tmpD.z = (quad->a.z + quad->d.z)/2;
              if(quad->childD) quad->getChildD()->tmpA.z = (quad->a.z + quad->d.z)/2;
            }
            break;

        case BOTTOM_LEFT:
            distanceA = norm(createVectorFromPoints(position, quad->a));
            distanceC = norm(createVectorFromPoints(position, quad->c));

            if(distanceA > LOD_LEVEL)
            {
              if(quad->childA) quad->getChildA()->tmpB.z = (quad->a.z + quad->b.z)/2;
              if(quad->childB) quad->getChildB()->tmpA.z = (quad->a.z + quad->b.z)/2; 
            }

            if(distanceC > LOD_LEVEL)
            {
              if(quad->childB) quad->getChildB()->tmpC.z = (quad->b.z + quad->c.z)/2;
              if(quad->childC) quad->getChildC()->tmpB.z = (quad->b.z + quad->c.z)/2;
            }
            break;

        default:
          break;
    }
}

void glDrawTriangle(Point3D a, Point3D b, Point3D c, GLuint textures[15], Sun sunShine){
    float averageHeight = (a.z + b.z + c.z)/3;
    if(averageHeight <= TEXTURE_LEVEL_1)   glBindTexture(GL_TEXTURE_2D,textures[8]);
    if(averageHeight > TEXTURE_LEVEL_1 && averageHeight <= TEXTURE_LEVEL_2)   glBindTexture(GL_TEXTURE_2D,textures[9]);
    if(averageHeight > TEXTURE_LEVEL_2 && averageHeight <= TEXTURE_LEVEL_3)   glBindTexture(GL_TEXTURE_2D,textures[10]);
    if(averageHeight > TEXTURE_LEVEL_3)    glBindTexture(GL_TEXTURE_2D,textures[11]);

    Color3f triangleLight = GetLight(sunShine, a, b, c);

    glBegin(GL_TRIANGLES);
        glColor4f(triangleLight.r, triangleLight.g, triangleLight.b, 1);
        glTexCoord2f(1,1); glVertex3f(a.x, a.y, a.z); 
        glTexCoord2f(0,1); glVertex3f(b.x, b.y, b.z);
        glTexCoord2f(0,0); glVertex3f(c.x,c.y,c.z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);
}

void glDrawTrees(TreeChart* trees, float latitude, GLuint textures[15], Sun sunShine) {
    for(int i = 0; i < trees->nbTrees; i++)
    {
      Point3D a = createPoint(0.,-0.5,1.);
      Point3D b = createPoint(0.,0.5,1.);
      Point3D c = createPoint(0.,0.5,0.);

      Color3f triangleLight = GetLight(sunShine, a , b , c);
      if(trees->trees[i].z <= TEXTURE_LEVEL_1)   glBindTexture(GL_TEXTURE_2D,textures[1]);
      if(trees->trees[i].z > TEXTURE_LEVEL_1 && trees->trees[i].z <= TEXTURE_LEVEL_2)   glBindTexture(GL_TEXTURE_2D,textures[12]);
      if(trees->trees[i].z > TEXTURE_LEVEL_2 && trees->trees[i].z <= TEXTURE_LEVEL_3)   glBindTexture(GL_TEXTURE_2D,textures[13]);
      if(trees->trees[i].z > TEXTURE_LEVEL_3)  glBindTexture(GL_TEXTURE_2D,textures[14]);
      
      glPushMatrix();
          glTranslatef(trees->trees[i].x, trees->trees[i].y, trees->trees[i].z);
          glRotatef(latitude*180/M_PI,0.,0.,1.);
          glColor4f(triangleLight.r,triangleLight.g,triangleLight.b, 1);
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
}

void glDrawSun (Sun* sun, GLuint textures[16]) {
    glPushMatrix();
        glTranslatef(0., 10 * cos(sun->longitude), 10 * sin(sun->longitude));
    glPopMatrix();
}