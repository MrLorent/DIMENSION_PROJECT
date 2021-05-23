#include "../include/draw.h"
#include "../include/objects.h"

//---------FONCTION CREATION TEXTURE--------------

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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);  
    
    SDL_FreeSurface(image);

    return texture_id;

}

//-----------------DRAW ---------------

void glDrawSkybox(float x,float y,float z,  GLuint textures[15])
{
  float D=50;

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

 
}

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

void glDrawHeightMap(QuadTree* quadTree, Camera* camera, GLuint textures[15]){
    if(!quadTree)
    {
        return;
    }

	/*if(!camera->sees(quadTree->a, quadTree->b, quadTree->c, quadTree->d))
	{
		return;
	}*/

    if(quadTree->isLeaf())
    {
        float averageHeight=(quadTree->a.z+quadTree->b.z+quadTree->d.z)/3;
        if(averageHeight>=0 && averageHeight<=0.25)glBindTexture(GL_TEXTURE_2D,textures[8]);
        if(averageHeight>0.25 && averageHeight<=0.50)glBindTexture(GL_TEXTURE_2D,textures[9]);
        if(averageHeight>0.50 && averageHeight<=0.75)glBindTexture(GL_TEXTURE_2D,textures[10]);
        if(averageHeight>0.75 && averageHeight<=1)glBindTexture(GL_TEXTURE_2D,textures[11]);

        glBegin(GL_TRIANGLES);
            glColor4f(1, 1, 1, 1);
            glTexCoord2f(1,1); glVertex3f(quadTree->a.x,quadTree->a.y,quadTree->a.z); 
            glTexCoord2f(0,1); glVertex3f(quadTree->b.x,quadTree->b.y,quadTree->b.z);
            glTexCoord2f(0,0); glVertex3f(quadTree->d.x,quadTree->d.y,quadTree->d.z);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,0);

        averageHeight=(quadTree->b.z+quadTree->c.z+quadTree->d.z)/3;
        if(averageHeight>=0 && averageHeight<=0.25)glBindTexture(GL_TEXTURE_2D,textures[8]);
        if(averageHeight>0.25 && averageHeight<=0.50)glBindTexture(GL_TEXTURE_2D,textures[9]);
        if(averageHeight>0.50 && averageHeight<=0.75)glBindTexture(GL_TEXTURE_2D,textures[10]);
        if(averageHeight>0.75 && averageHeight<=1)glBindTexture(GL_TEXTURE_2D,textures[11]);

        glBegin(GL_TRIANGLES);
            glColor4f(1, 1, 1, 1);
            glTexCoord2f(1,1); glVertex3f(quadTree->b.x,quadTree->b.y,quadTree->b.z);
            glTexCoord2f(0,1); glVertex3f(quadTree->c.x,quadTree->c.y,quadTree->c.z);
            glTexCoord2f(0,0); glVertex3f(quadTree->d.x,quadTree->d.y,quadTree->d.z);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,0);

        glDrawTree(quadTree->a,camera, textures);
        glDrawTree(quadTree->b,camera, textures);
        glDrawTree(quadTree->c,camera, textures);
        glDrawTree(quadTree->d,camera, textures);

    }
    else
    {
        glDrawHeightMap(quadTree->childA, camera, textures);
        glDrawHeightMap(quadTree->childB, camera, textures);
        glDrawHeightMap(quadTree->childC, camera, textures);
        glDrawHeightMap(quadTree->childD, camera, textures);
    }
}

void glDrawTree(Point3D treePoint, Camera camera, GLuint textures[10] ) {
  if(treePoint.tree ==  true) {
		glBindTexture(GL_TEXTURE_2D, textures[1]);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();
      glTranslatef(treePoint.x, treePoint.y,treePoint.z);
      glRotatef(camera.latitude*180/M_PI,0.,0.,1.);
			glColor4f(1, 1, 1, 1);
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