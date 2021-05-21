#include "../include/draw.h"
#include "../include/objects.h"

//---------FONCTION CREATION TEXTURE--------------

GLuint creaTexture(char *chemin){
    SDL_Surface* image = IMG_Load(chemin);
    if(NULL == image) {
        fprintf(stderr, "Echec du chargement de l'image %s\n", chemin);
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
            fprintf(stderr, "Format des pixels de l'image %s non supporte.\n", chemin);
            return EXIT_FAILURE;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);  
    
    SDL_FreeSurface(image);

    return texture_id;

}

//-----------------DRAW ---------------

void glDrawRepere(float length, GLuint textures[10]) {
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

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor4f(1, 1, 1, 1);
	glBegin(GL_POLYGON);
		glTexCoord3f( 0,  0, 0); glVertex3f(-5,  5,  0);
		glTexCoord3f( 1,  0, 0); glVertex3f( 5,  5,  0);
		glTexCoord3f( 1,  1, 0); glVertex3f( 5, -5,  0);
		glTexCoord3f( 0,  1, 0); glVertex3f(-5, -5,  0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	/*glTranslatef(3.0,0.0,1.0);
	glBegin(GL_QUADS);
		glColor3ub(255,255,255); //face rouge
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
    glEnd();*/
}

void glDrawHeightMap(QuadTree* quadTree){
    if(!quadTree)
    {
        return;
    }

void glDrawTrees(Tree trees[6], Camera camera, GLuint textures[10]) {
	for (int i=0 ; i<6 ; i++){
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glPushMatrix();
			
			glTranslatef(trees[i].x - cos(camera.latitude)*sin(camera.longitude), 
			trees[i].y - sin(camera.latitude)*sin(camera.longitude),	
			trees[i].z);
			glRotatef((90+camera.latitude*180/M_PI),0.0,0.0,1);
			glColor4f(1, 1, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord3f( 0,  0, 0); glVertex3f(-0.5,-0.5,1);
			glTexCoord3f( 0,  1, 0); glVertex3f(-0.5,-0.5,0);
			glTexCoord3f( 1,  1, 0); glVertex3f(0.5,-0.5,0);
			glTexCoord3f( 1,  0, 0); glVertex3f(0.5,-0.5,1);
			glEnd();
			
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D,0);
	}
}