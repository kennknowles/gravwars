#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "../include/video.h"
#include "../include/input.h"

using namespace gravwars;

VideoCore video;
InputCore *input;
PngTexture *png;
GLuint textures[1];

void display() {
	//cerr << "drawing" << endl;
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glBindTexture( GL_TEXTURE_2D, textures[0] );

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);		glVertex2f(0.0, 0.0);
	glTexCoord2f(0.0, 1.0);		glVertex2f(0.0, 1.0);
	glTexCoord2f(1.0, 1.0);		glVertex2f(1.0, 1.0);
	glTexCoord2f(1.0, 0.0);		glVertex2f(1.0, 0.0);

	glEnd();

	glFlush();
	glutSwapBuffers();
}

void reshape( int width, int height ) {
	glViewport( 0, 0, width, height );
	
	glutPostRedisplay();
}

void mouse( int button, int state, int x, int y ) {
}

void motion( int x, int y ) {
}

void keyboard( unsigned char key, int x, int y ) {	
	InputType i;
	
	if( key == 'q' )
		exit(0);
}

int main( int argc, char* argv[] ) {	
	input = new InputCore();

	png = new PngTexture( argv[1] );

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
	glutCreateWindow("Texture test");
	glutReshapeWindow(600, 450);


	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	
	glutKeyboardFunc( keyboard );


	glGenTextures( 1, &textures[0] );
	glBindTexture( GL_TEXTURE_2D, textures[0] );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexImage2D( GL_TEXTURE_2D, 0, png->channels, png->width, png->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
					png->data );

	glEnable( GL_TEXTURE_2D );
	glClearColor( 0.0, 0.0, 0.5, 0.0 );
	glClearDepth( 1.0 );
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( 0, 1.0, 0.0, 1.0, 0, 100 );
	
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();

	return( 0 );
}
