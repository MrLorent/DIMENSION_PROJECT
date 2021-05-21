#include "../include/objects.h"

// Plante l'arbre
Tree createTree(float x, float y, float z){
    Tree newTree ;
    newTree.x=x;
    newTree.y=y;
    newTree.z=z;
    return newTree;
}

void glDrawTree(Tree t) {
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glPushMatrix();
		glTranslatef(t.x - cos(camera.latitude)*sin(camera.longitude), 
		t.y - sin(camera.latitude)*sin(camera.longitude),	
		t.z);
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